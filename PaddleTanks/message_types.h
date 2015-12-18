#pragma once
namespace message_type {
	enum {
		ERROR = -1,
		NONE = 0,
		SPAWN_BULLET = 1, //ExtraInfo contains (SpawnBulletData *)
		BULLETREADY = 2,
		RELOAD = 3,
		TOIDLE = 4,
		TOACTIVE = 5,
		HIT = 6,
	};
};

struct SpawnBulletData {
	sf::Vector2f Position;
	sf::Vector2f Direction;
};