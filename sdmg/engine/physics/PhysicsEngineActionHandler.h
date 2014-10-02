//
//
//  @ Project : SDMG
//  @ File Name : PhysicsEngineActionHandler.h
//  @ Date : 30-9-2014
//  @ Author : 42IN13SAd
//
//

#pragma once

namespace sdmg {
	namespace engine {
		class MovableGameObject;

		namespace physics {
			class PhysicsEngineActionHandler
			{
			public:
				PhysicsEngineActionHandler();
				virtual ~PhysicsEngineActionHandler();
				void moveLeft(MovableGameObject *obj);
				void moveRight(MovableGameObject *obj);
				void idle(MovableGameObject *obj);
				void jump(MovableGameObject *obj);
				void shortAttack(MovableGameObject *obj);
				void middleAttack(MovableGameObject *obj);
				void longAttack(MovableGameObject *obj);
			};
		}
	}
}