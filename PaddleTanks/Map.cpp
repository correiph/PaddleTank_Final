#include "Globals.h"

#include "Map.h"

#include "message_types.h"
#include "vector2_math_utils.h"
#include "TiledBackground.h"
#include "Box2DGameEntity.h"
#include "TextureAtlas.h"
#include "AssetManager.h"
#include "Entities/EntityManager.h"
#include "PaddleTankGameEntity.h"
#include "Messaging/Telegram.h"
#include "PaddleTankEntityStates.h"
#include "PowerUpEntity.h"
#include "ScoreGameEntity.h"
#include "tinyxml/tinyxml2.h"

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

Map::Map() 
	: BaseGameEntity(BaseGameEntity::GetNextValidID()), m_background(nullptr), m_world(nullptr), m_ta(nullptr), m_contactListener(nullptr)
{
	//Register itself so it can send messages.
	EntityMgr->RegisterEntity(this);
}

Map::~Map()
{
	//Clean-up

	//Remove from the entity manager
	EntityMgr->RemoveEntity(this);

	//Clean up all the objects
	delete m_background;
	m_background = nullptr;

	//Clean up the contact listener.
	m_world->SetContactListener(nullptr);
	delete m_contactListener;
	m_contactListener = nullptr;

	//Obstacles and tanks get deleted outright.
	for (std::size_t i = 0; i < m_obstacles.size(); i++) {

		delete m_obstacles[i];
		m_obstacles[i] = nullptr;
	}
	m_obstacles.clear();

	for (std::size_t i = 0; i < m_tanks.size(); i++) {
		delete m_tanks[i];
		m_tanks[i] = nullptr;
	}
	m_tanks.clear();

	//The boundary walls get deleted from the world.
	for (std::size_t i = 0; i < m_boundaries.size(); i++) {
		//Don't forget the silly rule that the first two boundaries are the
		// scoring ones which contain a reference to the score entities.
		if (i < 2) {
			delete (ScoreGameEntity *)m_boundaries[i]->GetUserData();
		}
		m_world->DestroyBody(m_boundaries[i]);
		m_boundaries[i] = nullptr;
	}
	m_boundaries.clear();

	//Clean up the bullets from the bullet list.
	for (std::list<Box2DGameEntity *>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++) {
		delete *it;
		*it = nullptr;
	}
	m_bullets.clear();

	//Better make sure everything else is deleted before you do this!
	// We might want to add some checks to make sure the world is really empty.
	delete m_world;
	m_world = nullptr;
}


