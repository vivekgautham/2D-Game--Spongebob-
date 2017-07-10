#ifndef BITERMULTISPRITE__H
#define BITERMULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"
#include "sprite.h"
#include "multisprite.h"
#include "gamedata.h"

class BiterMultiSprite : public MultiSprite {
public:
  BiterMultiSprite(const std::string&);
  virtual ~BiterMultiSprite() { } 

//  virtual void draw() const;
  virtual void update(Uint32 ticks);
  
protected:
  BiterMultiSprite& operator=(const BiterMultiSprite&);
  BiterMultiSprite(const BiterMultiSprite&);
  void advanceFrame(Uint32 ticks);
};
#endif
