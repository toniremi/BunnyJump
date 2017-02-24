#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "Character.hpp"
#include "Platform.hpp"

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
    //Override Uploade method
    void update(float dt) override;
    
    //This is the same type we will pass to the platforms when creating them.
    //This is to set the aesthethic of the levels.
    type PlatformType;
    
private:
    
    //This is just ot make it easier to navigate through the platforms in the level
    Node* platforms;
    
    //Layers
    Node* Background;
    Node* BkgDetails;
    Node* level;
    Node* Menu;
    
    //A reference to the character created so we can access it through the class
    Character* character;
    
    int score;
    int platformsCount;
    int currentPlatformTag;
    float lastPlatformYPosition; //Holds the last platform y positon
    
    Size visibleSize;
    Vec2 origin;
    
private:
    //Methods
   
    //This will add the initial platforms
    void instantiateInitialPlatforms();
    //This is the heavy method that will create a random platform and position it in the node
    void instantiatePlatform();
    
    //Events for touch
    bool onTouchBegan (cocos2d::Touch * touch, cocos2d::Event * event);
    void onTouchEnded (cocos2d::Touch * touch, cocos2d::Event * event);
};

#endif // __HELLOWORLD_SCENE_H__
