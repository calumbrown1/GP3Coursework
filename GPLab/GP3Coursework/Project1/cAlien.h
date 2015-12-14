#ifndef _CALIEN_H
#define _CALIEN_H

#include "GameConstants.h"
#include "cModel.h"

class cAlien : public cModel
{
public:
	cAlien();

	virtual void update(float elapsedTime);

	~cAlien();

};
#endif