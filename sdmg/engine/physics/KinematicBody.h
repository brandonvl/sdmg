//
//
//  @ Project : SDMG
//  @ File Name : KinematicBody.h
//  @ Date : 30-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once
#include <vector>
#include "Box2D\Box2D.h"

namespace sdmg {
	namespace engine {
		namespace physics {

			class KinematicBody
			{
			public:
				KinematicBody();
				KinematicBody(b2Vec2 *startLocation);
				virtual ~KinematicBody();

				enum class Direction { Up, Left, Down, Right, None };

				void setStartLocation(b2Vec2 *vec);
				void setEndLocation(b2Vec2 *vec);
				void setDirection(Direction direction);
				void addBody(b2Body *body, b2Vec2 velocity);
				void removeBody(b2Body *body);
				void changeBodiesLinearVelocity(b2Vec2 velocity);
				void checkDirectionChange();

				void setBody(b2Body *body);
				b2Body *getBody();

				b2Vec2* getStartLocation();
				b2Vec2* getEndLocation();
				Direction GetDirection();

			private:
				b2Body *_body;
				b2Vec2 *_startLocation, *_endLocation;
				KinematicBody::Direction _direction;
				std::vector<b2Body*> *_bodies;
			};
		};
	}
}