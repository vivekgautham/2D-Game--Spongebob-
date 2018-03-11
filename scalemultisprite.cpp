#include "scalemultisprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingSprite.h"
#include <cstdlib>
#include <cmath>

ScaleMultiSprite::~ScaleMultiSprite() {
	if(explosion)
		delete explosion;
}

void ScaleMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}


ScaleMultiSprite::ScaleMultiSprite( const std::string& name, 
                          const std::vector<Frame*>& f) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  explosion(NULL),
  scale(Gamedata::getInstance().getRandFloat(
  					 Gamedata::getInstance().getXmlFloat(name+"/scale/min"),
  					 Gamedata::getInstance().getXmlFloat(name+"/scale/max"))),
  frames( f ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }

ScaleMultiSprite::ScaleMultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  explosion(NULL),
  scale(Gamedata::getInstance().getRandFloat(
  					 Gamedata::getInstance().getXmlFloat(name+"/scale/min"),
  					 Gamedata::getInstance().getXmlFloat(name+"/scale/max"))),
  frames( FrameFactory::getInstance().getFrames(name,scale) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ 
	setVelocity(scale*getVelocity());
}


void ScaleMultiSprite::draw() const { 
  if (explosion) {
	  explosion->draw();
	  return;
  }
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void ScaleMultiSprite::explode() {
	if (explosion) return;
	Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
    explosion = new ExplodingSprite(sprite);
}

void ScaleMultiSprite::update(Uint32 ticks) { 
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
//    advanceFrame();
  }
}
