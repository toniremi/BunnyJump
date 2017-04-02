//
//  GameOverScene.cpp
//  BunnyJump
//
//  Created by Antoni Remeseiro on 4/2/17.
//
//

#include "GameOverScene.hpp"

#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

//Game Scene becaus we will load it
#include "GameScene.h"

USING_NS_CC;

Scene* GameOverScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameOverScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Game Over", "fonts/KenVector Bold.ttf", 46);
    
    label->setAnchorPoint(Vec2( 0.5, 0.5 ));
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height*0.75));
    
    label->setTextColor(Color4B::WHITE);
    label->enableOutline(Color4B(67,67,67,255),4);
    label->enableShadow();
    
    // add the label as a child to this layer
    this->addChild(label, 1);
    
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
    
    //Play Button now
    auto Playbutton = cocos2d::ui::Button::create("green_button00.png", "green_button01.png", "grey_button00.png");
    Playbutton->setTitleText("Replay");
    Playbutton->setTitleFontName("fonts/KenVector Future.ttf");
    Playbutton->setTitleFontSize(24);
    Playbutton->setPosition(Vec2(origin.x + visibleSize.width/2 , origin.y + visibleSize.height/2));
    Playbutton->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                Director::getInstance()->replaceScene(TransitionFade::create(0.5, GameScene::createScene(), Color3B(67,67,67)));
                break;
            default:
                break;
        }
    });
    this->addChild(Playbutton,1);
    
    return true;
}


void GameOverScene::PlayItemCallback(Ref* pSender)
{
    
    
}
