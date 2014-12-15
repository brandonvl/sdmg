#include "RayCastCallBack.h"
#include "engine\physics\PhysicsEngine.h"

namespace sdmg {
	namespace engine {
		namespace physics {
			RayCastCallBack::RayCastCallBack(float x1, float y1, float x2, float y2, std::function<float32(b2Body*)> *callBack)
			{
				_pointOne = b2Vec2(x1, y1);
				_pointTwo = b2Vec2(x2, y2);
				_callBack = callBack;
			}


			RayCastCallBack::~RayCastCallBack()
			{
			}

			void RayCastCallBack::clearResults() {
				_results.clear();
			}

			void RayCastCallBack::setPointOne(float x, float y) {
				_pointOne.x = x;
				_pointOne.y = y;
			}

			void RayCastCallBack::setPointTwo(float x, float y) {
				_pointTwo.x = x;
				_pointTwo.y = y;
			}

			float32 RayCastCallBack::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
			{
				float32 returnValue = 1.0F;

				if (_callBack) {
					returnValue = (*_callBack)(fixture->GetBody());
				}

				_results.push_back(fixture->GetBody());

				return returnValue;
			}
		}
	}
}
