#include "TiledBackground.h"

#include <SFML/Graphics.hpp>


TiledBackground::TiledBackground(sf::Texture const &texture, sf::IntRect const &bgRect, sf::IntRect const &texRect) 
	: m_vertices(new sf::VertexArray()), m_texture(&texture)
{
	//Get the number of tiles needed to tile the entire background area.
	std::size_t w = bgRect.width / texRect.width + 1;
	std::size_t h = bgRect.height / texRect.height + 1;
	//Resize the vertex array to the required size.
	m_vertices->resize(w*h * 6U); //Needs 6 vertices per tile. Note that this can be done with a triangle strip and fewer resulting vertices using degenerate triangles.
	m_vertices->setPrimitiveType(sf::Triangles);
	std::size_t i = 0;
	//Iterate through the entire array and set the vertices.
	for (std::size_t y = 0; y < h; y++) {
		for (std::size_t x = 0; x < w; x++) {
			//Create triangles for this tile.
			// TL
			(*m_vertices)[i].position.x = (float)((texRect.width * x) + bgRect.left);
			(*m_vertices)[i].position.y = (float)((texRect.height * y) + bgRect.top);
			(*m_vertices)[i].texCoords.x = (float)(texRect.left);
			(*m_vertices)[i].texCoords.y = (float)(texRect.top);
			++i;
			// BL
			(*m_vertices)[i].position.x = (float)((texRect.width * x) + bgRect.left);
			(*m_vertices)[i].position.y = (float)((texRect.height * (y + 1)) + bgRect.top);
			(*m_vertices)[i].texCoords.x = (float)(texRect.left);
			(*m_vertices)[i].texCoords.y = (float)(texRect.top + texRect.height);
			++i;
			// TR
			(*m_vertices)[i].position.x = (float)((texRect.width * (x + 1)) + bgRect.left);
			(*m_vertices)[i].position.y = (float)((texRect.height * y) + bgRect.top);
			(*m_vertices)[i].texCoords.x = (float)(texRect.left + texRect.width);
			(*m_vertices)[i].texCoords.y = (float)(texRect.top);
			++i;
			// BL
			(*m_vertices)[i].position.x = (float)((texRect.width * x) + bgRect.left);
			(*m_vertices)[i].position.y = (float)((texRect.height * (y + 1)) + bgRect.top);
			(*m_vertices)[i].texCoords.x = (float)(texRect.left);
			(*m_vertices)[i].texCoords.y = (float)(texRect.top + texRect.height);
			++i;
			// BR
			(*m_vertices)[i].position.x = (float)((texRect.width * (x + 1)) + bgRect.left);
			(*m_vertices)[i].position.y = (float)((texRect.height * (y + 1)) + bgRect.top);
			(*m_vertices)[i].texCoords.x = (float)(texRect.left + texRect.width);
			(*m_vertices)[i].texCoords.y = (float)(texRect.top + texRect.height);
			++i;
			// TR
			(*m_vertices)[i].position.x = (float)((texRect.width * (x + 1)) + bgRect.left);
			(*m_vertices)[i].position.y = (float)((texRect.height * y) + bgRect.top);
			(*m_vertices)[i].texCoords.x = (float)(texRect.left + texRect.width);
			(*m_vertices)[i].texCoords.y = (float)(texRect.top);
			++i;
		}
	}
}


TiledBackground::~TiledBackground()
{
	//Clear the vertices and delete the array.
	m_vertices->clear();
	delete m_vertices;
	//Note that this class is not responsible for cleaning up the texture since it may be shared.
}


void TiledBackground::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	//Set the texture to use
	states.texture = m_texture;
	//Render
	target.draw(*m_vertices, states);
}
