from os import P_WAIT, stat
from typing import Text
import pygame
import random
import sys
import math

# Pygame Setup
pygame.init()

clock = pygame.time.Clock()
pygame.time.set_timer(pygame.USEREVENT, 1000)

pixel_per_block = 64
block_row = 9
block_column = 15

screen_width = block_column * pixel_per_block  # 15 * 64 = 960
screen_height = block_row * pixel_per_block  # 9 * 64 = 576

screen = pygame.display.set_mode((screen_width, screen_height))
buffer_surface = pygame.Surface(
    (pixel_per_block * block_column, pixel_per_block * block_row))
pygame.display.set_caption("Dungeon")
pygame.display.set_icon(pygame.image.load("images/slime.png"))
pygame.mouse.set_visible(False)

fontSize = 20
font = pygame.font.Font('freesansbold.ttf', fontSize)

textX = block_column * pixel_per_block - 95
textY = block_row * pixel_per_block - 100

# Images

playerImg = pygame.image.load("images/player.png")
slimeImg = pygame.image.load("images/slime.png")
tileImg = pygame.image.load("images/tile.png")
fireballImg = pygame.image.load("images/fireball.png")
aimImg = pygame.image.load("images/aim.png")

potionImg1 = pygame.transform.scale2x(pygame.transform.scale2x(
    pygame.image.load("images/bluePotion/tile000.png")))
potionImg2 = pygame.transform.scale2x(pygame.transform.scale2x(
    pygame.image.load("images/bluePotion/tile001.png")))
potionImg3 = pygame.transform.scale2x(pygame.transform.scale2x(
    pygame.image.load("images/bluePotion/tile002.png")))
potionImg4 = pygame.transform.scale2x(pygame.transform.scale2x(
    pygame.image.load("images/bluePotion/tile003.png")))
potionImg5 = pygame.transform.scale2x(pygame.transform.scale2x(
    pygame.image.load("images/bluePotion/tile004.png")))
potionImg6 = pygame.transform.scale2x(pygame.transform.scale2x(
    pygame.image.load("images/bluePotion/tile005.png")))
potionImg7 = pygame.transform.scale2x(pygame.transform.scale2x(
    pygame.image.load("images/bluePotion/tile006.png")))
potionImg8 = pygame.transform.scale2x(pygame.transform.scale2x(
    pygame.image.load("images/bluePotion/tile007.png")))

potionImg1_org = pygame.image.load("images/bluePotion/tile000.png")
potionImg2_org = pygame.image.load("images/bluePotion/tile001.png")
potionImg3_org = pygame.image.load("images/bluePotion/tile002.png")
potionImg4_org = pygame.image.load("images/bluePotion/tile003.png")
potionImg5_org = pygame.image.load("images/bluePotion/tile004.png")
potionImg6_org = pygame.image.load("images/bluePotion/tile005.png")
potionImg7_org = pygame.image.load("images/bluePotion/tile006.png")
potionImg8_org = pygame.image.load("images/bluePotion/tile007.png")

coinImg = pygame.image.load("images/coin.png")
heartImg = pygame.image.load("images/heart.png")
broken_heartImg = pygame.image.load("images/broken_heart.png")

potionImg1_org_temp = pygame.image.load("images/bluePotion/tile000.png")
coinImg_temp = pygame.image.load("images/coin.png")
heartImg_temp = pygame.image.load("images/heart.png")
broken_heartImg_temp = pygame.image.load("images/broken_heart.png")

