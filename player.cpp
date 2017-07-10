#include "player.h"
#include "hud.h"
#include "enemy.h"

Player::~Player(){
	delete sp;
	delete collStrategy;
}

Player::Player(const std::string& name):
	nam(name),
	sp(new JellyMultiSprite(name)),
	spchbub(Gamedata::getInstance().getXmlStr(name+"/spchbub")),
	weapon(Gamedata::getInstance().getXmlStr(name+"/weapons")),
	hud(NULL),
	en(NULL),
	collStrategy(new PerPixelCollisionStrategy),
	spWidth( sp->getFrame()->getWidth()),
	spHeight( sp->getFrame()->getHeight()),
	worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
	worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
	ks(NULL),
	velocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
             Gamedata::getInstance().getXmlInt(name+"/speedY")),
	leapStart(0,0),
	timer(100),
	ang(0.0),
	leapRadius(Gamedata::getInstance().getXmlInt(name+"/leapRadius")),
	leap(false),
	facingRight(true),
	energyLevel(0),
	exploded(false)
{
	stop();	
}

void Player::update(Uint32 ticks)
{ 
	energyLevel = hud->energyLevelPlayer();
	if( leap && ang <= PI)
	{
		sp->setPosition(parametric());
		ang += PI/timer;
	}
	else{
		sp->update(ticks);  
		leap = false;
	}
	spchbub.setPosition((sp->getPosition())+Vector2f(80,-5));
    weapon.update(ticks);
	notifyHud(1);
}

void Player::move(Uint8* keystate, SDL_Event* event)
{
		stop();
	if(!exploded){
		if(keystate[SDLK_w])
		{
			up();
		}
		if(keystate[SDLK_s])
		{
			down();
		}
		if(keystate[SDLK_a])
		{
			left();
			facingRight = false;
		}
		if(keystate[SDLK_d])
		{
			right();
			facingRight = true;
		}

		if(keystate[SDLK_l])
		{
			leapStart = sp->getPosition() + Vector2f(leapRadius,0);
			ang = 0.0;
			leap = true;
		}

		if(keystate[SDLK_f])
		{
		  if (!(sp->isExploding())){
			Vector2f off(0,0);
			if(facingRight)
				off[0] = 80;
			else
				off[0] = 60;
			if (sp->getVelocity()[0] == 0)
			{
				Vector2f velocity(Gamedata::getInstance().getXmlInt(Gamedata::getInstance().getXmlStr(nam+"/weapons")+"/speedX"),Gamedata::getInstance().getXmlInt(Gamedata::getInstance().getXmlStr(nam+"/weapons")+"/speedY"));
				if(facingRight)
					velocity = inv(velocity);
				else
					velocity = 2*velocity;
				weapon.load(1,sp->getPosition()+off,velocity);
			}
			else
				weapon.load(1,sp->getPosition()+off,inv(sp->getVelocity()));
			notifyHud(1);
			notifyEnemy();
		  }
		}
		
		if ((keystate[SDLK_a] && keystate[SDLK_d]) || (keystate[SDLK_s] && keystate[SDLK_w]))
		{
			stop();
		}
		
		if (((*event).type == SDL_MOUSEBUTTONDOWN))
		{
			if ((*event).button.button == SDL_BUTTON_LEFT)
			{
			Vector2f off(0,0);
			if(facingRight)
				off[0] = 80;
			else
				off[0] = 60;
			if (sp->getVelocity()[0] == 0)
			{
				Vector2f velocity(Gamedata::getInstance().getXmlInt(Gamedata::getInstance().getXmlStr(nam+"/weapons")+"/speedX"),Gamedata::getInstance().getXmlInt(Gamedata::getInstance().getXmlStr(nam+"/weapons")+"/speedY"));
				if(facingRight)
					velocity = inv(velocity);
				else
					velocity = 2*velocity;
				weapon.load(1,sp->getPosition()+off,velocity);
			}
			else
				weapon.load(1,sp->getPosition()+off,inv(sp->getVelocity()));
			}
			notifyHud(1);
			notifyEnemy();
		}
	}
		ks = keystate;
}

void Player::explode() 
{
	if (!exploded){
		sp->explode();
		exploded = true;
	}
}

void Player::up()
{
	if(sp->Y() > 0){
		sp->velocityY(-velocity[1]);
	}
	else
		sp->velocityY(0);
}

void Player::down()
{
	if (sp->Y() < worldHeight-spHeight){
		sp->velocityY(velocity[1]);
	}
	else
		sp->velocityY(0);
}

void Player::right()
{
	if (sp->X() < worldWidth-spWidth){
		sp->velocityX(velocity[0]);
	}
	else
		sp->velocityX(0);
}

void Player::left()
{
	if (sp->X() > 0){
		sp->velocityX(-velocity[0]);
	}
	else
		sp->velocityX(0);
}

void Player::attachToHud(Hud* h1)
{
	hud = h1;
}

void Player::notifyHud(int c) const
{
	switch(c){
		case 1:
			hud->updateWeaponCount();
			break;
		case 2:
			hud->updateEnemyMeter();
			break;
		default:
			break;
	}
}

void Player::attachToEnemy(Enemy* e)
{
	en = e;
}

void Player::notifyEnemy() 
{
	en->fire();
}

