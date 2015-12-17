 #include "AssetManager.h"

#include "TextureAtlas.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

template<typename T>
AssetManager<T>::AssetManager() {

}
 
template<typename T>
AssetManager<T>::~AssetManager() {
	UnloadAllAssets();
}

/// <summary>
/// Loads the asset.
/// </summary>
/// <param name="path">The path.</param>
/// <returns></returns>
template<typename T>
bool AssetManager<T>::LoadAsset(std::string const &path) {
	std::map<std::string, T *>::iterator it = m_assets.find(path);
	//Found in the map.
	if (it != m_assets.end()) {
		return true;
	}
	T *t = new T();
	//Try and load the file from a path.
	if (t->loadFromFile(path)) {
		m_assets[path] = t;
		return true;
	}
	//Can't be found or loaded. Delete and return false.
	delete t;
	return false;
}

template<typename T>
T *AssetManager<T>::GetAsset(std::string const &path) {
	std::map<std::string, T *>::iterator it = m_assets.find(path);
	//Found in the map.
	if (it != m_assets.end()) {
		return it->second;
	}
	T *t = new T();
	//Try and load the file from a path.
	if (t->loadFromFile(path)) {
		m_assets[path] = t;
		return t;
	}
	//Can't be found or loaded. Delete and return false.
	delete t;
	return nullptr;
}

template<typename T>
void AssetManager<T>::UnloadAllAssets() {
	for (std::map<std::string, T *>::iterator it = m_assets.begin(); it != m_assets.end(); it++) {
		delete it->second;
		it->second = nullptr;
	}
	m_assets.clear();
}

template class AssetManager < sf::Font >;
template class AssetManager < sf::Texture >;
template class AssetManager < TextureAtlas >;