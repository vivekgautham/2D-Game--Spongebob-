#include "bitermultisprite.h"
#include "explodingSprite.h"

BiterMultiSprite::BiterMultiSprite( const std::string& name) : MultiSprite(name) {}


void BiterMultiSprite::advanceFrame(Uint32 ticks) {
	int offset;
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
		if (getVelocity()[0] > 0)
			offset = 0;
		else 
			offset = numberOfFrames/2;
    currentFrame = offset + ((currentFrame+1) % (numberOfFrames/2));
		timeSinceLastFrame = 0;
	}
}


void BiterMultiSprite::update(Uint32 ticks) { 
	if ( explosion ) {
     explosion->update(ticks);
     if ( explosion->chunkCount() == 0 ) {
          delete explosion;
           explosion = NULL;
      }
       return;
    }

  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( std::abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -std::abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( std::abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -std::abs( velocityX() ) );
  }
}