bool Map::loadFromFile(std::string const &filename) {
	using namespace tinyxml2;
	XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != XML_NO_ERROR) {
		//Error, cannot load file.
		return false;
	}
	//Create a world and assign a contact listener to it.
	m_world = new b2World(b2Vec2_zero);
	m_contactListener = new MapContactListener();
	m_world->SetContactListener(m_contactListener);

	//Read the map
	XMLElement *mapEl = doc.FirstChildElement("Map");
	//Remember that all units are in pixels here, but we need to convert
	// to box2d meters at some point.
	int mapWidthPx = mapEl->IntAttribute("width");
	int mapHeightPx = mapEl->IntAttribute("height");
	float mapWidthMet = mapWidthPx * METERS_PER_PIXEL;
	float mapHeightMet = mapHeightPx * METERS_PER_PIXEL;

	//Create the scoring zones and the score entities.
	{
		b2BodyDef ldef;
		ldef.position.Set(BOUNDARY_WALLS_HEIGHT * 0.5f, mapHeightMet * 0.5f);
		ldef.type = b2_staticBody;
		b2Body *lbod = m_world->CreateBody(&ldef);
		b2PolygonShape shp;
		shp.SetAsBox(BOUNDARY_WALLS_HEIGHT * 0.5f, mapHeightMet * 0.5f);
		b2FixtureDef fdef;
		fdef.shape = &shp;
		fdef.isSensor = true;
		lbod->CreateFixture(&fdef);
		//The right scoring entity gets associated with the left scoring wall.
		ScoreGameEntity *rge = new ScoreGameEntity(sf::Vector2f(mapWidthPx * 0.5f + 50, 25), sf::Color::Blue);
		lbod->SetUserData(rge);
		m_boundaries.push_back(lbod);

		ldef.position.x = mapWidthMet - BOUNDARY_WALLS_HEIGHT * 0.5f;
		b2Body *rbod = m_world->CreateBody(&ldef);
		rbod->CreateFixture(&fdef);
		//The left scoring entity gets associated with the right scoring wall.
		ScoreGameEntity *lge = new ScoreGameEntity(sf::Vector2f(mapWidthPx * 0.5f - 50, 25), sf::Color::Red);
		rbod->SetUserData(lge);
		m_boundaries.push_back(rbod);
	}
	//create stats display on bottom
	{
		b2BodyDef sdef;
		sdef.position.Set(0.5f, 0.5f);
		sdef.type = b2_staticBody;
		b2Body *sbod = m_world->CreateBody(&sdef);
		b2PolygonShape sshp;
		sshp.SetAsBox(1.0f, 1.0f);
		b2FixtureDef sfdef;
		sfdef.shape = &sshp;
		sfdef.isSensor = true;
		sbod->CreateFixture(&sfdef);

		//The right scoring entity gets associated with the left scoring wall.
		TankStatsGameEntity *rtsge = new TankStatsGameEntity(sf::Vector2f(mapWidthPx * 0.5f + 220, 600 * 0.87), sf::Color::Blue);
		sbod->SetUserData(rtsge);
		rtsge->CurrentHealthPower();
		m_stats.push_back(rtsge);

		sdef.position.x = 1.0f;
		b2Body *s2bod = m_world->CreateBody(&sdef);
		s2bod->CreateFixture(&sfdef);

		//The left scoring entity gets associated with the right scoring wall.
		TankStatsGameEntity *ltsge = new TankStatsGameEntity(sf::Vector2f(mapWidthPx * 0.5f - 380, mapHeightPx * 0.87), sf::Color::Red);
		s2bod->SetUserData(ltsge);
		ltsge->CurrentHealthPower();
		m_stats.push_back(ltsge);
	}
	//Create the boundary walls.
	{
		b2BodyDef tdef;
		tdef.position.Set(mapWidthMet * 0.5f, -BOUNDARY_WALLS_HEIGHT * 0.5f);
		tdef.type = b2_staticBody;
		b2Body *tbod = m_world->CreateBody(&tdef);
		b2PolygonShape tshp;
		tshp.SetAsBox(mapWidthMet * 0.5f, BOUNDARY_WALLS_HEIGHT * 0.5f);
		tbod->CreateFixture(&tshp, 1.0f);
		m_boundaries.push_back(tbod);
		tdef.position.y = mapHeightMet + (BOUNDARY_WALLS_HEIGHT * 0.5f);
		tbod = m_world->CreateBody(&tdef);
		tbod->CreateFixture(&tshp, 1.0f);
		m_boundaries.push_back(tbod);
	}
	//Load the tile atlas.
	XMLElement *atlasEl = mapEl->FirstChildElement("TextureAtlas");
	G_AtlasManager.LoadAsset(atlasEl->Attribute("file"));
	m_ta = G_AtlasManager.GetAsset(atlasEl->Attribute("file"));

	//Load the background element and create the tiled background area.
	XMLElement *bgEl = mapEl->FirstChildElement("Background");
	
	//Here you need to create a background texture array.
	m_background = new TiledBackground(*m_ta->GetTexture(),
		sf::IntRect(0, 0, mapWidthPx, mapHeightPx), 
		m_ta->GetSprite(bgEl->Attribute("name"))->getTextureRect()
	);

	//Iterate through all obstacles in the map file.
	XMLElement *obstaclesEl = mapEl->FirstChildElement("Obstacles");
	XMLElement *obEl = obstaclesEl->FirstChildElement(); //Obstacles can have a variety of names
	while (obEl != nullptr) {
		//Create the obstacle and add it to the map.
		sf::Vector2f posPx;
		posPx.x = obEl->FloatAttribute("x");
		posPx.y = obEl->FloatAttribute("y");
		float angleDeg = obEl->FloatAttribute("a");
		b2BodyDef obBodDef;
		obBodDef.angle = angleDeg * RADIANS_PER_DEGREES;
		obBodDef.position = vec2utils::ConvertVectorType<sf::Vector2f, b2Vec2>(posPx * METERS_PER_PIXEL);
		obBodDef.type = b2_staticBody;
		b2Body *obBod = m_world->CreateBody(&obBodDef);
		Box2DGameEntity::FixtureType fixType = Box2DGameEntity::NONE;
		//Check the shape and make sure the fixture is spawned correctly.
		if (std::string(obEl->Name()) == "Rectangle") {
			fixType = Box2DGameEntity::RECT;
		}
		else if (std::string(obEl->Name()) == "Circle") {
			fixType = Box2DGameEntity::CIRCLE;
		}
		Box2DGameEntity *ge = new Box2DGameEntity(*obBod, *m_ta, obEl->Attribute("name"), fixType, 1.0f, 1.0f);
		m_obstacles.push_back(ge);
		//Don't forget to get the next element.
		obEl = obEl->NextSiblingElement();
	}
	
	//Iterate through all spawn points in the file and create a tank.
	XMLElement *spawnPointsEl = mapEl->FirstChildElement("SpawnPoints");
	XMLElement *spawnEl = spawnPointsEl->FirstChildElement("Spawn");
	//while (spawnEl != nullptr)
	{
		//Spawn the first paddle tank here.
		sf::Vector2f spawnPos;
		spawnPos.x = spawnEl->FloatAttribute("x");
		spawnPos.y = spawnEl->FloatAttribute("y");
		float angleDeg = spawnEl->FloatAttribute("a");
		std::string tankSprite(spawnEl->Attribute("tankSprite"));
		std::string barrelSprite(spawnEl->Attribute("tankBarrelSprite"));
		b2BodyDef tankBd;
		tankBd.angle = angleDeg * RADIANS_PER_DEGREES;
		tankBd.position = vec2utils::ConvertVectorType<sf::Vector2f, b2Vec2>(METERS_PER_PIXEL * spawnPos);
		tankBd.type = b2_dynamicBody;
		b2Body *tankBodOne = m_world->CreateBody(&tankBd);
		//Check what kind of type of tank it is suppose to be, such as an AI controlled one or a user controlled one.
		std::string type(spawnEl->Attribute("type"));
		EntityState<PaddleTankGameEntity> *estateOne = nullptr;
		if (type == "human") {
			estateOne = PaddleTankHumanControlledEntityState::Instance();
		}
		else if (type == "simpleai") {
			estateOne = PaddleTankAIControlledEntityState::Instance();
		}

		//Create tank one.
		PaddleTankGameEntity *tankOne = new PaddleTankGameEntity(*tankBodOne, *m_ta, tankSprite, barrelSprite, PADDLE_TANK_DENSITY, estateOne);
		tankOne->SetMapEntityID(this->ID());
		m_tanks.push_back(tankOne);
		//Don't forget to get the next element.
		spawnEl = spawnEl->NextSiblingElement("Spawn");

		//Spawn the second paddle tank here.
		//sf::Vector2f spawnPos;
		spawnPos.x = spawnEl->FloatAttribute("x");
		spawnPos.y = spawnEl->FloatAttribute("y");
		angleDeg = spawnEl->FloatAttribute("a");
		tankSprite = spawnEl->Attribute("tankSprite");
		barrelSprite = spawnEl->Attribute("tankBarrelSprite");
		//b2BodyDef tankBd;
		tankBd.angle = angleDeg * RADIANS_PER_DEGREES;
		tankBd.position = vec2utils::ConvertVectorType<sf::Vector2f, b2Vec2>(METERS_PER_PIXEL * spawnPos);
		tankBd.type = b2_dynamicBody;
		b2Body *tankBodTwo = m_world->CreateBody(&tankBd);
		//Check what kind of type of tank it is suppose to be, such as an AI controlled one or a user controlled one.
		type = spawnEl->Attribute("type");
		EntityState<PaddleTankGameEntity> *estateTwo = nullptr;
		if (type == "human") {
			estateTwo = PaddleTankHumanControlledEntityState::Instance();
		}
		else if (type == "simpleai") {
			estateTwo = PaddleTankAIControlledEntityState::Instance();
		}

		//Create the tank.
		PaddleTankGameEntity *tankTwo = new PaddleTankGameEntity(*tankBodTwo, *m_ta, tankSprite, barrelSprite, PADDLE_TANK_DENSITY, estateTwo);
		tankTwo->SetMapEntityID(this->ID());
		m_tanks.push_back(tankTwo);
		//Don't forget to get the next element.
		spawnEl = spawnEl->NextSiblingElement("Spawn");

		if (estateOne == PaddleTankHumanControlledEntityState::Instance())
		{
			std::vector<void *> *userdata = &std::vector<void *>();
			userdata->push_back(tankTwo);
			tankBodOne->SetUserData(userdata);
			tankBodTwo->SetUserData(new std::vector<void *>);
		}
		else
		{
			std::vector<void *> *userdata = &std::vector<void *>();
			userdata->push_back(tankTwo);
			tankBodTwo->SetUserData(userdata);
			tankBodOne->SetUserData(new std::vector<void *>);
		}
	}
	return true;
}

