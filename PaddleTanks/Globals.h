#pragma once
//Allows access to math defines like PI.
#define _USE_MATH_DEFINES
#include <cmath>

//Conversion factor for going from SFML to Box2D units and back.
float const METERS_PER_PIXEL = 1.0f / 10.0f; //1m = 10px
float const PIXELS_PER_METERS = 1.0f / METERS_PER_PIXEL; //1m = 10px

//Conversion factor for going from degrees (used by SFML) to radians (used by everything else)
float const RADIANS_PER_DEGREES = (float)(M_PI / 180.0);
float const DEGREES_PER_RADIANS = 1.0f / RADIANS_PER_DEGREES;

float const PADDLE_TANK_DENSITY = 100000.0f;
float const PADDLE_TANK_IMPULSE_POWER = 10000000.0f;

float const MIN_TIME_BETWEEN_SHOTS = 0.07f;
float const MAGAZINE_RELOAD_TIME = 1.0f;
float const BOUNDARY_WALLS_HEIGHT = 2.0f; //in meters

float const HEALTH_REGEN_RATE = 10.0f / 60.0f; // 10 hp per 60 frames (1 second), 100 hp in 10 seconds
float const POWER_REGEN_RATE = 20.0f / 60.0f; // 20 power per 60 frames (1 second), 100 power in 5 seconds

float const HEALTH_DAMAGE_HIT = 10.0f; // 10 health is subtracted per hit received
float const POWER_DRAIN_SHOT = 1.0f; // 1 power is subtracted per shot fired