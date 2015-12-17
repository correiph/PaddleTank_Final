#pragma once
#include <SFML\Graphics\Drawable.hpp>

namespace sf {
	class VertexArray;
	class Texture;
}

/// <summary>
/// TiledBackground is a class that can be used to create a tiled background with a single repeating texture.
/// Note that this class is not responsible for cleaning up the texture since it may be shared.
/// </summary>
/// <author>
/// Kevin Forest 2015
/// </author>
class TiledBackground :
	public sf::Drawable
{
public:	
	/// <summary>
	/// Initializes a new instance of the <see cref="TiledBackground"/> class. Keeps a reference to the texture.
	/// </summary>
	/// <param name="texture">The texture that contains the background image.</param>
	/// <param name="bgRect">The area that needs to be filled by the background area.</param>
	/// <param name="texRect">The subrect of the texture for the sprite that will be tiled.</param>
	TiledBackground(sf::Texture const &texture, sf::IntRect const &bgRect, sf::IntRect const &texRect);
	virtual ~TiledBackground();

protected:
	//Render the array to the screen.
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
	//Vertices and texture used to render the tiled background.
	sf::VertexArray *m_vertices;
	sf::Texture const *m_texture;
};

