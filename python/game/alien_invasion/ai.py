#!/usr/bin/env python3
import sys
import pygame
from settings import Settings
from ship import Ship
from bullet import Bullet
import game_functions as gf
from pygame.sprite import Group

def run_game():
    pygame.init()
    ai_settings = Settings()
    screen = pygame.display.set_mode((ai_settings.screen_width,ai_settings.screen_height))
    pygame.display.set_caption("alien invasion")
    ship = Ship(screen)
    bullets = Group()
    while 1:
        gf.check_events(ai_settings, screen, ship, bullets)
        gf.update_screen(ai_settings, screen, ship, bullets)

run_game()



