#!/usr/bin/env python3
import sys
import pygame
from settings import Settings
from ship import Ship
from pygame.sprite import Group
import game_functions as gf

def run_game():
    pygame.init()
    ai_settings = Settings()
    screen = pygame.display.set_mode((ai_settings.screen_width, ai_settings.screen_height))
    ship = Ship(screen)
    bullets = Group()
    aliens = Group()
    gf.create_fleet(ai_settings, screen, aliens)
    pygame.display.set_caption("alien invasion")
    while 1:
        gf.check_events(ai_settings, screen, ship, bullets)
        gf.update_screen(ai_settings, screen, ship, bullets, aliens)
        gf.update_bullets(aliens, bullets)
        gf.update_aliens(ai_settings, aliens)
run_game()



