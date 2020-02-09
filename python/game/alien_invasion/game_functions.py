import sys
import pygame
from bullet import Bullet
from alien import Alien

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

def update_screen(ai_settings, screen, ship, bullets, aliens):
    screen.fill(ai_settings.bg_color)
    ship.update()
    ship.blitme()
    #alien.blitme()
    aliens.draw(screen) # display all the elements of group in the screen

    for bullet in bullets.sprites():
        bullet.update()
        bullet.draw_bullet()
    pygame.display.flip()

def update_bullets(aliens, bullets):
    bullets.update()
    collisions = pygame.sprite.groupcollide(bullets, aliens, True, True)
    #collisions = pygame.sprite.spritecollide(alien, bullets, True)


def create_fleet(ai_settings, screen, aliens):
    alien = Alien(ai_settings, screen)
    alien_width = alien.rect.width
    available_space_x = ai_settings.screen_width - 2 * alien_width
    number_alien_x = int(available_space_x/(2 * alien_width))
    for alien_num in range(number_alien_x):
        alien = Alien(ai_settings, screen)
        alien.x = alien_width + 2 * alien_width * alien_num
        alien.rect.x = alien.x
        aliens.add(alien)

def change_fleet_direction(ai_settings, aliens):
    for alien in aliens.sprites():
        alien.rect.y += ai_settings.fleet_drop_speed
    ai_settings.fleet_direction *= -1

def check_fleet_edges(ai_settings, aliens):
    for alien in aliens.sprites():
        if alien.check_edges():
            change_fleet_direction(ai_settings, aliens)
            break

def update_aliens(ai_settings, aliens):
    check_fleet_edges(ai_settings, aliens)
    aliens.update()






