#include "jellymultisprite.h"
#include "explodingSprite.h"

JellyMultiSprite::JellyMultiSprite( const std::string& name) : MultiSprite(name)
{}


void JellyMultiSprite::advanceFrame(Uint32 ticks) {
	int offset;
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
		if (getVelocity()[0] > 0)
			offset = 0;
		else if (getVelocity()[0] == 0)
			if (currentFrame < numberOfFrames/2)
				offset = 0;
			else
				offset = numberOfFrames/2;
		else
			offset = numberOfFrames/2;
    currentFrame = offset + ((currentFrame+1) % (numberOfFrames/2));
		timeSinceLastFrame = 0;
	}
}


void JellyMultiSprite::update(Uint32 ticks) { 
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
    velocityY(0);
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY(0);
  }

  if ( X() < 0) {
    velocityX(0);
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX(0);
  }
}

