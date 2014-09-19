#pragma once
#include "Surface.h"

namespace SDMG {
	class StaticSurface :
		public Surface
	{
	public:
		StaticSurface();
		virtual ~StaticSurface();
	};
}
