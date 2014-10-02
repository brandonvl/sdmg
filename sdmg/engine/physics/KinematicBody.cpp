//
//
//  @ Project : SDMG
//  @ File Name : KinematicBody.cpp
//  @ Date : 30-9-2014
//  @ Author : 42IN13SAd
//
//

#include "KinematicBody.h"
#include "Box2D\Box2D.h"

namespace sdmg {
	namespace engine {
		namespace physics {

			KinematicBody::KinematicBody()
			{
				_bodies = new std::vector < b2Body*>();
			}

			KinematicBody::KinematicBody(b2Vec2 *startLocation)
			{
				_bodies = new std::vector < b2Body*>();
				_startLocation = startLocation;
			}

			KinematicBody::~KinematicBody()
			{
			}

			void KinematicBody::setStartLocation(b2Vec2 *vec)
			{
				_startLocation = vec;
			}

			void KinematicBody::setEndLocation(b2Vec2 *vec)
			{
				_endLocation = vec;
			}

			void KinematicBody::setDirection(KinematicBody::Direction direction)
			{
				_direction = direction;
			}

			void KinematicBody::addBody(b2Body *body, b2Vec2 velocity)
			{
				if (body->GetType() == b2_dynamicBody)
				{
					_bodies->push_back(body);
					//  Player *player = static_cast<Player*>(body->GetUserData());
					//  player->setPlatformVelocityX(velocity.x);
				}
			}

			void KinematicBody::removeBody(b2Body *body)
			{
				//_bodies->
				_bodies->erase(std::remove(_bodies->begin(), _bodies->end(), body), _bodies->end());
				//  Player *player = static_cast<Player*>(body->GetUserData());
				//  player->setPlatformVelocityX(0.0f);
			}

			void KinematicBody::changeBodiesLinearVelocity(b2Vec2 velocity)
			{
				for (auto i = _bodies->begin(); i != _bodies->end(); i++)
				{
					(*i)->SetLinearVelocity(velocity);
					//  Player *player = static_cast<Player*>((*i)->GetUserData());
					//  player->setPlatformVelocityX(velocity.x);
				}
			}

			b2Vec2* KinematicBody::getStartLocation()
			{
				return _startLocation;
			}

			b2Vec2* KinematicBody::getEndLocation()
			{
				return _endLocation;
			}

			KinematicBody::Direction KinematicBody::GetDirection()
			{
				return _direction;
			}

			b2Body *KinematicBody::getBody()
			{
				return _body;
			}

			void KinematicBody::setBody(b2Body *body)
			{
				_body = body;
			}


			void KinematicBody::checkDirectionChange()
			{
				/*
				if (tmp->GetType() == b2_kinematicBody)
				{
					KinematicBody *kinematicBody = static_cast<KinematicBody*>(tmp->GetUserData()); // (*_kinecticBodies)[tmp];

					b2Vec2 *endpoint = kinematicBody->getEndLocation();
					if (kinematicBody->GetDirection() == KinematicBody::Direction::Right)
					{
						if (tmp->GetPosition().x >= kinematicBody->getEndLocation()->x)
						{
							kinematicBody->setDirection(KinematicBody::Direction::Left);
							b2Vec2 vec = b2Vec2(-tmp->GetLinearVelocity().x, 0.0);
							kinematicBody->changeBodiesLinearVelocity(vec);
							tmp->SetLinearVelocity(vec);
						}
					}
					else if (kinematicBody->GetDirection() == KinematicBody::Direction::Left)
					{
						if (tmp->GetPosition().x <= kinematicBody->getStartLocation()->x)
						{
							kinematicBody->setDirection(KinematicBody::Direction::Right);
							b2Vec2 vec = b2Vec2(std::abs(tmp->GetLinearVelocity().x), 0.0);
							kinematicBody->changeBodiesLinearVelocity(vec);
							tmp->SetLinearVelocity(vec);
						}
					}
					else if (kinematicBody->GetDirection() == KinematicBody::Direction::Down)
					{
						if (tmp->GetPosition().y >= kinematicBody->getEndLocation()->y)
						{
							kinematicBody->setDirection(KinematicBody::Direction::Up);
							//tmp->SetLinearVelocity(b2Vec2(0.0, -tmp->GetLinearVelocity().y));


							b2Vec2 vec = b2Vec2(b2Vec2(0.0, -tmp->GetLinearVelocity().y));
							kinematicBody->changeBodiesLinearVelocity(vec);
							tmp->SetLinearVelocity(vec);
						}
					}
					else if (kinematicBody->GetDirection() == KinematicBody::Direction::Up)
					{
						if (tmp->GetPosition().y <= kinematicBody->getStartLocation()->y)
						{
							kinematicBody->setDirection(KinematicBody::Direction::Down);
							//tmp->SetLinearVelocity(b2Vec2(0.0, std::abs(tmp->GetLinearVelocity().y)));



							b2Vec2 vec = b2Vec2(b2Vec2(0.0, std::abs(tmp->GetLinearVelocity().y)));
							kinematicBody->changeBodiesLinearVelocity(vec);
							tmp->SetLinearVelocity(vec);
						}
					}
				}
				*/
			}		
		}
	}
}