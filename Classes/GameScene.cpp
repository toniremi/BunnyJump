#include "GameScene.h"
#include "SimpleAudioEngine.h"

#include "Constants.h"

#include <stdlib.h> //For rand()

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    //Increase speed to make things fel snapier
    scene->getPhysicsWorld()->setSpeed(2.0f);
    
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
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    //For now we set this manually
    PlatformType = type::grass;
    
    //Initialize minPlatformModifier so we can increase the step of the platforms regularly
    minPlatformModifier = kMinPlatformStep;
    
    //Create the nodes here
    
    //Level
    level = Node::create();
    //Because level position is set at origin we do not need to use origin again in any object that will go inside level
    level->setPosition(Vec2(origin.x, origin.y));
    this->addChild(level,1);
    
    //Create the node platforms to hold all the platforms inside this is to locate them easyly
    platforms = Node::create();
    platforms->setPosition(Vec2(0,0));
    level->addChild(platforms,1);
    
    // add "Backgrounds" they go one in top of the other one in layers
    auto bkg1 = Sprite::create("bg_layer1.png");
    auto bkg2 = Sprite::create("bg_layer2.png");
    auto bkg3 = Sprite::create("bg_layer3.png");
    auto bkg4 = Sprite::create("bg_layer4.png");
    
    // position the sprite on the center of the screen
    bkg1->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    bkg2->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    bkg3->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    bkg4->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    
    // add the sprite as a child to this layer
    this->addChild(bkg1, 0); //Add backround in the normal node so always stays
    //This go in level so they will move with it creating the illusion of moving up
    level->addChild(bkg2, 0);
    level->addChild(bkg3, 0);
    level->addChild(bkg4, 0);
    
    //Load character from spritecache
    character = Character::create(gender::male);
    character->setAnchorPoint(Vec2(0.5, 0.5));
    character->setPosition(Vec2(visibleSize.width/2 , visibleSize.height*0.15));
    
    level->addChild(character,5);
    
    //Create UI
    scoreLabel = Label::createWithTTF("0m", "fonts/KenVector Future.ttf", 32);
    
    scoreLabel->setAnchorPoint(Vec2( 0.5, 0.5 ));
    
    // position the label on the center of the screen
    scoreLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height*0.95));
    
    scoreLabel->setTextColor(Color4B::WHITE);
    scoreLabel->enableOutline(Color4B(67,67,67,255),2);
    
    // add the label as a child to this layer
    this->addChild(scoreLabel, 15);
    
    //Load the first batch of platforms
    instantiateInitialPlatforms();
    
    //Adding touch detector
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->setSwallowTouches(true);
    
    //Event for touch began
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    //Event for touch ended
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    
    //Adding listener to events dispatcher
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //Schedule the update
    this->scheduleUpdate();
    
    return true;
}

void GameScene::instantiateInitialPlatforms () {
    
    //Always create one platform beneath the player for an easy start
    Platform* p = Platform::create(PlatformType, size::normal, state::normal);
    p->setAnchorPoint(Vec2(0.5, 0.5));
    
    //Add the tag to identify the platforms tag will start at 50 until 50 + knumPlatforms (as of now 60).
    //So that means that first platform and the lowest one has tag 50 , next one tag 51 ,...
    //This will allow us to recycle platforms by looking at currentPlatformTag which will hold the next platform to be deleted
    p->setTag(kInitialPlatformTag);
    
    //Position the first one right beneath the hero to make it an easy start
    p->setPosition(Vec2(character->getPositionX()-origin.x,character->getPositionY()-character->getContentSize().height-origin.y));
    
    //Save this for next platform
    currentPlatformTag = kInitialPlatformTag;
    targetPlatformToReset = kInitialPlatformTag;
    lastPlatformYPosition = p->getPositionY();
    
    //Add to platfroms node
    platforms->addChild(p,0);
    
    //This is a group always the same of platforms so player can never fall at the begining
   while(currentPlatformTag < (kInitialPlatformTag+kNumPlatforms) ) {
       instantiatePlatform();
   }
}

