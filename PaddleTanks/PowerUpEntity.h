#include "Box2DGameEntity.h"
struct b2Vec2;

class PowerUpEntity : public Box2DGameEntity
{
public:
	enum { GOOD_POWERUP = 1, BAD_POWERUP = 2};

	PowerUpEntity(b2Body &body, TextureAtlas &atlas, std::string const &tankSpriteName, float density = 1.0f);
	
	virtual ~PowerUpEntity();
	void PowerUpEntity::ApplyLinearImpulse(b2Vec2 imp);
	void PowerUpEntity::Action();
	int PowerUpEntity::GetPowerUpType();
	void PowerUpEntity::Update(float delta);
protected:
	char m_powerUpType;
	//Render the sprite.
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
};