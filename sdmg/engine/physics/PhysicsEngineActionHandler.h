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
				void kneel(MovableGameObject *obj);
				void shortRangeAttack(MovableGameObject *obj);
				void longRangeAttack(MovableGameObject *obj);
				void respawn(MovableGameObject *obj);
				void knockbackLeft(MovableGameObject *obj);
				void knockbackRight(MovableGameObject *obj);


				void midRangeAttackBegin(MovableGameObject *obj);
				void midRangeAttack(MovableGameObject *obj);
				void midRangeAttackEnd(MovableGameObject *obj);
			};
		}
	}
}