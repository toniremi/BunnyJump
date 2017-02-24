//
//  Character.cpp
//  BunnyJump
//
//  Created by Anthony on 2/17/17.
//
//

#include "Character.hpp"
#include "Platform.hpp"
#include "Constants.h"

using namespace cocos2d;

Character::Character() {}

Character::~Character() {}

//Just pass the gender we construct the sprite accoding to that gender passed.
Character* Character::create(gender gr)
{
    std::string _sprite;
    switch (gr) {
        case gender::male:
            _sprite = "bunny1_stand.png";
            break;
        case gender::female:
            _sprite = "bunny2_stand.png";
            break;
        default:
            _sprite = "bunny1_stand.png";
            break;
    }
    
    //Get the spriteFrameFirst
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(_sprite);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", _sprite.c_str());
    CCASSERT(frame != nullptr, msg);
#endif

    Character *pSprite = new Character();
    
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

void Character::initOptions()
{
    // do things here like setTag(), setPosition(), any custom logic.
    
    //Character gets tag 1
    setTag(1);
    
    //Set the Name
    setName("Character");
    
    //Set the physics body
    auto physicsBody = PhysicsBody::createBox(Size(this->getContentSize().width,this->getContentSize().height),
                                              PhysicsMaterial(0.1f, 1.0f, 0.0f));
    physicsBody->setDynamic(true);
    
    //Set collisions
    physicsBody->setCategoryBitmask(0x02);    // 0010
    //physicsBody->setCollisionBitmask(0x01);   // 0001
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    
    physicsBody->setRotationEnable(false);
    
    //Add the physics body
    this->addComponent(physicsBody);
    
    //adds contact event listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Character::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

//Collision detection
//Platforms have tag 2 for normal and tag 3 for broken
bool Character::onContactBegin(cocos2d::PhysicsContact& contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    
    //by default its fallin
    bool falling = true;
    
    if (nodeA && nodeB)
    {
        if (nodeA->getName() == "Character" && nodeB->getName() == "Platform")
        {
            //Get the platform object
            Platform* p = (Platform*) nodeB;
            
            if(p->isBroken()) {
                //Its a broken platform so need to break it
                CCLOG("%s","Platform its broken so break it.");
            }
            
            //Apply Impulse to jump up if necessary
            Vec2 vel = nodeA->getPhysicsBody()->getVelocity();
            //If velocity its negative as faling then apply impulse if not just let it alone
            if(vel.y < 0) {
                //Falling
                falling = true;
                //Apply velocity
                vel.y = kNormalJumpForce;//upwards - don't change x velocity
                nodeA->getPhysicsBody()->setVelocity(vel);
            } else {
                //We dont do anything and let it go through
                falling = false;
            }
        }
        else if (nodeB->getName() == "Character" && nodeA->getName() == "Platform")
        {
            //Get the platform object
            Platform* p = (Platform*) nodeA;
            
            if(p->isBroken()) {
                //Its a broken platform so need to break it
                CCLOG("%s","Platform its broken so break it.");
            }
            
            //Apply Impulse to jump up if necessary
            Vec2 vel = nodeB->getPhysicsBody()->getVelocity();
            if(vel.y < 0) {
                //Falling
                falling = true;
                //Apply velocity
                vel.y = kNormalJumpForce;//upwards - don't change x velocity
                nodeB->getPhysicsBody()->setVelocity(vel);
            } else {
                //We dont do anything and let it go through
                falling = false;
            }
        }
    }
    
    //bodies can collide
    return falling;
}
