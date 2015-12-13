#include "cStar.h"
#include "cModel.h"
float speed;
cStar::cStar() : cModel()
{
	speed = rand()%(int)((m_StarMaxSpeed - m_StarMinSpeed) + 1) + m_StarMinSpeed;
}

void cStar::update(float elapsedTime)
{
	cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed * elapsedTime;
	if (m_mdlPosition.y <= -25.0f)
	{
		cModel::setIsActive(false);
	}
}

cStar::~cStar()
{

}