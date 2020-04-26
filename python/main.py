from time import sleep
import numpy as np
import math
import multiprocessing


AVAILABLE_TOKEN_LENGHT = [n ** 2 for n in range(1, 16)]


class GameOfLife(object):
    def __init__(self, x_scale, y_scale, multithreaded=False):
        self.width = 16 * x_scale
        self.height = 16 * y_scale
        self.board = np.zeros((self.width, self.height), dtype=int)
        self.multithreaded = multithreaded
        self.processes = max([l for l in AVAILABLE_TOKEN_LENGHT if multiprocessing.cpu_count() >= l])
        self.tokens_per_direction = round(math.sqrt(self.processes))
        self.token_width = round(self.board.shape[0] / math.sqrt(self.processes))
        self.token_height = round(self.board.shape[1] / math.sqrt(self.processes))
        self.thread_areas = []
        self.init()

    def init(self):
        for x in range(self.width):
            for y in range(self.height):
                self.board[x][y] = np.random.randint(0, 2)

    def parse(self):
        if self.multithreaded:
            token_queue = []
            for x in range(0, self.tokens_per_direction):
                for y in range(0, self.tokens_per_direction):
                    
                    x_init = x * self.token_width
                    y_init = y * self.token_height

                    x_end = x * self.token_width + self.token_width
                    y_end = y * self.token_height + self.token_height

                    if len(self.thread_areas) < self.processes:
                        self.thread_areas.append((x_init, y_init, self.token_width, self.token_height))

                    token_queue.append((self.board[x_init:x_end, y_init:y_end], (x, y)))

            # processing token queue
            parsed_tokens = []
            with multiprocessing.Pool(self.processes) as pool:
                parsed_tokens = pool.map(self.parse_token, token_queue)
                
            for token in parsed_tokens:
                x_init = token[1][0] * self.token_width
                y_init = token[1][1] * self.token_height

                x_end = token[1][0] * self.token_width + self.token_width
                y_end = token[1][1] * self.token_height + self.token_height

                for x in range(x_init, x_end):
                    for y in range(y_init, y_end):
                        self.board[x][y] = token[0][x % self.token_width][y % self.token_height]
        else:
            self.thread_areas.append((0, 0, self.board.shape[0], self.board.shape[1]))
            self.board = self.parse_token([self.board, (0, 0)])[0]

    def parse_token(self, token):
        token_id = token[1]
        token = token[0]

        copy = np.copy(token)
        
        for x in range(token.shape[0]):
            neighbour = 0
            for y in range(token.shape[1]):
                sliced = copy[max(0, x-1):min(x+2, token.shape[0]+1),max(0, y-1):min(y+2, token.shape[1]+1)]
                neighbour = np.sum(sliced) - copy[x][y]
                
                if copy[x][y] and (neighbour < 2 or neighbour > 3):
                    token[x][y] = 0
                elif not copy[x][y] and neighbour == 3:
                    token[x][y] = 1

        return token, token_id

    def get_board(self):
        return self.board, self.thread_areas

    def __str__(self):
        result = ""
        for x in range(self.width):
            for y in range(self.height):
                result += "*" if self.board[x][y] else " "
            result += "\n"
        return result


if __name__ == "__main__":
    gol = GameOfLife(4, 4, multithreaded=True)
    while True:
        gol.parse()
        print(gol)