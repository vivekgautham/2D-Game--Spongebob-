#include "enemy.h"
#include "hud.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

float distance(Vector2f pos1,Vector2f pos2){
	float x = pos1[0] - pos2[0];
	float y = pos1[1] - pos2[1];
  	return hypot(x, y);
}

Enemy::~Enemy(){
	delete sp;
	delete decaysp;
	delete collStrategy;
}

Enemy::Enemy(const std::string& name,Player* pl):
	sp(new BiterMultiSprite(name)),
	decaysp(new BiterMultiSprite("dyingbiterfish")),
	key("key"),
	weapon(Gamedata::getInstance().getXmlStr(name+"/weapons")),
	collStrategy(new PerPixelCollisionStrategy),
  	p(pl),
	hud(NULL),
	spWidth( sp->getFrame()->getWidth()),
	spHeight( sp->getFrame()->getHeight()),
	worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
	worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
	velocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
             Gamedata::getInstance().getXmlInt(name+"/speedY")),
	energyLevel(0),
	currentMode(ATTACK),
	exploded(false)
{
	p->attachToEnemy(this);
}

void Enemy::fire(){
	if (energyLevel > 0 && currentMode==ATTACK)
	{
		weapon.load(1,sp->getPosition()+Vector2f(10,50),inv(sp->getVelocity()));
	}
}

void Enemy::ai()
{
	float x = sp->X() - sp->getFrame()->getWidth()/2;
	float y = sp->Y() - sp->getFrame()->getHeight()/2;
	float px = p->getSprite()->X() - p->getSprite()->getFrame()->getWidth()/2;
	float py = p->getSprite()->Y() - p->getSprite()->getFrame()->getHeight()/2;
	float distanceToEnemy = ::distance(x,y,px,py);
/*	
	std::cout << "e" <<  x << "\t" << y << std::endl;
	std::cout << "p " << px << "\t" << py << std::endl;
	std::cout << distanceToEnemy << std::endl;
	std::cout << currentMode << std::endl;
*/
	if(currentMode == ATTACK){
		if (distanceToEnemy < safeDistance){
			if (p->inUseWepns() > 0 && energyLevel < criticalEnergy)
				currentMode = EVADE;
			if (p->inUseWepns() > 0 && p->isRight())
				left();
			if (p->inUseWepns() > 0 && !p->isRight())
				right();
		}
		else {
			if ( x > px ) left();
			if ( x < px ) right();
			if ( y > py ) up();
			if ( y < py ) down();
			if (inUseWepns() < 2 && p->inUseWepns() == 0)
				fire();
		}
	}
	else if(currentMode == EVADE) {
		if(distanceToEnemy > safeDistance){
			if (p->inUseWepns() > 0 && energyLevel > criticalEnergy)
				currentMode = ATTACK;
		}
	/*	else {
			if ( x > px ) right();
			if ( x < px ) left();
			if ( y > py ) down();
			if ( y < py ) up();
		}*/
	}
	if(p->hasExploded())
		currentMode = EVADE;
}

void Enemy::update(Uint32 ticks)
{
	energyLevel = hud->energyLevelEnemy();
	sp->update(ticks);  
	decaysp->update(ticks);
    weapon.update(ticks);
	ai();
}
		
void Enemy::explode() 
{
	if(!exploded){
		sp->explode();
		decaysp->explode();
		exploded = true;
	}
}

void Enemy::right() 
{
	sp->velocityX(fabs(sp->velocityX()));	
	decaysp->velocityX(fabs(sp->velocityX()));	
}

void Enemy::left() 
{
	sp->velocityX(-fabs(sp->velocityX()));	
	decaysp->velocityX(-fabs(sp->velocityX()));	
}

void Enemy::up() 
{
	sp->velocityY(-fabs(sp->velocityY()));	
	decaysp->velocityY(-fabs(sp->velocityY()));	
}

void Enemy::down() 
{
	sp->velocityY(fabs(sp->velocityY()));	
	decaysp->velocityY(fabs(sp->velocityY()));	
}

void Enemy::attachToHud(Hud* h1)
{
	hud = h1;
}

void Enemy::notifyHud() const
{
	hud->updatePlayerMeter();
}