//When you update the map you are also updating the Box2D world.
void Map::Update(float delta) {
	//Step the simulation
	m_world->Step(delta, 6, 2);
	//Update the tanks.
	for (std::size_t i = 0; i < m_tanks.size(); i++) {
		/*if (m_tanks[i]->IsTagged())
		{
			delete m_tanks[i];
			m_tanks.erase(std::remove(m_tanks.begin(), m_tanks.end(), m_tanks[i]), m_tanks.end());
		}
		else*/
			m_tanks[i]->Update(delta);
	}
	//Update the bullets unless they have been tagged, in which case
	// they get destroyed.
	for (auto it = m_bullets.begin(); it != m_bullets.end(); ) {
		if ((*it)->IsTagged()) {
			delete *it;
			it = m_bullets.erase(it);
		} else {
			(*it)->Update(delta);
			it++;
		}
		
	}
}

bool Map::HandleMessage(const Telegram& msg) {
	
	switch (msg.Msg) {
		//Spawn a new bullet at the request of a tank.
	case message_type::SPAWN_BULLET: {
		b2BodyDef bulldef;
		bulldef.type = b2_dynamicBody;
		bulldef.bullet = true;
		bulldef.position = vec2utils::ConvertVectorType<sf::Vector2f, b2Vec2>(((SpawnBulletData *)msg.ExtraInfo)->Position * METERS_PER_PIXEL);
		bulldef.angle = std::atan2(((SpawnBulletData *)msg.ExtraInfo)->Direction.y, ((SpawnBulletData *)msg.ExtraInfo)->Direction.x);
		bulldef.linearVelocity = vec2utils::ConvertVectorType<sf::Vector2f, b2Vec2>(((SpawnBulletData *)msg.ExtraInfo)->Direction * 100.0f);
		
		b2Body *bullBod = m_world->CreateBody(&bulldef);
		Box2DGameEntity *bullet = new Box2DGameEntity(*bullBod, *m_ta, "bulletSilverSilver_outline.png", Box2DGameEntity::CIRCLE, 1.0f, 1.0f, true);
		
		m_bullets.push_back(bullet);
		//Now that the telegram has been delivered, make sure you delete the extra info.
		delete (SpawnBulletData *)msg.ExtraInfo;
		return true;
	}
	default:
		return BaseGameEntity::HandleMessage(msg);
	}
	return false;
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//Draw background tiles
	target.draw(*m_background, states);
	//Draw the scores.
	for (std::size_t i = 0; i < 2; i++) {
		target.draw(*(ScoreGameEntity *)m_boundaries[i]->GetUserData(), states);
	}
	for (std::size_t i = 0; i < m_stats.size(); i++) {
		target.draw(*(TankStatsGameEntity *)m_stats[i], states);
	}

	//Draw static obstacles
	for (std::size_t i = 0; i < m_obstacles.size(); i++) {
		target.draw(*(m_obstacles[i]), states);
	}
	//Need to draw tanks here
	for (std::size_t i = 0; i < m_tanks.size(); i++) {
		target.draw(*(m_tanks[i]), states);
	}
	//Draw bullets
	for (auto it = m_bullets.begin(); it != m_bullets.end(); it++) {
		target.draw(**it, states);
	}
	//Draw smoke clouds
}

