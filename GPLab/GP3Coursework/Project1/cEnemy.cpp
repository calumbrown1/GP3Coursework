#include "cEnemy.h"

cEnemy::cEnemy() : cModel()
{
	
}

void cEnemy::randomise()
{
	//if (rand() % 2 == 0)
	//{
		cModel::m_mdlPosition.y = 30.0f;
	//}
	//else
	//{
		//cModel::m_mdlPosition.x = PLAYFIELDX;
	//}
	cModel::m_mdlPosition.x = -50.0f + (((float)rand()) / (float)RAND_MAX) * (50.0f - -50.0f);
	cModel::m_mdlPosition.z = -20.0f;   // random number as a float between 0 & 1
	cModel::m_mdlRotation = (rand() / (float)RAND_MAX) * 2 * 3.14f;
	cModel::m_mdlDirection.x = 0;
	cModel::m_mdlDirection.y = -1;
	cModel::m_mdlDirection.z = 0;
	cModel::m_mdlSpeed = m_EnemyMinSpeed + rand() / (float)RAND_MAX * m_EnemyMaxSpeed;
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
}

cEnemy::~cEnemy()
{

}

