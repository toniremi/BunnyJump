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
            _sprite = "bunny1_jump.png";
            break;
        case gender::female:
            _sprite = "bunny2_jump.png";
            break;
        default:
            _sprite = "bunny1_jump.png";
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
    //Assign the gender here used later on initWithOptions
    pSprite->_gender = gr;
    
    if (pSprite && pSprite->initWithSpriteFrame(frame))
    {
        pSprite->autorelease();
        
        pSprite->setSpriteFrame(frame);
        
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
    
    switch (_gender) {
        case gender::male:
            genderNum = 1;
            break;
        case gender::female:
            genderNum = 2;
            break;
        default:
            genderNum = 1;
            break;
    }
    
    //Character gets tag 1
    setTag(1);
    
    //Set the Name
    setName("Character");
    
    //Set anchor point to be in the middle
    setAnchorPoint(Vec2(0.5f, 0.5f));
    
    //Set the physics body
    auto physicsBody = PhysicsBody::createBox(Size(this->getContentSize().width,this->getContentSize().height),PhysicsMaterial(0.1f, 1.0f, 0.0f));
    
    //Set dynamic so it falls and w can apply forces to it.
    physicsBody->setDynamic(true);
    
    //Set collisions
    physicsBody->setCategoryBitmask(0x02);    // 0010
    //physicsBody->setCollisionBitmask(0x01);   // 0001
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    
    physicsBody->setRotationEnable(false);
    
    //Add the physics body
    this->addComponent(physicsBody);
    
    //Create an animation for the character for jumping
    //We will do a 4 frames animation as follows : stand => ready => stand => jump and keep jump until touching something
    char buff[100] = {0};
    //Generate the frames using the spritnf to change between female and male
    
    //Stand
    snprintf(buff, sizeof(buff), "bunny%d_stand.png", genderNum);
    auto frameStand = SpriteFrameCache::getInstance()->getSpriteFrameByName(buff);
    
    //Ready
    snprintf(buff, sizeof(buff), "bunny%d_ready.png", genderNum);
    auto frameReady = SpriteFrameCache::getInstance()->getSpriteFrameByName(buff);
    
    //Jump
    snprintf(buff, sizeof(buff), "bunny%d_jump.png", genderNum);
    auto frameJump = SpriteFrameCache::getInstance()->getSpriteFrameByName(buff);
    
    /*
     * Fix for the weird physics animation behaviour with SpriteFrameCache.
     **/
    //SpriteFrameCache does something really weird with the physic body when animating. We move the anchor point to 0,0 to avoid
    //the physics body from moving if the rect of the animated sprites have not the same size.
    frameStand->setAnchorPoint(Vec2(0, 0));
    frameReady->setAnchorPoint(Vec2(0, 0));
    frameJump->setAnchorPoint(Vec2(0, 0));
    
    //Now we just need to add them to animJump Frames
    Vector<SpriteFrame*> animJumpFrames;
    animJumpFrames.reserve(4);
    //Add the frames
    animJumpFrames.pushBack(frameStand);
    animJumpFrames.pushBack(frameReady);
    animJumpFrames.pushBack(frameStand);
    animJumpFrames.pushBack(frameJump);
    
    //Create the animation that we will play
    jumpAnim = Animation::createWithSpriteFrames(animJumpFrames, 0.05f);
    //The Animate object has autorelease so we need to retain it if we want to execute it later on
    jumpAnim->retain();

    //adds contact event listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Character::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
}

//isDead
//Tells other clases if the character is dead
bool Character::isDead() {
    //Tell others if character is dead. Useful for movement for example
    return dead;
}

//Die
//This methods kills the player and makes an animation. Also loads next scene , the game over scene.
void Character::die() {
    CCLOG("Player is DEAD");
    //Mark as dead
    dead = true;
     char sprite[100] = {0};
    //As animation we will change sprite and apply a short impulse up
     snprintf(sprite, sizeof(sprite), "bunny%d_hurt.png", genderNum);
    this->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(sprite));
    
    //Finally add the impulse up
    Vec2 vel = this->getPhysicsBody()->getVelocity();
    vel.y = kNormalJumpForce;//upwards - don't change x velocity
    this->getPhysicsBody()->setVelocity(vel);
}

//Jump
//This mthod makes the character jump automatically when touching a valid object like platforms , certain enemies or a spring
void Character::jump() {
    if(!dead) {
        //Animate the character
        //We dont really have a big spritesheet of the character being animated so we will use actions and time outs
        //to change some sprites to give it some life
        runAction(Animate::create(jumpAnim));
        
        //Apply velocity
        Vec2 vel = this->getPhysicsBody()->getVelocity();
        vel.y = kNormalJumpForce;//upwards - don't change x velocity
        this->getPhysicsBody()->setVelocity(vel);
    }
}

//Collision detection
//Platforms have tag 2 for normal and tag 3 for broken
bool Character::onContactBegin(cocos2d::PhysicsContact& contact)
{
    //If its dead ignore everything
    if(dead) {
        return false;
    }
    
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    
    //by default its fallin
    bool makeContact = true;
    
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
                makeContact = true;
                //Make the character jump and animate it
                this->jump();
            } else {
                //We dont do anything and let it go through
                makeContact = false;
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
                makeContact = true;
                //Make the character jump and animate it
                this->jump();
            } else {
                //We dont do anything and let it go through
                makeContact = false;
            }
        } else if (nodeA->getName() == "Character" && nodeB->getName() == "Spikes") {
            
            //Pass through spikes no matter direction
            makeContact = false;
            
            //Checking for spikes
            Vec2 vel = nodeA->getPhysicsBody()->getVelocity();
            //If velocity its negative as faling then apply impulse if not just let it alone
            if(vel.y < 0 && nodeB->getTag() == kTopSpikeTag) {
                //Falling on spikes
                this->die();
            } else if(vel.y > 0 && nodeB->getTag() == kBottomSpikeTag) {
                //Hitting on spikes on jump
                this->die();
            }
        } else if(nodeB->getName() == "Character" && nodeA->getName() == "Spikes") {
            //Pass through spikes no matter direction
            makeContact = false;
            
            //Checking for spikes
            Vec2 vel = nodeB->getPhysicsBody()->getVelocity();
            //If velocity its negative as faling then apply impulse if not just let it alone
            if(vel.y < 0 && nodeA->getTag() == kTopSpikeTag) {
                //Falling on spikes
                this->die();
            } else if(vel.y > 0 && nodeA->getTag() == kBottomSpikeTag){
                //Hitting on spikes on jump
                this->die();
            }
        }
    }
    
    //bodies can collide
    return makeContact;
}
