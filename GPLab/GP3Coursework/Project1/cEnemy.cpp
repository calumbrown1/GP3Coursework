#include "cEnemy.h"

cEnemy::cEnemy() : cModel()
{
	
}

void cEnemy::initialise()
{


	cModel::m_mdlPosition.y = 20.0f;
	cModel::m_mdlPosition.x = (float)rand() / RAND_MAX * 40.0 - 20.0f;
	cModel::m_mdlPosition.z = -20.0f;   // random number as a float between 0 & 1
	cModel::m_mdlRotation = 180.0f;
	cModel::m_mdlDirection.x = 0;
	cModel::m_mdlDirection.y = -1;
	cModel::m_mdlDirection.z = 0;
	cModel::m_mdlSpeed = 6.0f;
	cModel::m_IsActive = true;
}

void cEnemy::update(float elapsedTime)
{
	cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed * elapsedTime;


}

cEnemy::~cEnemy()
{

}

