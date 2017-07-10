#include "hud.h"
#include "gamedata.h"

Hud::Hud():
  name(),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  playerMeter(5,105,200,200,10,2,SDL_MapRGB(screen->format, 0x00,0xff,0x00),1000),
  enemyMeter(5,130,200,200,10,2,SDL_MapRGB(screen->format, 0x00,0x00,0xff),1000),
  p(NULL),
  e(NULL),
  posx(Gamedata::getInstance().getXmlInt("HUD/x")),
  posy(Gamedata::getInstance().getXmlInt("HUD/y")),
  width(Gamedata::getInstance().getXmlInt("HUD/width")),
  height(Gamedata::getInstance().getXmlInt("HUD/height")),
  disp(true),
  initSeconds(Gamedata::getInstance().getXmlInt("HUD/secs")),
  free(0),
  inUse(0)
{
}

Hud::Hud(const std::string& name):
  name(name),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  playerMeter(5,105,200,200,10,2,SDL_MapRGB(screen->format, 0x00,0xff,0x00),1000),
  enemyMeter(5,130,200,200,10,2,SDL_MapRGB(screen->format, 0x00,0x00,0xff),1000),
  p(NULL),
  e(NULL),
  posx(Gamedata::getInstance().getXmlInt("HUD/"+name+"/x")),
  posy(Gamedata::getInstance().getXmlInt("HUD/"+name+"/y")),
  width(Gamedata::getInstance().getXmlInt("HUD/"+name+"/width")),
  height(Gamedata::getInstance().getXmlInt("HUD/"+name+"/height")),
  disp(false),
  initSeconds(Gamedata::getInstance().getXmlInt("HUD/"+name+"/secs")),
  free(0),
  inUse(0)
{
}

Hud::Hud(const std::string& name, Player *pl):
  name(name),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  playerMeter(5,105,200,200,10,2,SDL_MapRGB(screen->format, 0x00,0xff,0x00),1000),
  enemyMeter(5,145,200,200,10,2,SDL_MapRGB(screen->format, 0x00,0x00,0xff),1000),
  p(pl),
  e(NULL),
  posx(Gamedata::getInstance().getXmlInt("HUD/"+name+"/x")),
  posy(Gamedata::getInstance().getXmlInt("HUD/"+name+"/y")),
  width(Gamedata::getInstance().getXmlInt("HUD/"+name+"/width")),
  height(Gamedata::getInstance().getXmlInt("HUD/"+name+"/height")),
  disp(false),
  initSeconds(Gamedata::getInstance().getXmlInt("HUD/"+name+"/secs")),
  free(0),
  inUse(0)
{
	p->attachToHud(this);
}

Hud::Hud(const std::string& name, Player *pl, Enemy *en):
  name(name),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  playerMeter(5,90,200,200,10,2,SDL_MapRGB(screen->format, 0x00,0xff,0x00),1000),
  enemyMeter(5,130,200,200,10,2,SDL_MapRGB(screen->format, 0x00,0x00,0xff),1000),
  p(pl),
  e(en),
  posx(Gamedata::getInstance().getXmlInt("HUD/"+name+"/x")),
  posy(Gamedata::getInstance().getXmlInt("HUD/"+name+"/y")),
  width(Gamedata::getInstance().getXmlInt("HUD/"+name+"/width")),
  height(Gamedata::getInstance().getXmlInt("HUD/"+name+"/height")),
  disp(false),
  initSeconds(Gamedata::getInstance().getXmlInt("HUD/"+name+"/secs")),
  free(0),
  inUse(0)
{
	p->attachToHud(this);
	e->attachToHud(this);
}

Hud::~Hud()
{
}

void Hud::draw() const{
	if (disp || (clock.getSeconds() < initSeconds)) {
	const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
	Draw_AALine(screen, posx, posy+height/2, posx+width, posy+height/2, height, 0xff, 0xff, 0xff,0xff/4);
	Draw_AALine(screen, posx, posy, posx, posy+height,3, RED);
	Draw_AALine(screen, posx, posy, posx+width, posy,3, RED);
	Draw_AALine(screen, posx+width, posy, posx+width, posy+height,3, RED);
	Draw_AALine(screen, posx, posy+height, posx+width, posy+height,3, RED);
	clock.draw();
    IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("HUD/"+name+"/message"),posx+5,posy+140);
  	IOManager::getInstance().printMessageAt("Jellyfish's Health",posx+5,posy+60);
  	IOManager::getInstance().printMessageAt("BiterFish's Health",posx+5,posy+100);
	playerMeter.draw();
	enemyMeter.draw();
    IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("HUD/"+name+"/message1"),posx+5,posy+160);
    IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("HUD/"+name+"/message2"),posx+5,posy+180);
    IOManager::getInstance().printMessageValueAt(Gamedata::getInstance().getXmlStr("HUD/"+name+"/message3"),inUse,posx+5,posy+200);
    IOManager::getInstance().printMessageValueAt(Gamedata::getInstance().getXmlStr("HUD/"+name+"/message4"),free,posx+5,posy+220);
    IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("HUD/"+name+"/message5"),posx+5,posy+240);
    IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("HUD/"+name+"/message6"),posx+5,posy+260);
    //IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("HUD/"+name+"/message7"),posx+5,posy+60);
    //IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("HUD/"+name+"/message8"),posx+5,posy+260);
	}
}

void Hud::update() {
	clock.update();
 // 	Uint32 ticks = clock.getTicksSinceLastFrame();
//	meter.update(ticks);
}

void Hud::updateWeaponCount() {
	free = p->freeWepns();
	inUse = p->inUseWepns();
}

void Hud::updateEnemyMeter() {
	enemyMeter.decHealth();
}

void Hud::updatePlayerMeter() {
	playerMeter.decHealth();
}

void Hud::toggleHud() {
	disp = !disp;
}

void Hud::rst() {
	clock.reset();
	playerMeter.reset();
	enemyMeter.reset();
	playerMeter.setColor(SDL_MapRGB(screen->format, 0x00,0xff,0x00));
	enemyMeter.setColor(SDL_MapRGB(screen->format, 0x00,0x00,0xff));
}
