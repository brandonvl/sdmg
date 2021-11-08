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

			enum class Moveing {
				REPEAT, ONCE
			};

			float getStartLocationX();
			float getStartLocationY();
			float getEndLocationX();
			float getEndLocationY();
			void setStartLocation(b2Vec2 vec);
			void setEndLocation(b2Vec2 vec);
			void checkDirectionChange();
			int getDamageOnImpact();
			void setDamageOnImpact(int damage);
			Moveing getMoveing();
			void setMoveing(Moveing moveing);

			bool getMustBeDestroyed();
			void setMustBeDestroyed(bool destroy);

			MovableGameObject *getOwner();
			void setOwner(MovableGameObject *owner);

		private:
			int _damageOnImpact;
			Moveing _moveing;
			MovableGameObject *_owner = nullptr;
			b2Vec2 _startLocation, _endLocation;
			bool _mustBeDestroyed;
			void checkStartHigherThanEnd();
			void checkEndHigherThanStart();
			void changeDirection(MovableGameObject::Direction direction);
		};
	}
}