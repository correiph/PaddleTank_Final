#include "TextureAtlas.h"
#include "tinyxml/tinyxml2.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <sstream>

TextureAtlas::TextureAtlas()
	: m_texture(nullptr)
{
	
}


TextureAtlas::~TextureAtlas()
{
	delete m_texture;
	for (std::map<std::string, sf::Sprite *>::iterator it = m_sprites.begin(); it != m_sprites.end(); it++) {
		delete it->second;
		it->second = nullptr;
	}
	m_sprites.clear();
}


bool TextureAtlas::loadFromFile(std::string const &atlasPath) {
	//Cannot reuse or reload.
	if (m_texture != nullptr) {
		return false;
		
	}
	//Open  the xml file for parsing.
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(atlasPath.c_str()) != tinyxml2::XML_NO_ERROR) {
		return false;
	}
	//Get the texture atlas root element and try and load the texture.
	tinyxml2::XMLElement *rootEl = doc.FirstChildElement("TextureAtlas");
	sf::Texture *t = new sf::Texture();
	if (!t->loadFromFile(rootEl->Attribute("imagePath"))) {
		delete t;
		return false;
	}
	m_texture = t;
	t->setSmooth(true);
	//Get each subtexture and create a sprite for it. Put the sprite
	// in the map with the name of the sprite as the key.
	//A duplicate name replaces the previous one and leaks the sprite.
	tinyxml2::XMLElement *subEl = rootEl->FirstChildElement("SubTexture");
	while (subEl != nullptr) {
		std::string name(subEl->Attribute("name"));
		int x = subEl->IntAttribute("x");
		int y = subEl->IntAttribute("y");
		int w = subEl->IntAttribute("width");
		int h = subEl->IntAttribute("height");
		//If the possiblity of a duplicate name exists it needs to be handled before this next line.
		m_sprites[name] = new sf::Sprite(*m_texture, sf::IntRect(x, y, w, h));
		subEl = subEl->NextSiblingElement("SubTexture");
	}

	return true;
}

sf::Sprite const * const TextureAtlas::GetSprite(std::string const &name) {
	//Check if a sprite with that name is in the map. If it is return it,
	// otherwise return a nullptr.
	std::map<std::string, sf::Sprite *>::iterator it = m_sprites.find(name);
	if (it != m_sprites.end()) {
		return it->second;
	}
	return nullptr;
}
