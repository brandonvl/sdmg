#pragma once
#include <map>
#include "sdl\include\SDL.h"

namespace SDMG {
	class SurfaceManager
	{
	public:
		SurfaceManager();
		~SurfaceManager();

		void loadStatic(std::string path);
		void loadSpriteMap(std::string path);

	private:
		std::map<std::string, SDL_Surface*>* _surfaces;

	};
}