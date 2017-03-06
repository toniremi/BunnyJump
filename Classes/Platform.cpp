//
//  Platform.cpp
//  BunnyJump
//
//  Created by Antoni Remeseiro on 2/20/17.
//
//

#include "Platform.hpp"

using namespace cocos2d;

Platform::Platform() {}

Platform::~Platform() {}

//Just pass the gender we construct the sprite accoding to that gender passed.
Platform* Platform::create(type t, size s, state st)
{
    std::string _sprite = "ground_";
    //Type
    switch (t) {
        case type::cake:
            _sprite += "cake";
            break;
        case type::grass:
            _sprite += "grass";
            break;
        case type::sand:
            _sprite = "sand";
            break;
        case type::snow:
            _sprite += "snow";
            break;
        case type::stone:
            _sprite += "stone";
            break;
        case type::wood:
            _sprite += "wood";
            break;
        default:
            _sprite = "grass";
            break;
    }
    
    //Size
    switch (s) {
        case size::small:
            _sprite += "_small";
            break;
        case size::normal:
            //No add something just leave it
        default:
            break;
    }
    
    //State (Broken or not broken)
    switch (st) {
        case state::broken:
            _sprite += "_broken";
            break;
        case state::normal:
            //No add anything just leave
        default:
            break;
    }
    
    //Ad png format at the end
    _sprite += ".png";
    
    //Get the spriteFrameFirst
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(_sprite);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", _sprite.c_str());
    CCASSERT(frame != nullptr, msg);
#endif
    
    Platform *pSprite = new Platform();
    
    if (pSprite && pSprite->initWithSpriteFrame(frame))
    {
        pSprite->autorelease();
        
        pSprite->initOptions();
        
        //pSprite->scheduleUpdate(); // runs update()
        
        return pSprite;
    }
    
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

void Platform::initOptions()
{
    // do things here like setTag(), setPosition(), any custom logic.
    if(_state == state::broken) {
        setTag(3); //broken platforms are tage 3
    } else {
        setTag(2); //normal platforms are tag 2
    }
    
    //Set the name
    setName("Platform");
    
    //Set the physics body
    auto physicsBody = PhysicsBody::createBox(Size(this->getContentSize().width,this->getContentSize().height),
                                              PhysicsMaterial(0.1f, 1.0f, 0.0f));
    physicsBody->setDynamic(false);
    
    physicsBody->setCategoryBitmask(0x01);    // 0010
    //physicsBody->setCollisionBitmask(0x02);   // 0001
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    
    //Add the physics body
    this->addComponent(physicsBody);
}

state Platform::getState() {
    //Return the state of this platform
    return _state;
}

bool Platform::isBroken() {
    
    //Return true if its broken
    if(_state == state::broken) {
        return true;
    }
    
    //Return false if its not broken
    return false;
}

#pragma mark - Platform Addons

void Platform::AddSpikes() {
    //If called will add spikes on the platform
    
    //Here we decide if they go on top or bottom
    
}

void Platform::AddWalkingEnemy() {
    //Add an enemy that kind of patrolls the platform
    
}

void Platform::AddSpring() {
    //Adds a spring that boosts our jump
}

