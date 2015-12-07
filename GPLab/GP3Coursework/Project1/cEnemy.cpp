#include "cEnemy.h"
#include <time.h>
cEnemy::cEnemy() : cModel()
{
	
}

void cEnemy::randomise()
{
	// makes rand more random using time as seed
	srand (time(NULL));

	cModel::m_mdlPosition.y =  20;
	cModel::m_mdlPosition.x = rand() % 21 + (-10);
	cModel::m_mdlPosition.z = -20.0f;   
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

	

	if (m_mdlPosition.y <= -30.0f)
	{
		cEnemy::setIsActive(false);
	}
	
}

cEnemy::~cEnemy()
{

}

