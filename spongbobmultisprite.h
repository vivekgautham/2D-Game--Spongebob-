#ifndef SPONGMULTISPRITE__H
#define SPONGMULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"
#include "multisprite.h"
#include "sprite.h"
#include "gamedata.h"

class SpongbobMultiSprite : public MultiSprite {
public:
  SpongbobMultiSprite(const std::string&);
  virtual ~SpongbobMultiSprite() { } 

  void explode(){
//	  trap.setVelocity(Vector2f(30,30));
	  trap.explode();
	  isTrapped = false;
  }
  
  virtual void rst() {
        setPosition(Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),Gamedata::getInstance().getXmlInt(name+"/startLoc/y")));
		isTrapped = true;
	}

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  
protected:
  SpongbobMultiSprite& operator=(const SpongbobMultiSprite&);
  SpongbobMultiSprite(const SpongbobMultiSprite&);
  void advanceFrame(Uint32 ticks);

private:
  std::string name;
  Sprite spchbub;
  Sprite trap;
  bool isTrapped;
};
#endif