healthBar_1000 = pygame.image.load("images/healthBar/healthBar-100%.png")
healthBar_975 = pygame.image.load("images/healthBar/healthBar-97.5%.png")
healthBar_950 = pygame.image.load("images/healthBar/healthBar-95%.png")
healthBar_925 = pygame.image.load("images/healthBar/healthBar-92.5%.png")
healthBar_900 = pygame.image.load("images/healthBar/healthBar-90%.png")
healthBar_875 = pygame.image.load("images/healthBar/healthBar-87.5%.png")
healthBar_850 = pygame.image.load("images/healthBar/healthBar-85%.png")
healthBar_825 = pygame.image.load("images/healthBar/healthBar-82.5%.png")
healthBar_800 = pygame.image.load("images/healthBar/healthBar-80%.png")
healthBar_775 = pygame.image.load("images/healthBar/healthBar-77.5%.png")
healthBar_750 = pygame.image.load("images/healthBar/healthBar-75%.png")
healthBar_725 = pygame.image.load("images/healthBar/healthBar-72.5%.png")
healthBar_700 = pygame.image.load("images/healthBar/healthBar-70%.png")
healthBar_675 = pygame.image.load("images/healthBar/healthBar-67.5%.png")
healthBar_650 = pygame.image.load("images/healthBar/healthBar-65%.png")
healthBar_625 = pygame.image.load("images/healthBar/healthBar-62.5%.png")
healthBar_600 = pygame.image.load("images/healthBar/healthBar-60%.png")
healthBar_575 = pygame.image.load("images/healthBar/healthBar-57.5%.png")
healthBar_550 = pygame.image.load("images/healthBar/healthBar-55%.png")
healthBar_525 = pygame.image.load("images/healthBar/healthBar-52.5%.png")
healthBar_500 = pygame.image.load("images/healthBar/healthBar-50%.png")
healthBar_475 = pygame.image.load("images/healthBar/healthBar-47.5%.png")
healthBar_450 = pygame.image.load("images/healthBar/healthBar-45%.png")
healthBar_425 = pygame.image.load("images/healthBar/healthBar-42.5%.png")
healthBar_400 = pygame.image.load("images/healthBar/healthBar-40%.png")
healthBar_375 = pygame.image.load("images/healthBar/healthBar-37.5%.png")
healthBar_350 = pygame.image.load("images/healthBar/healthBar-35%.png")
healthBar_325 = pygame.image.load("images/healthBar/healthBar-32.5%.png")
healthBar_300 = pygame.image.load("images/healthBar/healthBar-30%.png")
healthBar_275 = pygame.image.load("images/healthBar/healthBar-27.5%.png")
healthBar_250 = pygame.image.load("images/healthBar/healthBar-25%.png")
healthBar_225 = pygame.image.load("images/healthBar/healthBar-22.5%.png")
healthBar_200 = pygame.image.load("images/healthBar/healthBar-20%.png")
healthBar_175 = pygame.image.load("images/healthBar/healthBar-17.5%.png")
healthBar_150 = pygame.image.load("images/healthBar/healthBar-15%.png")
healthBar_125 = pygame.image.load("images/healthBar/healthBar-12.5%.png")
healthBar_100 = pygame.image.load("images/healthBar/healthBar-10%.png")
healthBar_75 = pygame.image.load("images/healthBar/healthBar-7.5%.png")
healthBar_50 = pygame.image.load("images/healthBar/healthBar-5%.png")
healthBar_25 = pygame.image.load("images/healthBar/healthBar-2.5%.png")
healthBar_0 = pygame.image.load("images/healthBar/healthBar-0%.png")


menuBackground = pygame.image.load("images/menuBackground.jpg")
statusBackground = pygame.image.load("images/planks.png")

endGameTitle = pygame.image.load("images/endGame.png")
endGameTitle = pygame.transform.scale2x(endGameTitle)

gameTitle1 = pygame.image.load("images/gameTitle2_1.png")
gameTitle1 = pygame.transform.scale2x(gameTitle1)

gameTitle2 = pygame.image.load("images/gameTitle2_2.png")
gameTitle2 = pygame.transform.scale2x(gameTitle2)

tileImg = pygame.transform.scale2x(tileImg)

howToPlay_1 = pygame.image.load("images/howToPlay_1.png")
howToPlay_2 = pygame.image.load("images/howToPlay_2.png")
howToPlay_3 = pygame.image.load("images/howToPlay_3.png")
howToPlay_4 = pygame.image.load("images/howToPlay_4.png")
howToPlay_5 = pygame.image.load("images/howToPlay_5.png")
howToPlay_6 = pygame.image.load("images/howToPlay_6.png")

howToPlay_1 = pygame.transform.scale2x(howToPlay_1)
howToPlay_2 = pygame.transform.scale2x(howToPlay_2)
howToPlay_3 = pygame.transform.scale2x(howToPlay_3)
howToPlay_4 = pygame.transform.scale2x(howToPlay_4)
howToPlay_5 = pygame.transform.scale2x(howToPlay_5)
howToPlay_6 = pygame.transform.scale2x(howToPlay_6)

HOW_TO_PLAY_BG = (66, 66, 66)

