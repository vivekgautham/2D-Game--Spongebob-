#ifndef MANAGER_H
#define MANAGER_H
#include <list>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "hud.h"
#include "player.h"
#include "enemy.h"
#include "world.h"
#include "viewport.h"
#include "vector2f.h"
#include "multisprite.h"
#include "bitermultisprite.h"
#include "spongbobmultisprite.h"
#include "fishmultisprite.h"
#include "sprite.h"
#include "bubblesprite.h"
#include "gamedata.h"
#include "sound.h"


class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  SDLSound sound;
  World shallow;
  World coral;
  World middle;
  World deep;
  Viewport& viewport;
  Player p;
  Enemy e;
  Hud h;  
  std::list<Drawable*> sprites;
  std::list<Drawable*> bubbles;
  std::list<Drawable*> fishes;
  std::list<Drawable*>::iterator currentSprite;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  bool godMode;
  unsigned int countDownTimer;

  void rst();
  void draw() const;
  void update();
  void makeSprites();
  bool checkForCollisions();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
#endif
