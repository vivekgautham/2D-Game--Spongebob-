#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "clock.h"
#include "gamedata.h"
#include "ioManager.h"

Clock& Clock::getInstance() {
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before Clock");
  }
  static Clock clock; 
  return clock;
}

Clock::Clock() :
  ticks(0),
  started(false), 
  paused(false), 
  sloMo(false), 
  fps(0),
  sumOfTicks(SDL_GetTicks()),
  frameCount(0),
  totalTicks(SDL_GetTicks()),
  pos( Gamedata::getInstance().getXmlInt("clock/locX"),
       Gamedata::getInstance().getXmlInt("clock/locY")),
  frameCap(Gamedata::getInstance().getXmlInt("frameCap")),
  framesAreCapped(Gamedata::getInstance().getXmlBool("framesAreCapped")),
  ticksForFrame(1000/frameCap)
  {
  start();
}

void Clock::draw() const { 
  IOManager::getInstance().
    printMessageValueAt("Seconds: ", getSeconds(), pos[0], pos[1]);
  IOManager::getInstance().
    printMessageValueAt("fps: ", getFps(), pos[0], pos[1]+20);
}


void Clock::reset() {
	sumOfTicks = 0;
	totalTicks = 0;
    frameCount = 0;
}

void Clock::update() { 
  if(paused){
  	  ticks= 0;
  }
  else if(sloMo){
	  frameCount++;
	  ticks = 1;
	  sumOfTicks += ticks;
	  fps = frameCount*1000/sumOfTicks;
  }
  else{
	  frameCount++;
	  ticks= SDL_GetTicks() - totalTicks;
	  ticks += capFrameRate();
	  sumOfTicks += ticks;
	  fps = frameCount*1000/sumOfTicks;
  }
  totalTicks = SDL_GetTicks();
}

unsigned int Clock::getTicksSinceLastFrame() const {
  return ticks;
}

void Clock::toggleSloMo() {
  sloMo = ! sloMo;
}

int Clock::getFps() const { 
  return fps;
}

void Clock::start() { 
  started = true; 
  paused = false; 
}

void Clock::pause() { 
	paused = true;
}

void Clock::unpause() { 
	paused = false;
}

unsigned int Clock::capFrameRate() const {
	if(!framesAreCapped)
		return 0;
	else{
		if (ticks < ticksForFrame)
		{
			SDL_Delay(ticksForFrame - ticks);
		}
		return (ticksForFrame - ticks);
	}
}

		


