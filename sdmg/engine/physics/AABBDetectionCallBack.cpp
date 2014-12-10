#include "AABBDetectionCallBack.h"


AABBDetectionCallBack::AABBDetectionCallBack()
{
}


AABBDetectionCallBack::~AABBDetectionCallBack()
{
}

bool AABBDetectionCallBack::ReportFixture(b2Fixture *fixture)
{

	return true;
}
