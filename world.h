#include <string>
#include "ioManager.h"
#include "frame.h"
#include "viewport.h"

class World {
public:
  World(const std::string& name, int fact);
  ~World();
  void update();
  void draw() const;
private:
  const IOManager& io;
  SDL_Surface* const surface;
  Frame* frame;
  int factor;
  unsigned frameWidth;
  unsigned worldWidth;
  float viewX;
  float viewY;
  const Viewport & view;
  World(const World&);
  World& operator=(const World&);
};
