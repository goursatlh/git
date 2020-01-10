import sys
import pygame
from bullet import Bullet

SHIFT_OFFSET = 10
def check_events(ai_settings, screen, ship, bullets):
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_RIGHT:
                ship.moving_right = True
            elif event.key == pygame.K_LEFT:
                ship.moving_left = True
            elif event.key == pygame.K_SPACE:
                new_bullet = Bullet(ai_settings, screen, ship)
                bullets.add(new_bullet)
            elif event.key == pygame.K_q:
                # push q key will exit from the game
                sys.exit()
        elif event.type == pygame.KEYUP:
            if event.key == pygame.K_RIGHT:
                ship.moving_right = False
            elif event.key == pygame.K_LEFT:
                ship.moving_left = False

def update_screen(ai_settings, screen, ship, alien, bullets):
    screen.fill(ai_settings.bg_color)
    ship.update()
    ship.blitme()
    alien.blitme()
    for bullet in bullets.sprites():
        bullet.update()
        bullet.draw_bullet()
    pygame.display.flip()

def update_bullets(aliens, bullets):
    collisions = pygame.sprite.groupcollide(bullets, aliens, True, True)

