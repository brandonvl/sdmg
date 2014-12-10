#pragma once

#include "Box2D\Box2D.h"
#include <vector>

class AABBDetectionCallBack : public b2QueryCallback
{
public:
	AABBDetectionCallBack();
	virtual ~AABBDetectionCallBack();
	bool ReportFixture(b2Fixture* fixture) override;
private:
	std::vector<b2Body*> _bodiesFound;
};

