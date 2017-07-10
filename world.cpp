#include <iostream>
#include "world.h"
#include "frameFactory.h"

World::World(const std::string& name, int fact) : 
  io( IOManager::getInstance() ),
  surface( io.loadAndSet(
    Gamedata::getInstance().getXmlStr("shallow/file"), 
    Gamedata::getInstance().getXmlBool("shallow/transparency")) ),
  frame( FrameFactory::getInstance().getFrame(name)),
  factor(fact),
    frameWidth( frame->getWidth() ),
    worldWidth( Gamedata::getInstance().getXmlInt("world/width") ),
    viewX(0.0), viewY(0.0), 
    view(Viewport::getInstance()) 
{}

void World::update() {
  viewX = static_cast<int>(view.X() / factor) % frameWidth;
  viewY = view.Y();
}

void World::draw() const { 
  frame->draw(viewX, viewY, 0, 0); 
  frame->draw(0, viewY, frameWidth-viewX, 0); 
}

World::~World()
{
	SDL_FreeSurface(surface);
}
