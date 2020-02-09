class Settings():
    def __init__(self):
        self.screen_width = 600
        self.screen_height = 400
        self.bg_color = (230,230,230)
        self.bullet_speed_factor = 50
        self.bullet_width = 3
        self.bullet_height = 15
        self.bullet_color = 60,60,60

        # settings for the aliens
        self.alien_speed_factor = 10
        self.fleet_drop_speed = 10
        # move direction of fleet, 1 right, -1 left
        self.fleet_direction = 1 

