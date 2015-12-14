#ifndef _CEXPLOSION_H
#define _CEXPLOSION_H

#include "GameConstants.h"
#include "cModel.h"

class cExploison : public cModel
{
public:
	cExploison();

	virtual void update(float elapsedTime);

	~cExploison();

private:
	int deleteTime = 50;
};
#endif