# Audio

pickPotionSound = pygame.mixer.Sound("audio/pickPotion.wav")
slimeWalk = pygame.mixer.Sound("audio/slimeWalk.ogg")
slimeDeath = pygame.mixer.Sound("audio/slimeDeath.wav")
shootFireball = pygame.mixer.Sound("audio/shootFireball.wav")
clickButtonSound = pygame.mixer.Sound("audio/clickButtonSound.wav")

# Map(BG)

TILE = 0

myMap = []
for iter in range(block_row):
    temp_row = []
    for inner_iter in range(block_column):
        temp_row.append(TILE)
    myMap.append(temp_row)


class Border(pygame.sprite.Sprite):

    def __init__(self, x, y, w, h):
        super().__init__()
        self.rect = pygame.Rect((x, y), (w, h))


UpperBorder = Border(0, -1, block_column * pixel_per_block, 1)
BottomBorder = Border(0, block_row * pixel_per_block,
                      block_column * pixel_per_block, 1)
LeftBorder = Border(-1, 0, 1, block_row * pixel_per_block)
RightBorder = Border(block_column * pixel_per_block,
                     0, 1, block_row * pixel_per_block)

mainMenu = True
MENU_FRAME_RATE = .1
currSprite = 0
howToPlay_counter = 0
BG = howToPlay_1
while mainMenu:
    currSprite += MENU_FRAME_RATE

    BG_sprites = [gameTitle1, gameTitle2]

    if currSprite >= len(BG_sprites):
        currSprite = 0

    screen.blit(menuBackground, (-480, -312))
    screen.blit(BG_sprites[int(currSprite)], (80, 100))
    pygame.display.update()

    for event in pygame.event.get():
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_a:
                mainMenu = False
                clickButtonSound.play()
            if event.key == pygame.K_h:
                howToPlay = True

                while howToPlay:
                    howToPlay_counter += 1
                    howToPlayBG = [howToPlay_1, howToPlay_2,
                                   howToPlay_3, howToPlay_4, howToPlay_5, howToPlay_6]
                    screen.fill(HOW_TO_PLAY_BG)

                    if howToPlay_counter > 9:
                        howToPlay_counter = 0
                        BG = random.choice(howToPlayBG)

                    screen.blit(BG,  (80, 100))
                    pygame.display.update()
                    for inner_event in pygame.event.get():
                        if inner_event.type == pygame.KEYDOWN:
                            howToPlay = False
                        if inner_event.type == pygame.QUIT:
                            pygame.quit()
                            sys.exit()

        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()


# Groups

potions_group = pygame.sprite.Group()
slime_group = pygame.sprite.Group()
block_group = pygame.sprite.Group()
fireball_group = pygame.sprite.Group()
player_group = pygame.sprite.Group()
effect_group = pygame.sprite.Group()
status_group = pygame.sprite.Group()
health_bar_group = pygame.sprite.Group()

# Classes

RED = (200, 0, 0)
BLUE = (0, 0, 200)
GOLD = (225, 60, 0)
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)


class StatusHealth(pygame.sprite.Sprite):
    def __init__(self, x, y):
        super().__init__()
        self.x = x
        self.y = y
        self.image = heartImg_temp
        self.rect = self.image.get_rect()
        self.rect.topleft = (self.x, self.y)


class StatusCoin(pygame.sprite.Sprite):
    def __init__(self, x, y):
        super().__init__()
        self.x = x
        self.y = y
        self.image = coinImg_temp
        self.rect = self.image.get_rect()
        self.rect.topleft = (self.x, self.y)


class StatusPotion(pygame.sprite.Sprite):

    FRAME_RATE = .25

    def __init__(self, x, y):
        super().__init__()
        self.x = x
        self.y = y
        self.image = potionImg1_org_temp
        self.rect = self.image.get_rect()
        self.rect.topleft = (self.x, self.y)


