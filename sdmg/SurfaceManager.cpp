#include "SurfaceManager.h"

namespace SDMG {
	SurfaceManager::SurfaceManager()
	{
		_surfaces = new std::map<std::string, SDL_Surface*>();
	}


	SurfaceManager::~SurfaceManager()
	{
	}

	void SurfaceManager::loadStatic(std::string path) {

		// Load image from disk
		// Create new StaticSurface
		// Add StaticSurface to _surfaces map
	}

	void SurfaceManager::loadSpriteMap(std::string path) {

		// Load image from disk
		// Create new SpriteMapSurface
		// Add SpriteMapSurface to _surfaces map
	}
}