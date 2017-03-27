//
//  Platform.cpp
//  BunnyJump
//
//  Created by Antoni Remeseiro on 2/20/17.
//
//

#include "Platform.hpp"

using namespace cocos2d;
//Modified the constructor a little bit so we can carry the properties of this platform from Create
Platform::Platform() {}

Platform::~Platform() {}

//Just pass the gender we construct the sprite accoding to that gender passed.
Platform* Platform::create(type t, size s, state st)
{
    //Create our platform object
    Platform *pSprite = new Platform();

    //Build the sprite name
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
    
    if (pSprite && pSprite->initWithSpriteFrame(frame))
    {
        pSprite->autorelease();
        
        //Init with some options
        pSprite->initOptions();
        
        //pSprite->scheduleUpdate(); // runs update()
    
        return pSprite;
    }
    
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

//Init with options
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
//This method will basically add some decoration into the platform (grass , cactus, mushrooms,...)
void Platform::AddDecorations() {
    CCLOG("Size %d",static_cast<int>(_size));
    //Depending on the type and size we will add one or more things
    if(_size == size::small) {
        //We should only add one object
        if(_type == type::grass) {
            //Decoration can be grass only or a mushroom
            if(RandomHelper::random_int(1, 2) == 1) {
                //Grass
                char msg[256] = {0};
                sprintf(msg, "grass%d.png", RandomHelper::random_int(1, 2));
                auto decor = Sprite::createWithSpriteFrameName(msg);
                decor->setAnchorPoint(Vec2(0.5, 0));
                decor->setPosition(Vec2(RandomHelper::random_real( 0+decor->getContentSize().width, this->getContentSize().width-decor->getContentSize().width),this->getContentSize().height));
                this->addChild(decor,1);
            } else {
                //Mushrooms
                if(RandomHelper::random_int(1, 2) == 1) {
                    //Red Mushroom
                    auto decor = Sprite::createWithSpriteFrameName("mushroom_red.png");
                    decor->setAnchorPoint(Vec2(0.5, 0));
                    decor->setPosition(Vec2(RandomHelper::random_real( 0+decor->getContentSize().width, this->getContentSize().width-decor->getContentSize().width),this->getContentSize().height));
                    this->addChild(decor,1);
                } else {
                    //Red Mushroom
                    auto decor = Sprite::createWithSpriteFrameName("mushroom_brown.png");
                    decor->setAnchorPoint(Vec2(0.5, 0));
                    decor->setPosition(Vec2(RandomHelper::random_real( 0+decor->getContentSize().width, this->getContentSize().width-decor->getContentSize().width),this->getContentSize().height));
                    this->addChild(decor,1);
                }
            }
        } else if(_type == type::sand) {
            if(RandomHelper::random_int(1, 2) == 1) {
                //Grass
                char msg[256] = {0};
                sprintf(msg, "grass_brown%d.png", RandomHelper::random_int(1, 2));
                auto decor = Sprite::createWithSpriteFrameName(msg);
                decor->setAnchorPoint(Vec2(0.5, 0));
                decor->setPosition(Vec2(RandomHelper::random_real( 0+decor->getContentSize().width, this->getContentSize().width-decor->getContentSize().width),this->getContentSize().height));
                this->addChild(decor,1);
            }
        }
    } else {
        //We can add anything and even more than one. We will do groupsof predefined things to add but position will be random.
        if(_type == type::grass) {
            //Decoration can be grass only or a mushroom
            if(RandomHelper::random_int(1, 2) == 1) {
                //Grass Normal (Green)
                char msg[256] = {0};
                sprintf(msg, "grass%d.png", RandomHelper::random_int(1, 2));
                auto decor = Sprite::createWithSpriteFrameName(msg);
                decor->setAnchorPoint(Vec2(0.5, 0));
                decor->setPosition(Vec2(RandomHelper::random_real( 0+decor->getContentSize().width, this->getContentSize().width-decor->getContentSize().width),this->getContentSize().height));
                this->addChild(decor,1);
            } else {
                //Mushrooms
                if(RandomHelper::random_int(1, 2) == 1) {
                    //Red Mushroom
                    auto decor = Sprite::createWithSpriteFrameName("mushroom_red.png");
                    decor->setAnchorPoint(Vec2(0.5, 0));
                    decor->setPosition(Vec2(RandomHelper::random_real( 0+decor->getContentSize().width, this->getContentSize().width-decor->getContentSize().width),this->getContentSize().height));
                    this->addChild(decor,1);
                } else {
                    //Red Mushroom
                    auto decor = Sprite::createWithSpriteFrameName("mushroom_brown.png");
                    decor->setAnchorPoint(Vec2(0.5, 0));
                    decor->setPosition(Vec2(RandomHelper::random_real( 0+decor->getContentSize().width, this->getContentSize().width-decor->getContentSize().width),this->getContentSize().height));
                    this->addChild(decor,1);
                }
            }
        } else if(_type == type::sand) {
            if(RandomHelper::random_int(1, 2) == 1) {
                //Grass Brown
                char msg[256] = {0};
                sprintf(msg, "grass_brown%d.png", RandomHelper::random_int(1, 2));
                auto decor = Sprite::createWithSpriteFrameName(msg);
                decor->setAnchorPoint(Vec2(0.5, 0));
                decor->setPosition(Vec2(RandomHelper::random_real( 0+decor->getContentSize().width, this->getContentSize().width-decor->getContentSize().width),this->getContentSize().height));
                this->addChild(decor,1);
            } else {
                //Cactus
                auto decor = Sprite::createWithSpriteFrameName("cactus.png");
                decor->setAnchorPoint(Vec2(0.5, 0));
                decor->setPosition(Vec2(RandomHelper::random_real( 0+decor->getContentSize().width, this->getContentSize().width-decor->getContentSize().width),this->getContentSize().height));
                this->addChild(decor,1);
            }
        }
    }
}

void Platform::AddSpikes() {
    
    //Set as containing traps
    containsTraps = true;
    
    //This will hold the name of the sprite there are two for one sike or group of spikes
    String spike = "";
    if(RandomHelper::random_int(1, 2) == 1) {
        spike = "spike";
    } else {
        spike = "spikes";
    }
    
    //Here we decide if they go on top or bottom
    //If called will add spikes on the platform
    //The x will decide if they go on top or bottom
    int y = RandomHelper::random_int(1, 2);
    if(y==1) {
        //Top
        std::string sprite = spike.getCString();
        sprite += "_top.png";
        
        //Create Sprite
        auto trap = Sprite::createWithSpriteFrameName(sprite);
        //Anchor
        trap->setAnchorPoint(Vec2(0.5, 0));
        //Position it inside the platform
        float posY = this->getContentSize().height-3.5f;
        float min = trap->getContentSize().width/2;
        float max = this->getContentSize().width-(trap->getContentSize().width/2);
        float posX = RandomHelper::random_real(min, max);
        //Set position
        trap->setPosition(Vec2(posX,posY));
        //Add name
        trap->setName("Spikes");
        //Add tag for identification on collision
        trap->setTag(kTopSpikeTag);
        //Set physicis body
        auto physicsBody = PhysicsBody::createBox(Size(trap->getContentSize().width,trap->getContentSize().height),
                                                  PhysicsMaterial(0.1f, 1.0f, 0.0f));
        physicsBody->setDynamic(false);
        
        physicsBody->setCategoryBitmask(0x01);    // 0010
        physicsBody->setContactTestBitmask(0xFFFFFFFF);
        
        //Add the physics body
        trap->addComponent(physicsBody);
        
        //Add it to the platform. or asthetics reasons we add it behind platform.
        this->addChild(trap,-1);
        
    } else {
        //Bottom
        std::string sprite = spike.getCString();
        sprite += "_bottom.png";
        
        //Create Sprite
        auto trap = Sprite::createWithSpriteFrameName(sprite);
        //Anchor
        trap->setAnchorPoint(Vec2(0.5, 1));
        //Position it inside the platform
        float posY = 3.5f;
        float min = trap->getContentSize().width/2;
        float max = this->getContentSize().width-(trap->getContentSize().width/2);
        float posX = RandomHelper::random_real(min, max);
        //Set position
        trap->setPosition(Vec2(posX,posY));
        //Add name
        trap->setName("Spikes");
        //Add tag for identification in collision
        trap->setTag(kBottomSpikeTag);
        //Set physicis body
        auto physicsBody = PhysicsBody::createBox(Size(trap->getContentSize().width,trap->getContentSize().height),
                                                  PhysicsMaterial(0.1f, 1.0f, 0.0f));
        physicsBody->setDynamic(false);
        
        physicsBody->setCategoryBitmask(0x01);    // 0010
        physicsBody->setContactTestBitmask(0xFFFFFFFF);
        
        //Add the physics body
        trap->addComponent(physicsBody);
        
        this->addChild(trap,-1);
    }
}

void Platform::AddWalkingEnemy() {
    //Add an enemy that kind of patrolls the platform
    
}

void Platform::AddSpring() {
    //Adds a spring that boosts our jump
}

