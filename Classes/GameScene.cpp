#include "GameScene.h"
#include "SimpleAudioEngine.h"

#include "Constants.h"



USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //For now we set this manually
    PlatformType = type::grass;
    
    // add "Backgrounds" they go one in top of the other one in layers
    auto bkg1 = Sprite::create("bg_layer1.png");
    auto bkg2 = Sprite::create("bg_layer2.png");
    auto bkg3 = Sprite::create("bg_layer3.png");
    auto bkg4 = Sprite::create("bg_layer4.png");
    
    // position the sprite on the center of the screen
    bkg1->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    bkg2->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    bkg3->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    bkg4->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(bkg1, 0);
    this->addChild(bkg2, 0);
    this->addChild(bkg3, 0);
    this->addChild(bkg4, 0);
    
    //Load character from spritecache
    Character* character = Character::create(gender::male);
    character->setAnchorPoint(Vec2(0.5, 0.5));
    character->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    this->addChild(character,5);
    
    //Load a platform as test right under character
    Platform* p = Platform::create(PlatformType, size::normal, state::normal);
    p->setAnchorPoint(Vec2(0.5, 0.5));
    p->setPosition(Vec2(character->getPositionX(),character->getPositionY()-character->getContentSize().height));
    
    this->addChild(p,1);
    
    Platform* p2 = Platform::create(PlatformType, size::small, state::normal);
    p2->setAnchorPoint(Vec2(0.5, 0.5));
    p2->setPosition(Vec2(character->getPositionX(),character->getPositionY()-character->getContentSize().height+100));
    
    this->addChild(p2,1);
    
    return true;
}

void instantiateInitialPlatforms () {
    
    //This is a group always the same of platforms so player can never fall at the begining
    
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
