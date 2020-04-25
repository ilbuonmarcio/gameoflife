import pygame
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
        self.sliced_chunks = []
        self.init()

    def init(self):
        for x in range(self.width):
            for y in range(self.height):
                self.board[x][y] = np.random.randint(0, 2)

    def parse(self):
        if self.multithreaded:
            payload_queue = []
            self.sliced_chunks.clear()

            for i in range(0, self.tokens_per_direction):
                for j in range(0, self.tokens_per_direction):
                    directions = (
                        (1 if j == 0 else 0),
                        (1 if i + 1 == self.tokens_per_direction else 0),
                        (1 if j + 1 == self.tokens_per_direction else 0),
                        (1 if i == 0 else 0)
                    )

                    x_init = i * self.token_width
                    y_init = j * self.token_height

                    x_end = (i + 1) * self.token_width
                    y_end = (j + 1) * self.token_height

                    self.sliced_chunks.append([x_init, y_init, self.token_width, self.token_height])

                    sliced_area = self.board[max(0, x_init):min(x_end+1, self.board.shape[0]+1),max(0, y_init):min(y_end+1, self.board.shape[1]+1)]

                    payload_queue.append([sliced_area, (i, j), directions])

            # TODO: parallelize execution after that
            # For now we're doing it normally as usual, one at a time,
            # but we will fake as if it was done asynchronously and not this way
            parsed_tokens = []
            for payload in payload_queue:
                parsed_tokens.append(self.parse_token(payload[0], payload[1], payload[2]))
            
            # Reassembling tokens
            for token in parsed_tokens:
                token_position = token[1]
                token_touches = token[2]
                token = token[0]

                x_init = token_position[0] * self.token_width
                y_init = token_position[1] * self.token_height

                x_end = (token_position[0] + 1) * self.token_width
                y_end = (token_position[1]+ 1) * self.token_height

                for x in range(x_init, x_end):
                    for y in range(y_init, y_end):
                        self.board[x][y] = token[x % self.token_width - 1][y % self.token_height - 1]

        else:
            self.board = self.parse_token(self.board)[0]

    def parse_token(self, token, token_id=(0, 0), touches=(0, 0, 0, 0)):
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

        if touches[0]:
            token = np.delete(token, token.shape[0]-1, 0)
        if touches[1]:
            token = np.delete(token, 0, 1)
        if touches[2]:
            token = np.delete(token, 0, 0)
        if touches[3]:
            token = np.delete(token, token.shape[1]-1, 1)

        return token, token_id, touches

    def get_board(self):
        return self.board, self.sliced_chunks

    def __str__(self):
        result = ""
        for x in range(self.width):
            for y in range(self.height):
                result += "*" if self.board[x][y] else " "
            result += "\n"
        return result
