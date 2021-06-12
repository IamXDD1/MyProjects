import pygame
import random
from pygame import mixer

sightRow = 19
sightColumn = 31
mapRow = 50
mapColumn = 50
pygame.init()
screen = pygame.display.set_mode((32 * sightColumn, 32 * sightRow))
#                                (width           , height       )

# title and icon

pygame.display.set_caption("ShabbyCraft 1.0")
icon = pygame.image.load(
    "images/window_icon.png")
pygame.display.set_icon(icon)

# background music

mixer.music.load(
    "music/bgm_.mp3")
mixer.music.play(-1)

# sound effects

walkSound = mixer.Sound("sound/walkSound.mp3")
pickStickSound = mixer.Sound("sound/pickStickSound.mp3")
pickRockSound = mixer.Sound("sound/pickRockSound.wav")

# images
playerImg = pygame.image.load(
    "images/playerOnDirt.png")
dirtImg = pygame.image.load(
    "images/dirt.png")
stickImg = pygame.image.load(
    "images/stickOnDirt.png")
rockImg = pygame.image.load(
    "images/rockOnDirt.png")

# texts

rock_amout = 0
stick_amout = 0

fontSize = 20

font = pygame.font.Font('freesansbold.ttf', fontSize)

textX = 16
textY = 16


def showText():
    rockAmount = font.render("Rock: " + str(rock_amout), True, (0, 0, 0))
    screen.blit(rockAmount, (textX, textY))
    stickAmount = font.render("Stick: " + str(stick_amout), True, (0, 0, 0))
    screen.blit(stickAmount, (textX, textY + fontSize * 1.5))

# functions


def renderDirt(x, y):
    screen.blit(dirtImg, (x, y))


TYPE_ROCK = 0
TYPE_STICK = 1


def renderEntity(type, x, y):
    if type == TYPE_ROCK:
        screen.blit(rockImg, (x, y))
    elif type == TYPE_STICK:
        screen.blit(stickImg, (x, y))


def renderPlayer(x, y):
    screen.blit(playerImg, (x, y))


entities = []


def generateEntity():
    rand_type = random.randint(0, 1)
    rand_x = random.randint(0, mapColumn - 1)
    rand_y = random.randint(0, mapRow - 1)

    overlapped = True

    while overlapped:
        overlapped = False
        for index in range(len(entities)):
            if rand_x == entities[index][1] and entities[index][2]:
                overlapped = True
                change = random.randint(0, 3)
                if change == 0:
                    rand_x += 1
                elif change == 1:
                    rand_x -= 1
                elif change == 2:
                    rand_y += 1
                else:
                    rand_y -= 1
                break

    entity = [rand_type, rand_x, rand_y]
    entities.append(entity)


# game loop
isRunning = True

playerX = 30
playerY = 30

deltaX = 0
deltaY = 0

sightLeftBnd = 0
sightUpBnd = 0

ENTITIES_CNT = 40


def saveGame():
    save = open("saves/player.txt", "r+")
    save.write(str(playerX) + ", " + str(playerY) + ", " +
               str(rock_amout) + ", " + str(stick_amout))
    save.close()

# load saves


save = open("saves/player.txt", "r+")
status = save.read().split(", ")
playerX = int(status[0])
playerY = int(status[1])
rock_amout = int(status[2])
stick_amout = int(status[3])
save.close()


gameRunning = True
while gameRunning:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            gameRunning = False
        if event.type == pygame.KEYDOWN:
            walkSound.play()
            if event.key == pygame.K_w:
                if playerY > 0:
                    playerY -= 1
            if event.key == pygame.K_s:
                if playerY < mapRow - 1:
                    playerY += 1
            if event.key == pygame.K_d:
                if playerX < mapColumn - 1:
                    playerX += 1
            if event.key == pygame.K_a:
                if playerX > 0:
                    playerX -= 1

    for index in range(len(entities)):
        # remove entity when overlap with player
        if playerX == entities[index - 1][1] and playerY == entities[index - 1][2]:
            if entities[index - 1][0] == TYPE_ROCK:
                rock_amout += 1
                pickRockSound.play()
            elif entities[index - 1][0] == TYPE_STICK:
                stick_amout += 1
                pickStickSound.play()
            del entities[index - 1]
            break
    while len(entities) < ENTITIES_CNT:
        generateEntity()

    # Boundary adjusting mechanism
    if playerX > sightLeftBnd + (sightColumn / 2) - 1:
        if (sightLeftBnd + sightColumn + 1 <= mapColumn):
            sightLeftBnd += 1
    if playerX < sightLeftBnd + (sightColumn / 2) - 1:
        if (sightLeftBnd > 0):
            sightLeftBnd -= 1
    if playerY > sightUpBnd + (sightRow / 2) - 1:
        if (sightUpBnd + sightRow + 1 <= mapRow):
            sightUpBnd += 1
    if playerY < sightUpBnd + (sightRow / 2) - 1:
        if (sightUpBnd > 0):
            sightUpBnd -= 1
    # ----------------------------

    for row in range(mapRow):
        for column in range(mapColumn):
            if row >= sightUpBnd and column >= sightLeftBnd:
                currX = (column - sightLeftBnd) * 32
                currY = (row - sightUpBnd) * 32
                if row == playerY and column == playerX:
                    renderPlayer(currX, currY)
                else:
                    rendered = False
                    for index in range(len(entities)):
                        if entities[index][1] == column and entities[index][2] == row:
                            renderEntity(entities[index][0], currX, currY)
                            rendered = True
                            break
                    if not rendered:
                        renderDirt(currX, currY)
            if column > sightLeftBnd + sightColumn:
                break
        if row > sightUpBnd + sightRow:
            break
    showText()
    pygame.display.update()
    saveGame()
