#ifndef BUBBLESPRITE__H
#define BUBBLESPRITE__H
#include <string>
#include <vector>
#include "drawable.h"
#include "scalesprite.h"
#include "gamedata.h"

class BubbleSprite : public ScaleSprite {
public:
  BubbleSprite(const std::string&);
  virtual ~BubbleSprite() { } 

  virtual void update(Uint32 ticks);
  virtual void rst();
  
protected:
  BubbleSprite& operator=(const BubbleSprite&);
  BubbleSprite(const BubbleSprite&);
  void distributeStartLocation();
};
#endif
