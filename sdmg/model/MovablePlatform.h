//
//
//  @ Project : SDMG
//  @ File Name : MovablePlatform.h
//  @ Date : 2-10-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include "engine\MovableGameObject.h"
#include "Box2D/Box2D.h"

using namespace sdmg::engine;

namespace sdmg {
	namespace model {
		class MovablePlatform : public MovableGameObject {
		public:
			MovablePlatform();
			virtual ~MovablePlatform();

			float getStartLocationX();
			float getStartLocationY();
			float getEndLocationX();
			float getEndLocationY();
			void setStartLocation(b2Vec2 vec);
			void setEndLocation(b2Vec2 vec);
			void checkDirectionChange();
			bool getDieOnImpact();
			void setDieOnImpact(bool isDieOnImpact);
		private:
			bool _dieOnImpact;
			b2Vec2 _startLocation, _endLocation;
		};
	}
}