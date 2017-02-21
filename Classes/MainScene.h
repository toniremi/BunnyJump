//
//  MainScene.hpp
//  BunnyJump
//
//  Created by Anthony on 2/17/17.
//
//

#ifndef MainScene_h
#define MainScene_h

#include "cocos2d.h"

class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // Play Button callback
    void PlayItemCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
};
#endif /* MainScene_hpp */
