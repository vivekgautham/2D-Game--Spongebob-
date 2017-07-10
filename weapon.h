#ifndef WEAPON_H
#define WEAPON_H

#include <iostream>
#include "multisprite.h"
#include "gamedata.h"

class Weapon : public MultiSprite {
public:
  explicit Weapon(const std::string& name,const Vector2f& pos, const Vector2f& vel) :
    MultiSprite(name, pos, vel), 
    distance(0), 
    maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")), 
    tooFar(false) 
  { }
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }
  void set(Vector2f pos,Vector2f vel) {
	  setPosition(pos);
	  setVelocity(vel);
  }
private:
  float distance;
  float maxDistance;
  bool tooFar;
};
#endif
