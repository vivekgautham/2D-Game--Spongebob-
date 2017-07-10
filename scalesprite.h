#ifndef SCALESPRITE__H
#define SCALESPRITE__H
#include <string>
#include "drawable.h"

class ScaleSprite : public Drawable {
public:
  ScaleSprite(const std::string&, 
         const Vector2f& pos, const Vector2f& vel, const Frame*);
  ScaleSprite(const std::string&, const Vector2f& pos, const Vector2f& vel);
  ScaleSprite(const std::string&);

  ScaleSprite(const ScaleSprite& s);
  virtual ~ScaleSprite() { } 
  ScaleSprite& operator=(const ScaleSprite&);

  bool operator<(const ScaleSprite& rhs) const {
	  return scale < rhs.scale;
  }

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;

  virtual void update(Uint32 ticks);
  double getScale() const { return scale; } 

protected:
  double scale;
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  int getDistance(const ScaleSprite*) const;
};
#endif
