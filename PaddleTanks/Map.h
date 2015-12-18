#pragma once

#include "Entities/BaseGameEntity.h"

#include <SFML/Graphics/Drawable.hpp>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include "TankStatsGameEntity.h"
#include <vector>
#include <list>

class TiledBackground;
class Box2DGameEntity;
class TextureAtlas;
class MapContactListener;

class b2World;
class b2Body;

/// <summary>
/// A Map is a heavy-weight entity that includes all the information needed to load and play a level.
/// A map is loaded from a properly formated XML file. The map is also a container for the world and
/// the entities that live in it. The map uses Box2D for the world.
/// Note that just instantiating a map is not enough to make it usable - to be properly initialised 
/// a map must be loaded via loadFromFile().
/// A map should be updated each frame via Update(), which will also update the b2World and the entities
/// within the world.
/// A map is also a BaseGameEntity, which allows it to send and receive messages.
/// Drawing a map also draws all of the entities it contains.
/// </summary>
///
/// <file-format>
/*
	<Map width="d" height="e">
		<TextureAtlas file="/path/to/atlas/file.xml" />
		<Background name="spriteName" />
		<Obstacles>
			<Rectangle name="spriteName" x="i" y="j" a="k" /> ...
			<Circle name="spriteName" x="i" y="j" a="k" /> ...
		</Obstacles>
		<SpawnPoints>
			<Spawn x="i" y="j" a="k" type="human|simpleai|otheraitype" tankSprite="spriteName" tankBarrelSprite="spriteName" /> ...
		</SpawnPoints>
	</Map>

	where spriteName is the name of a sprite within the texture atlas,
	d and e are integer values describing the width and height of the map in pixels,
	and i, j, and k are integer values describing the centre position and rotation
	about the center for the obstacle or spawn point.
*/
/// </file-format>
///
/// <author>
/// Kevin Forest 2015
/// </author>
class Map :
	public BaseGameEntity,
	public sf::Drawable
{
public:	
	/// <summary>
	/// Initializes a new instance of the <see cref="Map"/> class. Note that the map is not usable
	/// until you load a map .xml file.
	/// </summary>
	Map();
	virtual ~Map();
	
	/// <summary>
	/// Loads a map from file. The file should be a properly formatted XML file. The loading process 
	/// also handles creation of entities and the b2World. Reloading a different map via the same
	/// map object is not recommended - instead you should instantiate a new map object.
	/// </summary>
	/// <param name="filename">The filename (including the path) to the level you wish to load.</param>
	/// <returns>true if the load succeeded, false otherwise. </returns>
	bool loadFromFile(std::string const &filename);

	//Update the map, including the physics simulation.
	virtual void Update(float delta);
	//Handle a message sent to this entity.  Will only work if you register the entity with the entity manager.
	virtual bool HandleMessage(Telegram const &msg);
	
protected:
	//Draw the map, including all the entities it contains.
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	//The world and it's boundaries.
	b2World *m_world;
	//The first two spots should have the left and right scoring boundaries.
	// Note that the user data on these spots should point to the score
	// entities. This is probably a little silly - I think you can do better.
	std::vector<b2Body *> m_boundaries;
	
	std::vector<Box2DGameEntity *> m_obstacles;
	std::vector<Box2DGameEntity *> m_tanks;
	std::list<Box2DGameEntity *> m_bullets;
	std::vector<Box2DGameEntity *> m_powerups;
	std::vector<BaseGameEntity *> m_stats;

	TankStatsGameEntity *rtsge;
	TankStatsGameEntity *ltsge;

	TiledBackground *m_background;
	TextureAtlas *m_ta;
	//The contact listener for the map.
	MapContactListener *m_contactListener;
};

//Handles contacts in the box2d world.
class MapContactListener : public b2ContactListener {
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
};

