//
//  GameSingleton.cpp
//  BunnyJump
//
//  Created by Antoni Remeseiro on 4/2/17.
//
//

#include "GameSingleton.hpp"

USING_NS_CC;

int GameSingleton::getGameScore()
{
    return this->score;
}

void GameSingleton::setGameScore(int latestScore)
{
    this->score = latestScore;
}