class Effect(pygame.sprite.Sprite):

    FONT_SIZE = 8
    DURATION = 25
    SPEED = 1.8

    def __init__(self, x, y, type):
        super().__init__()
        self.x = x * pixel_per_block
        self.y = y * pixel_per_block
        self.type = type
        self.timer = 0

        if self.type == 'coin':
            self.image = coinImg
        elif self.type == 'heart':
            self.image = heartImg
        elif self.type == 'mana':
            self.image = potionImg1_org
        elif self.type == 'broken_heart':
            self.image = broken_heartImg

        self.rect = self.image.get_rect()
        self.rect.topleft = (self.x, self.y)

    def update(self):
        self.y -= Effect.SPEED
        self.rect.topleft = (self.x, self.y)
        self.timer += 1
        buffer_surface.blit(self.image, self.rect.topleft)

        self.image.set_alpha(225 * (1 - self.timer / Effect.DURATION))

        if self.timer > Effect.DURATION:
            pygame.sprite.Sprite.remove(self, effect_group)


class Mouse(pygame.sprite.Sprite):
    def __init__(self, image_source):
        super().__init__()
        self.image = image_source
        self.rect = self.image.get_rect()

    def update(self):
        self.rect.center = pygame.mouse.get_pos()


class Block(pygame.sprite.Sprite):
    def __init__(self, type, x, y):
        super().__init__()
        self.type = type
        self.x = x
        self.y = y
        if type == TILE:
            self.image = tileImg
        self.rect = self.image.get_rect()
        self.rect.topleft = (self.x * pixel_per_block,
                             self.y * pixel_per_block)


def generatePotion():
    rand_x = random.randrange(0, block_column - 1)
    rand_y = random.randrange(0, block_row - 1)

    potions_group.add(Potion(rand_x, rand_y))


class Potion(pygame.sprite.Sprite):

    count = 0
    MAX_COUNT = 8
    FRAME_RATE = .25

    def __init__(self, x, y):
        Potion.count += 1
        super().__init__()
        self.x = x
        self.y = y
        self.sprites = []
        self.sprite_index = 0

        self.sprites.append(potionImg1)
        self.sprites.append(potionImg2)
        self.sprites.append(potionImg3)
        self.sprites.append(potionImg4)
        self.sprites.append(potionImg5)
        self.sprites.append(potionImg6)
        self.sprites.append(potionImg7)
        self.sprites.append(potionImg8)

        self.image = self.sprites[0]
        self.rect = self.image.get_rect()
        self.rect.topleft = (self.x * pixel_per_block,
                             self.y * pixel_per_block)

    def __del__(self):
        Potion.count -= 1

    def update(self):
        self.sprite_index += Potion.FRAME_RATE

        temp = int(self.sprite_index)

        self.image = self.sprites[temp % len(self.sprites)]

        if Potion.count < Potion.MAX_COUNT:
            generatePotion()


potions_group.add(Potion(3, 3))


class Player(pygame.sprite.Sprite):

    SPEED = .08
    MANA_PER_POTION = 1
    MANA_REGEN_PER_ITER = .01
    HEALTH_REGEN_PER_ITER = .01

    MANA_REGEN_PER_KILL = 2
    HEALTH_REGEN_PER_KILL = 1

    def __init__(self, x, y):
        super().__init__()
        self.image = playerImg
        self.x = x
        self.y = y

        self.health = 100
        self.coin = 0
        self.score = 0

        self.velocity_x = 0
        self.velocity_y = 0
        self.direction = 'left'

        self.mana = 0

        self.rect = self.image.get_rect()
        self.rect.topleft = (self.x * pixel_per_block,
                             self.y * pixel_per_block)

    def update(self):

        if self.health <= 0:
            pygame.sprite.Sprite.remove(self, player_group)
        elif self.health > 100:
            self.health = 100

        if self.velocity_y > 0:
            if self.y + self.velocity_y < block_row - .8:
                self.y += self.velocity_y
            else:
                self.velocity_y = -self.velocity_y * .1

        if self.velocity_x > 0:
            self.direction = 'right'
            if self.x + self.velocity_x < block_column - 1:
                self.x += self.velocity_x
            else:
                self.velocity_x = -self.velocity_x * .1

        if self.velocity_y < 0:
            if self.y + self.velocity_y > 0:
                self.y += self.velocity_y
            else:
                self.velocity_y = -self.velocity_y * .1

        if self.velocity_x < 0:
            self.direction = 'left'
            if self.x + self.velocity_x > 0:
                self.x += self.velocity_x
            else:
                self.velocity_x = -self.velocity_x * .1

        if self.direction == 'right':
            self.image = pygame.transform.flip(playerImg, True, False)
        else:
            self.image = playerImg

        self.rect.topleft = (self.x * pixel_per_block,
                             self.y * pixel_per_block)

        if pygame.sprite.spritecollide(self, potions_group, True):
            self.mana += Player.MANA_PER_POTION
            effect_group.add(Effect(
                player.x, player.y, 'mana'))
            pickPotionSound.play()


