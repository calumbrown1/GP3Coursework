#include "cPlayer.h"


cPlayer::cPlayer() : cModel()
{

}

void cPlayer::attachInputMgr(cInputMgr* inputMgr)
{
	m_InputMgr = inputMgr;
}

void cPlayer::update(float elapsedTime)
{
	if (m_InputMgr->isKeyDown(VK_RIGHT))
	{
		translationZ = 1.0f;
	}
	if (m_InputMgr->isKeyDown(VK_LEFT))
	{
		translationZ = -1.0f;
	}

	if (m_InputMgr->isKeyDown(VK_SPACE) && charge >= 10)
	{
		glm::vec3 mdlLaserDirection;

		mdlLaserDirection.x = 0.0f;//-(float)glm::sin(glm::radians(this->getRotation()));
		mdlLaserDirection.y = 2.0f;//(float)glm::cos(glm::radians(this->getRotation()));
		mdlLaserDirection.z = 0.0f;
		//mdlLaserDirection *= -1;

		// Add new bullet sprite to the vector array
		theTardisLasers.push_back(new cLaser);
		int numLasers = theTardisLasers.size() - 1;
		theTardisLasers[numLasers]->initialise(glm::vec3(0, 0, 0), 0.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0, 0, 0), 5.0f, true);
		theTardisLasers[numLasers]->setDirection(mdlLaserDirection);
		theTardisLasers[numLasers]->setRotation(0.0f);
		theTardisLasers[numLasers]->setScale(glm::vec3(0.015, 0.015, 0.015));
		theTardisLasers[numLasers]->setSpeed(5.0f);
		theTardisLasers[numLasers]->setPosition(this->getPosition() + mdlLaserDirection);
		theTardisLasers[numLasers]->setIsActive(true);
		theTardisLasers[numLasers]->setMdlRadius(2.5f);
		//theTardisLasers[numLasers]->setMdlDimensions(theLaser.getModelDimensions());
		theTardisLasers[numLasers]->update(elapsedTime);
		// play the firing sound
		if (music == true)m_SoundMgr->getSnd("Shot")->playAudio(AL_TRUE);
		charge -= 10;
	}

	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	for (vector<cLaser*>::iterator laserIterartor = theTardisLasers.begin(); laserIterartor != theTardisLasers.end(); ++laserIterartor)
	{
		(*laserIterartor)->update(elapsedTime);
		for (vector<cEnemy*>::iterator enemyIterator = theEnemy.begin(); enemyIterator != theEnemy.end(); ++enemyIterator)
		{
			if ((*enemyIterator)->SphereSphereCollision((*laserIterartor)->getPosition(), (*laserIterartor)->getMdlRadius()))
			{
				// if a collision set the bullet and spaceship to false
 				(*enemyIterator)->setIsActive(false);
				(*laserIterartor)->setIsActive(false);
				//Create explosion at position
				theExplosions.push_back(new cExploison);
				int numExp = theExplosions.size() - 1; 
				theExplosions[numExp]->initialise((*enemyIterator)->getPosition(), 0.0f, glm::vec3(0.1, 0.1, 0.1), glm::vec3(0, -1, 0), 6.0f,true);
				theExplosions[numExp]->setScale(glm::vec3(0.1, 0.1, 0.1));
				// play the explosion sound.
				if (music==true)m_SoundMgr->getSnd("Explosion")->playAudio(AL_TRUE);
				score += 10;
			}
		}
	}

	vector<cLaser*>::iterator laserIterartor = theTardisLasers.begin();
	while (laserIterartor != theTardisLasers.end())
	{
		if ((*laserIterartor)->isActive() == false)
		{
			laserIterartor = theTardisLasers.erase(laserIterartor);
		}
		else
		{
			++laserIterartor;
		}
	}

	vector<cEnemy*>::iterator enemyIterartor = theEnemy.begin();
	while (enemyIterartor != theEnemy.end())
	{
		if ((*enemyIterartor)->isActive() == false)
		{
			enemyIterartor = theEnemy.erase(enemyIterartor);
		}
		else
		{
			++enemyIterartor;
		}
	}

	// Find out what direction we should be thrusting, using rotation.
	glm::vec3 mdlVelocityAdd;
	mdlVelocityAdd.x = 1.0f; //-(float)glm::sin(glm::radians(m_mdlRotation));  // Remember to adjust for radians
	mdlVelocityAdd.y = 0.0f;
	mdlVelocityAdd.z = 0.0f; //-(float)glm::cos(glm::radians(m_mdlRotation));

	m_mdlRotation -= rotationAngle;

	mdlVelocityAdd *= translationZ;
	m_mdlDirection += mdlVelocityAdd;

	m_mdlPosition += m_mdlDirection * m_mdlSpeed *elapsedTime;
	m_mdlDirection *= 0.80f;


	rotationAngle = 0;
	translationZ = 0;
	if (charge <100) charge += 0.1;
}

cPlayer::~cPlayer()
{

}