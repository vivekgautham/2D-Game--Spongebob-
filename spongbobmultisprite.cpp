#include "spongbobmultisprite.h"
#include <cstdlib>
#include <cmath>


SpongbobMultiSprite::SpongbobMultiSprite( const std::string& name) : 
	MultiSprite(name) ,
	name(name),
	spchbub(Gamedata::getInstance().getXmlStr(name+"/spchbub")),
	trap("trap"),
	isTrapped(true)
{
}


void SpongbobMultiSprite::advanceFrame(Uint32 ticks) {
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

void SpongbobMultiSprite::draw() const{
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
  if( x%y < 200 && isTrapped)
  	spchbub.draw();
  if(isTrapped || (trap.isExploding()))
    trap.draw();
}

void SpongbobMultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  Vector2f pos(20,-120);
  spchbub.setPosition(getPosition() + pos);
  if(isTrapped)
  	  trap.setPosition(getPosition() + Vector2f(0,-2));
  else
	  trap.update(ticks);

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

