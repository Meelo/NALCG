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
        mView(view), mParticleNode(0), mPromotionDuration(PROMOTION_DURATION),
        mWhiteFlashNode(0), transformed(false)
    {
    }

    virtual ~PromotionAnimation()
    {
    }

    virtual bool animate(const Real& timeSinceLastFrame);
    virtual char promotionToSymbol(unsigned int promoteTo) const;
    virtual void createBlasts();
    virtual void createWhiteFlash();

protected:
    static const int PROMOTION_DURATION = 4;
    unsigned int mPromoteTo;
    View* mView;
    SceneNode* mParticleNode;
    double mPromotionDuration;
    SceneNode* mWhiteFlashNode;
    bool transformed;

};

#endif // _NALCG_PROMOTION_ANIMATION_H_
