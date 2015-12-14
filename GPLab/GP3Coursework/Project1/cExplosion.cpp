#include "cExplosion.h"

cExploison::cExploison() : cModel()
{

}

cExploison::~cExploison()
{

}

void cExploison::update(float elapsedTime)
{
	deleteTime -= 1;
	if (deleteTime <= 0) cExploison::setIsActive(false);
}
