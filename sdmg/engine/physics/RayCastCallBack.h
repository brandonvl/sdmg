#pragma once

#include "Box2D\Box2D.h"
#include <functional>
#include <vector>

namespace sdmg {
	namespace engine {
		namespace physics {

			class PhysicsEngine;

			class RayCastCallBack : public b2RayCastCallback
			{
			private:
				b2Vec2 _pointOne;
				b2Vec2 _pointTwo;
				std::function<float32(b2Body*)> *_callBack;
				PhysicsEngine *_engine;
				std::vector<b2Body*> _results;
			public:
				RayCastCallBack(float x1, float y1, float x2, float y2, std::function<float32(b2Body*)> *callBack);
				virtual ~RayCastCallBack();
				std::vector<b2Body*> &getResults() { return _results; }
				const b2Vec2 getPointOne() { return _pointOne; }
				const b2Vec2 getPointTwo() { return _pointTwo; }
				void setPhysicsEngine(PhysicsEngine *engine) { _engine = engine; }
				float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) override;
			};
		}
	}
}

