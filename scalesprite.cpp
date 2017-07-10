#include <cmath>
#include "scalesprite.h"
#include "gamedata.h"
#include "frameFactory.h"

ScaleSprite::ScaleSprite(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Frame* frm):
  Drawable(n, pos, vel), 
  scale(Gamedata::getInstance().getRandFloat(
  					 Gamedata::getInstance().getXmlFloat(n+"/scale/min"),
  					 Gamedata::getInstance().getXmlFloat(n+"/scale/max"))),
  frame( frm ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{
	setVelocity(scale*getVelocity());
}

ScaleSprite::ScaleSprite(const string& n):
  Drawable(n,  
           Vector2f(Gamedata::getInstance().getXmlInt(n+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(n+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(n+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(n+"/speedY")) 
		   ),
  scale(Gamedata::getInstance().getRandFloat(
  					 Gamedata::getInstance().getXmlFloat(n+"/scale/min"),
  					 Gamedata::getInstance().getXmlFloat(n+"/scale/max"))),
  frame( FrameFactory::getInstance().getFrame(n,scale)),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ 
	setVelocity(scale*getVelocity());
}

ScaleSprite::ScaleSprite(const ScaleSprite& s) :
  Drawable(s), 
  scale(s.scale),
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

ScaleSprite& ScaleSprite::operator=(const ScaleSprite& rhs) {
  Drawable::operator=( rhs );
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

void ScaleSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

int ScaleSprite::getDistance(const ScaleSprite *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void ScaleSprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

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
