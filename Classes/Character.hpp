//
//  Character.hpp
//  BunnyJump
//
//  Created by Anthony on 2/17/17.
//
//

#ifndef Character_hpp
#define Character_hpp

#include "cocos2d.h"

using namespace cocos2d;

// Gender of the character may be gender::male or gender::female
enum class gender: char
{
    male='m',
    female='f', // C++11 allows the extra comma
};

class Character : public cocos2d::Sprite
{
public:
    //Variables
    
    //Methods
    Character();
    ~Character();
    
    static Character* create(gender gr);
   
    void initOptions();
    
private:
    //Variables
    gender _gender;
    
    //Methods
    bool onContactBegin(PhysicsContact& contact);
};

#endif /* Character_hpp */
