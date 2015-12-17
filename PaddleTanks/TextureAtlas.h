#pragma once

#include <map>
#include <string>

namespace sf {
	class Sprite;
	class Texture;
}
/// <summary>
/// TextureAtlas is a class that wraps a collections of sprites that are all drawn from a single texture. The atlas is
/// loaded by providing an XML atlas file that includes the URL of the texture file associated with the atlas.
/// A texture atlas can only draw from a single texture. When you ask the atlas for a sprite you need to make a copy
/// of the sprite it returns. 
/// Note that a freshly instantiated texture atlas is not in a useful state - you need to loadFromFile() to ensure
/// it is properly initialised. In addition you should avoid reusing an atlas by loading a new file - instead you should
/// delete the old atlas and create a new one.
/// TODO: Add a means to get the sprite names?
/// </summary>
///
/// <file-format>
/*
    <TextureAtlas imagePath="path/to/texture/file">
		<SubTexture name="spriteName" x="i" y="j" width="k" height="l"/> ...
	</TextureAtlas>
	Where i, j, k, and l are integers that describe the subrect for that sprite, and
	'spriteName' should be unique for each SubTexture - if a duplicate name exists only
	the last subtexture with that name will be available.
*/
/// </file-format>
///
/// <author>
/// Kevin Forest 2015
/// </author>
class TextureAtlas
{
public:
	//Create a new texture atlas.
	TextureAtlas();
	virtual ~TextureAtlas();	

	/// <summary>
	/// Loads the atlas from an XML file.
	/// </summary>
	/// <param name="atlasPath">The path to the atlas file.</param>
	/// <returns>true if the atlas is loaded properly, false otherwise.</returns>
	bool loadFromFile(std::string const &atlasPath);

	/// <summary>
	/// Gets a sprite from the atlas and returns it. The caller should make a copy of this sprite
	/// rather than using the one in the atlas.
	/// </summary>
	/// <param name="name">The name of the sprite in the atlas.</param>
	/// <returns>A pointer to the sprite if found in the atlas, NULL otherwise.</returns>
	sf::Sprite const * const GetSprite(std::string const &name);

	//Returns a reference to the atlases Texture (if one has been loaded)
	// otherwise return NULL.
	sf::Texture const * const GetTexture() {
		return m_texture;
	}
private:
	//Pointers to the texture and the list of sprites. Sprites are
	// indexed by their name as found in the XML file.
	sf::Texture *m_texture;
	std::map<std::string, sf::Sprite *> m_sprites;
};

