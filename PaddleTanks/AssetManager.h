#pragma once

#include <map>
#include <string>

class TextureAtlas;
namespace sf {
	class Texture;
	class Font;
}

#define G_FontManager (FontManager::Instance())
#define G_TextureManager (TextureManager::Instance())
#define G_AtlasManager (AtlasManager::Instance())

/// <summary>
/// A templated Singleton AssetManager class. Each asset type can have it's own asset manager.
/// "Assets" are defined as any class that has the following:
///		-Ability to be reference via Pointer
///		-A no-arg constructor (explict or default)
///		-A public method named loadFromFile that returns a bool and takes a string as a parameter
///			bool loadFromFile(std::string const &filepath)
/// Once loaded an asset cannot be individually unloaded. This is deliberate since the role of
/// the asset manager is to serve as a central repository for shared assets like
/// textures and fonts, since it is difficult to ensure that all external references to a
/// loaded asset have been released. You have to nuke the entire collection of loaded
/// assets at once - this ensures you are very, very careful about asset management.
/// To make life a little easier and declutter this header file, the bodies are implemented in
/// a .cpp file. To ensure that the code links there are explicit typedefs of the various
/// Asset Manager types. New asset types will require new typedefs.
///
/// </summary>
/// <author>
/// Kevin Forest 2015
/// </author>
template<typename T>
class AssetManager {
public:
	static AssetManager<T> &Instance() {
		static AssetManager<T> instance;
		return instance;
	}
private:
	AssetManager();
	//Do not implement!
	AssetManager(const AssetManager&);
	AssetManager& operator=(const AssetManager&);

public:
	~AssetManager();

	//Attempts to load the asset found at path. If that asset has already been 
	// loaded returns true (a duplicate is not loaded). If that asset has
	// not already been loaded, attempt to load it. If the asset is successfully
	// loaded return true, otherwise return false.
	//Basically if this returns true the asset is loaded and ready to go, if it returns false
	// the asset was not loaded and is not ready to go.
	bool LoadAsset(std::string const &path);
	//Returns a pointer to the asset. The pointer is not const so be nice. If the asset
	// is not loaded, attempt to load it first. Then return a pointer to the assets
	// (if loaded) or nullptr (if not loaded).
	T *GetAsset(std::string const &path);

	//Unload all of the assets and delete them from memory. 
	// IF ANY EXTERNAL REFERENCES TO THOSE ASSETS STILL EXIST YOU WILL GET UNDEFINED BEHAVIOUR!!!
	// You have been warned.
	void UnloadAllAssets();

private:
	//Map for the assets, key is filename/path.
	std::map<std::string, T *> m_assets;
	

};
//Explicit typedefs to ensure the various templated classes are compiled and linked.
typedef AssetManager<sf::Font> FontManager;
typedef AssetManager<sf::Texture> TextureManager;
typedef AssetManager<TextureAtlas> AtlasManager;


