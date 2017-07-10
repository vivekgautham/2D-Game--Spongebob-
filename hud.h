#ifndef HUD_H
#define HUD_H
#include "clock.h"
#include "player.h"
#include "enemy.h"
#include "health.h"
#include "aaline.h"
#include "ioManager.h"


class Hud
{
public:
  	Hud ();
  	Hud (const std::string&);
  	Hud (const std::string& ,Player*);
  	Hud (const std::string& ,Player*,Enemy*);
  	~Hud ();
	void rst();
	void draw() const;
	void update();
	void toggleHud();
	void updateWeaponCount();
	void updateEnemyMeter();
	void updatePlayerMeter();
	int energyLevelEnemy(void) const
	{
		return enemyMeter.getCurlevel();
	}
	int energyLevelPlayer(void) const
	{
		return playerMeter.getCurlevel();
	}

	bool isDisp() const {return disp;}
	unsigned int getinitSecs() const { return initSeconds;}
private:
	std::string name;  
    const IOManager& io;
	Clock& clock;
  	SDL_Surface * const screen;
	Health playerMeter;
	Health enemyMeter;
	Player * p; 
	Enemy * e; 
	int posx;
	int posy;
	int width;
	int height;
	bool disp;
	unsigned int initSeconds;
	unsigned int free;
	unsigned int inUse;
  	Hud(const Hud&);
  	Hud& operator=(const Hud&);
};
		
#endif
