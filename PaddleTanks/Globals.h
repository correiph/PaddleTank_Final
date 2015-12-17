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
float const PADDLE_TANK_IMPULSE_POWER = 1000000.0f;

float const MIN_TIME_BETWEEN_SHOTS = 0.1f;
float const MAGAZINE_RELOAD_TIME = 1.0f;
float const BOUNDARY_WALLS_HEIGHT = 2.0f; //in meters
