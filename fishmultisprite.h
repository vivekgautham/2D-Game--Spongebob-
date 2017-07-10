#ifndef FISHMULTISPRITE__H
#define FISHMULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"
#include "scalemultisprite.h"
#include "gamedata.h"

class FishMultiSprite : public ScaleMultiSprite {
public:
  FishMultiSprite(const std::string&);
  virtual ~FishMultiSprite() { } 

  virtual void update(Uint32 ticks);
  virtual void rst();

protected:
  FishMultiSprite& operator=(const FishMultiSprite&);
  FishMultiSprite(const FishMultiSprite&);
  void advanceFrame();
  void distributeStartLocation();
};
#endif
