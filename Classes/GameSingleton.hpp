//
//  GameSingleton.hpp
//  BunnyJump
//
//  Created by Antoni Remeseiro on 4/2/17.
//
//

#ifndef GameSingleton_hpp
#define GameSingleton_hpp

#include <stdio.h>
class GameSingleton {
public:
    static GameSingleton *getInstance() {
        static GameSingleton *instance = new GameSingleton();
        return instance;
    }
    
    //Any public method or variable here
    
    // Game Play
    int getGameScore();
    void setGameScore(int latestScore);
    
private:
    GameSingleton() {}
    int score;
};
#endif /* GameSingleton_hpp */