class Contacts {
public:
	enum {
		NONE = 0,
		SCORING_WALL_A = 1,
		SCORING_WALL_B = SCORING_WALL_A << 1,
		BOUNCY_WALL_A = SCORING_WALL_A << 2,
		BOUNCY_WALL_B = SCORING_WALL_A << 3,
		TANK_A = SCORING_WALL_A << 4,
		TANK_B = SCORING_WALL_A << 5,
		BULLET_A = SCORING_WALL_A << 6,
		BULLET_B = SCORING_WALL_A << 7,
	};
	static int GetContacts(b2Fixture *fixtureA, b2Fixture *fixtureB) {
		int c = NONE;
		
		if (fixtureA->GetBody()->GetType() == b2_staticBody) {
			//Either scoring or bouncy wall or obstacle.
			if (fixtureA->IsSensor()) {
				c |= SCORING_WALL_A;
			}
			else {
				c |= BOUNCY_WALL_A;
			}
		}
		else if (fixtureA->GetBody()->GetType() == b2_dynamicBody) {
			//Either a tank or a bullet.
			Box2DGameEntity *ge = (Box2DGameEntity *)fixtureA->GetBody()->GetUserData();
			if (ge->EntityType() == BaseGameEntity::PADDLE_TANK_ENTITY) {
				c |= TANK_A;
			}
			else {
				c |= BULLET_A;
			}
		}

		if (fixtureB->GetBody()->GetType() == b2_staticBody) {
			//Either scoring or bouncy wall or obstacle.
			if (fixtureB->IsSensor()) {
				c |= SCORING_WALL_B;
			}
			else {
				c |= BOUNCY_WALL_B;
			}
		}
		else if (fixtureB->GetBody()->GetType() == b2_dynamicBody) {
			//Either a tank or a bullet.
			Box2DGameEntity *ge = (Box2DGameEntity *)fixtureB->GetBody()->GetUserData();
			if (ge->EntityType() == BaseGameEntity::PADDLE_TANK_ENTITY) {
				c |= TANK_B;
			}
			else {
				c |= BULLET_B;
			}
		}
		return c;
	}
};

