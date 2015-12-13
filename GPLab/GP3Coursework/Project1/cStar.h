#ifndef _CSTAR_H
#define _CSTAR_H


#include "GameConstants.h"
#include "cModel.h"

class cStar : public cModel
{
public :
	cStar();
	virtual void update(float elapsedTime);

	~cStar();

private : 
	const float m_StarMaxSpeed = 20.0f;
	const float m_StarMinSpeed = 10.0f;
};
#endif