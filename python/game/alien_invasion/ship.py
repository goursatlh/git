import pygame
SHIFT_OFFSET = 10

class Ship():
    def __init__(self, screen):
        self.screen = screen
        self.image = pygame.image.load('images/ship.bmp')
        self.rect = self.image.get_rect()
        self.screen_rect = self.screen.get_rect()

        self.rect.centerx = self.screen_rect.centerx
        self.rect.bottom = self.screen_rect.bottom

        self.moving_right = False
        self.moving_left = False

    def update(self):  # why add self input parameter
        if self.moving_right:
            self.rect.centerx += SHIFT_OFFSET
        elif self.moving_left:
            self.rect.centerx -= SHIFT_OFFSET

    def blitme(self):
        self.screen.blit(self.image, self.rect)