player = Player(7, 4)  # Put Player in the Middle of the Screen
player_group.add(player)


class Fireball(pygame.sprite.Sprite):

    SPEED = .2

    def __init__(self, start_x, start_y, end_x, end_y):
        super().__init__()
        self.x = start_x
        self.y = start_y

        self.start_x = start_x
        self.start_y = start_y

        self.end_x = end_x / pixel_per_block
        self.end_y = end_y / pixel_per_block

        self.image = fireballImg
        self.rect = self.image.get_rect()

        self.rect.center = (self.x * pixel_per_block, self.y * pixel_per_block)

    @staticmethod
    def generateFireball():
        mouse_x, mouse_y = pygame.mouse.get_pos()
        start_x = player.x + .5
        start_y = player.y + .5
        deltaX = mouse_x - start_x
        deltaY = mouse_y - start_y
        base = (deltaX ** 2 + deltaY ** 2) ** .5

        sin = -deltaY / base

        degree = ((math.asin(sin)) / math.pi) * 180 - 90

        if deltaX == 0 and deltaY == 0:
            return

        new_fireball = Fireball(start_x, start_y, mouse_x, mouse_y)
        new_fireball.image = pygame.transform.rotate(fireballImg, degree)

        fireball_group.add(new_fireball)

    def update(self):

        deltaX = self.end_x - self.start_x
        deltaY = self.end_y - self.start_y

        base = (deltaX ** 2 + deltaY ** 2) ** .5

        velocity_x = (deltaX / base) * Fireball.SPEED
        velocity_y = (deltaY / base) * Fireball.SPEED

        self.x += velocity_x
        self.y += velocity_y

        self.rect.center = (self.x * pixel_per_block,
                            self.y * pixel_per_block)


class Slime(pygame.sprite.Sprite):

    SPEED = .018
    DAMAGE_PER_COLLISION = 10
    COIN_PER_KILL = 1
    COUNT = 3
    SCORE_PER_KILL = 3

    def __init__(self, x, y):
        super().__init__()
        self.x = x
        self.y = y

        self.direction = 'left'
        self.shot = False
        self.collided = False

        self.image = slimeImg
        self.rect = self.image.get_rect()
        self.rect.center = (self.x * pixel_per_block, self.y * pixel_per_block)

    def update(self):

        if pygame.sprite.spritecollide(self, fireball_group, True):
            self.collided = True
            self.shot = True

        if self.collided:
            pygame.sprite.Sprite.remove(self, slime_group)
            if self.shot:
                player.coin += 1
                player.score += Slime.SCORE_PER_KILL
                effect_group.add(Effect(
                    player.x - .5, player.y, 'coin'))
                player.health += Player.HEALTH_REGEN_PER_KILL
                effect_group.add(Effect(
                    player.x, player.y, 'heart'))
                player.mana += Player.MANA_REGEN_PER_KILL
                effect_group.add(Effect(
                    player.x + .5, player.y, 'mana'))
            slimeDeath.play()
            Slime.generateSlime()

        deltaX = (player.x + .5) - self.x
        deltaY = (player.y + .5) - self.y

        base = (deltaX ** 2 + deltaY ** 2) ** .5

        velocity_x = (deltaX / base) * Slime.SPEED
        velocity_y = (deltaY / base) * Slime.SPEED

        if velocity_x > 0:
            self.direction = 'right'
        elif velocity_x < 0:
            self.direction = 'left'

        if self.direction == 'right':
            self.image = pygame.transform.flip(slimeImg, True, False)
        else:
            self.image = slimeImg

        self.x += velocity_x
        self.y += velocity_y

        self.rect.center = (self.x * pixel_per_block, self.y * pixel_per_block)

        if self.rect.colliderect(player.rect):
            player.health -= Slime.DAMAGE_PER_COLLISION / 2
            effect_group.add(Effect(
                player.x, player.y, 'broken_heart'))
            self.collided = True

    @staticmethod
    def generateSlime():
        rand_x = random.randrange(0, block_column)
        rand_y = random.randrange(0, block_row)

        slime_group.add(Slime(rand_x, rand_y))


