#ifndef CLOCK_H
#define CLOCK_H
#include <SDL.h>
#include <string>
#include <deque>
#include "vector2f.h"

class Manager;

class Clock {
public:
  static Clock& getInstance();  // This class is a Singleton
  unsigned int getTicksSinceInit() const { return sumOfTicks; }
  unsigned int getSeconds() const { return sumOfTicks/1000; }

  void toggleSloMo();
  bool isStarted() const { return started; }
  bool isPaused() const  { return paused;  }
  int getFps() const;
	
  void reset();
  void start();
  void pause();
  void unpause();
  void draw() const;  // For debugging
  void update();  // Increments time/ticks
  unsigned int capFrameRate() const;
  unsigned int getTicksSinceLastFrame() const;

private:
  unsigned int ticks;
  bool started;
  bool paused;
  bool sloMo;
  unsigned int fps;
  unsigned int sumOfTicks;
  unsigned int frameCount;
  unsigned int totalTicks;
  Vector2f pos;
  int frameCap;
  bool framesAreCapped;
  Uint32 ticksForFrame;

  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);
  friend class Manager;
//  void update();  // Increments time/ticks
};
#endif