void MapContactListener::BeginContact(b2Contact* contact) {
	b2Fixture *fix1 = contact->GetFixtureA();
	b2Fixture *fix2 = contact->GetFixtureB();
	int c = Contacts::GetContacts(fix1, fix2);
	b2Fixture *temp;
	switch (c) {
	case (Contacts::SCORING_WALL_B | Contacts::BULLET_A) :
		//Swap fixtures
		temp = fix1;
		fix1 = fix2;
		fix2 = temp;
	case (Contacts::SCORING_WALL_A | Contacts::BULLET_B) :
		//Score a point.
		//((ScoreGameEntity *)fix1->GetBody()->GetUserData())->IncrementScore();
		// send message to points for appropriate team.
		// tag bullet for destruction.
		((BaseGameEntity *)fix2->GetBody()->GetUserData())->Tag();
		break;
	case (Contacts::TANK_B | Contacts::BULLET_A) :
		//swap fixtures
		temp = fix1;
		fix1 = fix2;
		fix2 = temp;
	case (Contacts::TANK_A | Contacts::BULLET_B) :
		((TankStatsGameEntity *)fix1->GetBody()->GetUserData())->LoseHealth();
		// send message to points for appropriate team.
		// tag bullet for destruction.
		((BaseGameEntity *)fix2->GetBody()->GetUserData())->Tag();
		break;
	case (Contacts::BULLET_A | Contacts::BULLET_B):
		//Tag both bullets for destruction.
		((BaseGameEntity *)fix1->GetBody()->GetUserData())->Tag();
		((BaseGameEntity *)fix2->GetBody()->GetUserData())->Tag();
		break;
	}
}
void MapContactListener::EndContact(b2Contact* contact) {

}


