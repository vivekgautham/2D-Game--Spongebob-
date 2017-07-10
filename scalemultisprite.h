#ifndef SCALEMULTISPRITE__H
#define SCALEMULTISPRITE__H
#include <string>
#include <vector>
#include "sprite.h"
#include "drawable.h"

class ExplodingSprite;
class ScaleMultiSprite : public Drawable {
public:
  ScaleMultiSprite(const std::string&, const std::vector<Frame*>&);
  ScaleMultiSprite(const std::string&);
  virtual ~ScaleMultiSprite();
  
  bool operator<(const ScaleMultiSprite &rhs) const {
	  return scale < rhs.scale;
  }

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  virtual void explode();
  
  virtual bool isExploding() const { 
	  if(explosion)
		  return true;
	  else
		  return false;
	  }
	
protected:
  ExplodingSprite* explosion;
  double scale;
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);
  ScaleMultiSprite(const ScaleMultiSprite&);
  ScaleMultiSprite& operator=(const ScaleMultiSprite&);
};
#endif
