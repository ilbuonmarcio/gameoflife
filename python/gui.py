import pygame
from pygame.locals import *
from main import GameOfLife

pygame.init()

GAME_RES = WIDTH, HEIGHT = 512, 512
FPS = 60
GAME_TITLE = 'Blank'

window = pygame.display.set_mode(GAME_RES, HWACCEL|HWSURFACE|DOUBLEBUF)
pygame.display.set_caption(GAME_TITLE)
clock = pygame.time.Clock()

# Game Values

background_color = (150, 150, 150)
count = 1

gol = GameOfLife(8, 8, multithreaded=True)
cell_scale = 4

# End of Game Values

# Game loop
game_ended = False
while not game_ended:

    # Event handling
    for event in pygame.event.get():
        if event.type == QUIT:
            game_ended  = True
            break
        if event.type == KEYDOWN:
            if event.key == K_ESCAPE:
                game_ended  = True
                break

    # Game logic
    count += 1

    gol.parse()

    # Display update
    pygame.Surface.fill(window, background_color)

    current_board, thread_areas = gol.get_board()
    for x in range(0, len(current_board)):
        for y in range(0, len(current_board[x])):
            pygame.draw.rect(window, (255 * current_board[x][y], 255 * current_board[x][y], 255 * current_board[x][y]), (x * cell_scale, y * cell_scale, cell_scale, cell_scale))
    
    for area in thread_areas:
        pygame.draw.rect(window, (0, 255, 0), (area[0] * cell_scale, area[1] * cell_scale, area[2] * cell_scale, area[3] * cell_scale), 1)

    pygame.display.update()
    clock.tick(FPS)

pygame.quit()
exit(0)