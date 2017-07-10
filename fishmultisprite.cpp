#include "fishmultisprite.h"
#include "explodingSprite.h"

FishMultiSprite::FishMultiSprite( const std::string& name) : ScaleMultiSprite(name) {
	distributeStartLocation();
}

void FishMultiSprite::advanceFrame() {
	if (getVelocity()[0] > 0)
		currentFrame = 0;
	else
		currentFrame = 1;
}


void FishMultiSprite::rst(){
	distributeStartLocation();
}


void FishMultiSprite::distributeStartLocation(){
  Vector2f startPos((Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x")),(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y")));
  Vector2f randIncrPos((Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"))/scale,(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y"))*scale);
  setPosition(startPos + randIncrPos);
}


void FishMultiSprite::update(Uint32 ticks) { 
	if ( explosion ) {
     explosion->update(ticks);
     if ( explosion->chunkCount() == 0 ) {
          delete explosion;
           explosion = NULL;
      }
       return;
    }

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  advanceFrame();
  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }
}

