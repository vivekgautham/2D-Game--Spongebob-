#ifndef JELLYMULTISPRITE__H
#define JELLYMULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"
#include "multisprite.h"
#include "sprite.h"
#include "gamedata.h"

class JellyMultiSprite : public MultiSprite {
public:
  JellyMultiSprite(const std::string&);
  virtual ~JellyMultiSprite() { } 

  virtual void update(Uint32 ticks);

protected:
  JellyMultiSprite& operator=(const JellyMultiSprite&);
  JellyMultiSprite(const JellyMultiSprite&);
  void advanceFrame(Uint32 ticks);

};
#endif
