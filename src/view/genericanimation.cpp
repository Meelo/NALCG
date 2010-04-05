#include "genericanimation.h"

#include "bufferedinputhandler.h"

int GenericAnimation::id = 1;

GenericAnimation::~GenericAnimation()
{
    if (mCallback)
    {
        mCallback->animationFinished();
    }
}