class PlayerHealthBar(pygame.sprite.Sprite):

    def __init__(self):
        super().__init__()
        self.x = (player.x + .1) * pixel_per_block
        self.y = (player.y - .25) * pixel_per_block

        self.image = healthBar_1000
        self.rect = self.image.get_rect()
        self.rect.topleft = (self.x, self.y)

    def update(self):
        if player.health >= 100:
            self.image = healthBar_1000
        elif player.health >= 97.5:
            self.image = healthBar_975
        elif player.health >= 95:
            self.image = healthBar_950
        elif player.health >= 92.5:
            self.image = healthBar_925
        elif player.health >= 90:
            self.image = healthBar_900
        elif player.health >= 87.5:
            self.image = healthBar_875
        elif player.health >= 85:
            self.image = healthBar_850
        elif player.health >= 82.5:
            self.image = healthBar_825
        elif player.health >= 80:
            self.image = healthBar_800
        elif player.health >= 77.5:
            self.image = healthBar_775
        elif player.health >= 75:
            self.image = healthBar_750
        elif player.health >= 72.5:
            self.image = healthBar_725
        elif player.health >= 70:
            self.image = healthBar_700
        elif player.health >= 67.5:
            self.image = healthBar_675
        elif player.health >= 65:
            self.image = healthBar_650
        elif player.health >= 62.5:
            self.image = healthBar_625
        elif player.health >= 60:
            self.image = healthBar_600
        elif player.health >= 57.5:
            self.image = healthBar_575
        elif player.health >= 55:
            self.image = healthBar_550
        elif player.health >= 52.5:
            self.image = healthBar_525
        elif player.health >= 50:
            self.image = healthBar_500
        elif player.health >= 47.5:
            self.image = healthBar_475
        elif player.health >= 45:
            self.image = healthBar_450
        elif player.health >= 42.5:
            self.image = healthBar_425
        elif player.health >= 40:
            self.image = healthBar_400
        elif player.health >= 37.5:
            self.image = healthBar_375
        elif player.health >= 35:
            self.image = healthBar_350
        elif player.health >= 32.5:
            self.image = healthBar_325
        elif player.health >= 30:
            self.image = healthBar_300
        elif player.health >= 27.5:
            self.image = healthBar_275
        elif player.health >= 25:
            self.image = healthBar_250
        elif player.health >= 22.5:
            self.image = healthBar_225
        elif player.health >= 20:
            self.image = healthBar_200
        elif player.health >= 17.5:
            self.image = healthBar_175
        elif player.health >= 15:
            self.image = healthBar_150
        elif player.health >= 12.5:
            self.image = healthBar_125
        elif player.health >= 10:
            self.image = healthBar_100
        elif player.health >= 7.5:
            self.image = healthBar_75
        elif player.health >= 5:
            self.image = healthBar_50
        elif player.health >= 2.5:
            self.image = healthBar_25
        elif player.health >= 0:
            self.image = healthBar_0

        self.x = (player.x + .1) * pixel_per_block
        self.y = (player.y - .25) * pixel_per_block
        self.rect.topleft = (self.x, self.y)


# Instances
mouse = Mouse(aimImg)
aim = pygame.sprite.Group()
aim.add(mouse)

playerHealthBar = PlayerHealthBar()
health_bar_group.add(playerHealthBar)

for iter in range(Slime.COUNT):
    Slime.generateSlime()

# status_background = StatusBackground((block_column - 2) *
#                  pixel_per_block, (block_row - 2) * pixel_per_block)

status_group.add(StatusPotion(textX, textY + fontSize * 1.5))
status_group.add(StatusCoin(textX, textY))
# status_group.add(StatusHealth(textX, textY + fontSize * 3))

for y in range(len(myMap)):
    for x in range(len(myMap[y])):
        if myMap[y][x] == TILE:
            tile = Block(TILE, x, y)
            block_group.add(tile)
# GameLoop


gameRunning = True
level = 1

counter = 0


mob_cnt = level * 3


block_group.draw(screen)
pygame.display.update()


