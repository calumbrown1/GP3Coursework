#include "cEnemy.h"

cEnemy::cEnemy() : cModel()
{
	
}

void cEnemy::randomise()
{

	cModel::m_mdlPosition.y = 30.0f + (((float)rand()) / (float)RAND_MAX) * (35.0f + 35.0f);
	cModel::m_mdlPosition.x = -50.0f + (((float)rand()) / (float)RAND_MAX) * (50.0f + 50.0f);
	cModel::m_mdlPosition.z = -20.0f;   // random number as a float between 0 & 1
	cModel::m_mdlRotation = (rand() / (float)RAND_MAX) * 2 * 3.14f;
	cModel::m_mdlDirection.x = 0;
	cModel::m_mdlDirection.y = -1;
	cModel::m_mdlDirection.z = 0;
	cModel::m_mdlSpeed = 5.0f;
	cModel::m_IsActive = true;
}

void cEnemy::update(float elapsedTime)
{
	 cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed * elapsedTime;
	/*
	if (cModel::m_mdlPosition.x > PLAYFIELDX)
	cModel::m_mdlPosition.x -= 2 * PLAYFIELDX;
	if (cModel::m_mdlPosition.x < -PLAYFIELDX)
	cModel::m_mdlPosition.x += 2 * PLAYFIELDX;
	if (cModel::m_mdlPosition.z > PLAYFIELDZ)
	cModel::m_mdlPosition.z -= 2 * PLAYFIELDZ;
	if (cModel::m_mdlPosition.z < -PLAYFIELDZ)
	cModel::m_mdlPosition.z += 2 * PLAYFIELDZ;
	*/

	/*
	CAUSES VECTOR ERROR
	if (m_mdlPosition.y <= -20.0f)
	{
		cEnemy::setIsActive(false);
	}
	*/
}

cEnemy::~cEnemy()
{

}

