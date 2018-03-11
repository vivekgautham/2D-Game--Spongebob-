#include "bubblesprite.h"
#include <cstdlib>
#include <cmath>

BubbleSprite::BubbleSprite( const std::string& name) : ScaleSprite(name) {
	distributeStartLocation();
}

void BubbleSprite::rst(){
	distributeStartLocation();
}


void BubbleSprite::distributeStartLocation(){
  Vector2f randIncrPos(Gamedata::getInstance().getRandInRange(0,worldWidth),0);
  setPosition(getPosition() + randIncrPos);
}

void BubbleSprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  if ( Y() < 0) {
  Vector2f randPos(Gamedata::getInstance().getRandInRange(0,worldWidth), Gamedata::getInstance().getXmlInt("bubble/startLoc/y"));
  setPosition(randPos);
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -std::abs( velocityY() ) );
  }

}