def eliminateFireball():
    pygame.sprite.spritecollide(UpperBorder, fireball_group, True)
    pygame.sprite.spritecollide(BottomBorder, fireball_group, True)
    pygame.sprite.spritecollide(LeftBorder, fireball_group, True)
    pygame.sprite.spritecollide(RightBorder, fireball_group, True)


finalScore = 0
while gameRunning:

    level = int(counter / 60 + 1)
    mob_cnt = level * 3
    Slime.SPEED = .018 * (1 + (level / 10) * 3)

    if Slime.COUNT < mob_cnt:
        for iter in range(mob_cnt - Slime.COUNT):
            Slime.generateSlime()
        Slime.COUNT = mob_cnt

    if player.health <= 0:
        gameRunning = False
    # Detecting Events
    for event in pygame.event.get():

        if event.type == pygame.QUIT:
            gameRunning = False
            pygame.quit()
            sys.exit()
            break

        if event.type == pygame.USEREVENT:
            counter += 1
            player.score += 1

        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_w:
                player.velocity_y = -Player.SPEED
            if event.key == pygame.K_s:
                player.velocity_y = Player.SPEED
            if event.key == pygame.K_a:
                player.velocity_x = -Player.SPEED
            if event.key == pygame.K_d:
                player.velocity_x = Player.SPEED

        if event.type == pygame.KEYUP:
            if event.key == pygame.K_w:
                player.velocity_y = 0
            if event.key == pygame.K_s:
                player.velocity_y = 0
            if event.key == pygame.K_a:
                player.velocity_x = 0
            if event.key == pygame.K_d:
                player.velocity_x = 0

        if event.type == pygame.MOUSEBUTTONDOWN:
            if player.mana > 0:
                player.mana -= 1
                shootFireball.play()
                Fireball.generateFireball()

    # Contents
    player.mana += Player.MANA_REGEN_PER_ITER
    player.health += Player.HEALTH_REGEN_PER_ITER

    # Regular Update

    aim.update()
    player_group.update()
    potions_group.update()
    fireball_group.update()
    slime_group.update()
    status_group.update()
    health_bar_group.update()
    eliminateFireball()

    player_score = font.render(
        "       " + str(player.coin), True, GOLD)
    mana_point = font.render(
        "       " + str(int(player.mana)), True, BLUE)
    player_health = font.render(
        "       " + str(int(player.health)), True, RED)
    game_level = font.render(
        "Level " + str(level), True, WHITE)
    time_passed = font.render(
        "Time  " + str(counter), True, WHITE)
    score_board = font.render(
        "Score " + str(player.score), True, WHITE)

    block_group.draw(buffer_surface)
    potions_group.draw(buffer_surface)
    slime_group.draw(buffer_surface)
    player_group.draw(buffer_surface)
    effect_group.update()
    fireball_group.draw(buffer_surface)
    health_bar_group.draw(buffer_surface)
    buffer_surface.blit(statusBackground, ((block_column - 2) *
                                           pixel_per_block, (block_row - 2) * pixel_per_block))
    status_group.draw(buffer_surface)
    buffer_surface.blit(player_score, (textX, textY))
    buffer_surface.blit(mana_point, (textX, textY + fontSize * 1.5))
    # buffer_surface.blit(player_health, (textX, textY + fontSize * 3))
    buffer_surface.blit(game_level, (200, 10))
    buffer_surface.blit(time_passed, (300, 10))
    buffer_surface.blit(score_board, (400, 10))
    aim.draw(buffer_surface)

    screen.blit(buffer_surface, (0, 0))

    pygame.display.update()
    clock.tick(100)

endGame = True

screen.blit(menuBackground, (-480, -312))
screen.blit(endGameTitle, (80, 100))


myFile = open("saves/highestScore.txt", 'r+')

higestScore = int(myFile.read())

if player.score > higestScore:
    myFile.write(str(player.score))
    higestScore = player.score

myFile.close()

score_board = font.render(
    "Final Score " + str(player.score), True, RED)


while endGame:
    rand_color = (random.randrange(0, 255), random.randrange(
        0, 255), random.randrange(0, 255))
    highest_score = font.render(
        "Highest Score " + str(player.score), True, rand_color)
    screen.blit(score_board, (400, 300))
    screen.blit(highest_score, (400, 350))

    pygame.display.update()
    for event in pygame.event.get():
        if event.type == pygame.KEYDOWN:
            endGame = False
            clickButtonSound.play()
            sys.exit()