void GameScene::instantiatePlatform() {
    
    //Here at some point we will randomize other stuff like the size and state of the platform
    
    //Also at some point we may also add enemies on top of this platforms that walk on them or just make them spiked
    
    //Create platforms
    size platformSize = size::normal;
    
    //Here we will add different random elements to start randomizing and increasing difficulty of level
    if(score>500) {
        //This from time to time therell bee a small platform
        int p = RandomHelper::random_int(1, 3); //Modifie the max for less chance or more chance
        if( p == 1) {
            platformSize = size::small;
        }
    }
    
    //After 1000 we will start increasing the distance between platform to a limit
    if(score > 1000) {
        //Increase the min step by Platfrom Step Increase constant
        minPlatformModifier = minPlatformModifier + kPlatformStepIncrease;
        
        //Control that the min step doesnt surpas the max step
        if(minPlatformModifier > (kMaxPlatformStep-kMinPlatformStep)) {
            //If its bigger we will do minPlatformModifier to be kMaxStep - kMinStep (200-50=150) so now platforms will be between 200 and 150
            minPlatformModifier = (kMaxPlatformStep-kMinPlatformStep);
        }
    }
    
    //Create the platform with whatever parameters we get
    Platform* p = Platform::create(PlatformType, platformSize, state::normal);
    //Add properties of the platform.
    p->setType(PlatformType);
    p->setSize(platformSize);
    p->setState(state::normal);
    
    //Anchor point
    p->setAnchorPoint(Vec2(0.5, 0.5));
    
    state thestate = p->getState();
    CCLOG("State %d",static_cast<int>(thestate));
    
    //Set the tag. The tags will be rotating among the platforms so
    int tag = currentPlatformTag+1;
    //if(tag > kInitialPlatformTag+kNumPlatforms) {
        //tag = kInitialPlatformTag;
    //}
    p->setTag(tag); //Increase the tag
    //Random positioning helper variables
    
    //We add decoration
    if(RandomHelper::random_int(1, 3) == 1) {
        p->AddDecorations();
    }
    
    //Get a position being x random in screen and y random inside a certain margin so we can always reach
    float randX = RandomHelper::random_real((p->getContentSize().width/2), (visibleSize.width - p->getContentSize().width/2));
    //This is the separation between this platform and last one instantiated we need to control this so always can be reached
    float randYRange = RandomHelper::random_real((float)minPlatformModifier, (float)kMaxPlatformStep);
    //Position it on top of last platform with the increased step
    float randY = lastPlatformYPosition + randYRange;
    
    //Set the new position
    p->setPosition(Vec2(randX, randY));
    
    //Save info for next platform
    currentPlatformTag = p->getTag();
    lastPlatformYPosition = p->getPositionY();
    
    //finally add it to platforms node
    platforms->addChild(p,0);

}


void GameScene::update(float dt)
{
    //Character movement
    //If gets to the edge spawns on the other side of the screen
    if(character->getPositionX() < 0 && character->getPhysicsBody()->getVelocity().x < 0) {
        //Character is moving left and passed the left edge so spawn on the other side
        Vec2 pos = character->getPosition();
        pos.x = visibleSize.width; //The coordinate of the other side of the scree
        character->setPosition(pos);
    } else if(character->getPositionX() > visibleSize.width && character->getPhysicsBody()->getVelocity().x > 0) {
        //Character moving to the right and left the edge so spawn on the other side
        Vec2 pos = character->getPosition();
        pos.x = 0; //The coordinate of the other side of the scree
        character->setPosition(pos);
    }
    
    //Get the worlspace for character inside the node level to get its position relative to the screen
    Vec2 characterPos;
    characterPos = level->convertToWorldSpace(character->getPosition());
    //CCLOG("Char Position %f",characterPos.y);
    
    //Move node down if character avobe half screen
    if(characterPos.y > visibleSize.height/2) {
        Vec2 pos = level->getPosition();
        pos.y -= ((float)scrollSpeed);
        level->setPosition(pos);
    }
    
    //Score
    score = std::abs(level->getPositionY()/3);
    score *= 1; //To make it positive
    char s[100] = {0};
    sprintf(s, "%im", (int) score);
    scoreLabel->setString (s);
    
    //If characterpos.y its < 0 means the character fell on the screen so game over
    if(characterPos.y < 0) {
        CCLOG("GAME OVER");
    }
    
    //Go trhough the platforms and respawn platforms out of the screen at the bottom for newer ones on top
    for(const auto &child : platforms->getChildren())
    {
        //Get position relative to screen if its negative we need to reset that platform
        Vec2 platformPos;
        platformPos = platforms->convertToWorldSpace(child->getPosition());
        
        //To avoid poping effect (things dissapeareing in the screen) i added to the y the height of the platform.
        //This way we make sure we do not delet the platform until the platform its not on the screen. I added ful heigh to have more room.
        if((platformPos.y+child->getContentSize().height) < 0) {
            //Platform need to be reseted
            int platformTag = child->getTag();
            
            //Remove this chilg and instantiate a new platform
            child->removeFromParentAndCleanup(true);
            
            //Instantiate the new platform
            instantiatePlatform();
            
        }
    }
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

#pragma mark - Touch Events
bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
    CCLOG("Touched");
    
    Vec2 p = touch->getLocation();
    
    if(p.x > visibleSize.width/2-origin.x) {
        //Get velocity Y and get float from constant
        float VelY = character->getPhysicsBody()->getVelocity().y;
        float velX = (float) kCharacterXVelocity;
        //Assign new velocity
        character->getPhysicsBody()->setVelocity(Vec2(velX, VelY));
    } else {
        //Get velocity Y and get float from constant
        float VelY = character->getPhysicsBody()->getVelocity().y;
        float velX = (float) kCharacterXVelocity;
        velX = velX * -1; //Negative X velocity to move to left
        //Assign new velocity
        character->getPhysicsBody()->setVelocity(Vec2(velX, VelY));
    }
    
    return true;
}

void GameScene::onTouchEnded(Touch* touch, Event* event) {
    
    CCLOG("Not touching");
    //no velocity on x so we just go up
    float VelY = character->getPhysicsBody()->getVelocity().y;
    character->getPhysicsBody()->setVelocity(Vec2(0, VelY));
    
}
