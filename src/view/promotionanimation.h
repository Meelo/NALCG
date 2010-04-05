#ifndef _NALCG_PROMOTION_ANIMATION_H_
#define _NALCG_PROMOTION_ANIMATION_H_

#include "genericanimation.h"

class View;
class PromotionAnimation : public GenericAnimation
{
public:
    PromotionAnimation(unsigned int promoteTo, SceneNode *promotingNode,
        SceneManager *sceneMgr, View* view)
        : GenericAnimation(promotingNode, sceneMgr), mPromoteTo(promoteTo),
        mView(view)
    {
    }

    virtual ~PromotionAnimation()
    {
    }

    virtual bool animate(const Real& timeSinceLastFrame);
    virtual char promotionToSymbol(unsigned int promoteTo) const;

protected:
    unsigned int mPromoteTo;
    View* mView;

};

#endif // _NALCG_PROMOTION_ANIMATION_H_
