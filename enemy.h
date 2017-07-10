#ifndef ENEMY_H
#define ENEMY_H
#include "bitermultisprite.h"
#include "gamedata.h"
#include "math.h"
#include "player.h"
#include "objectpool.h"
#include "weapon.h"
#include "collisionStrategy.h"

class Hud;

class Enemy 
{
	public:
		Enemy(const std::string& name,Player* pl);
		virtual	~Enemy();

		virtual void update(Uint32 ticks);
		virtual void draw() const {
			if (!exploded || sp->isExploding()){
				if (energyLevel > criticalEnergy)
					sp->draw();
				else
					decaysp->draw();
			}
			weapon.draw();
		}
		
		const Drawable* getSprite() const {return sp;}
		Vector2f getPosition() const {return sp->getPosition();}
		float getX() const { return sp->X();}
		float getY() const { return sp->Y();}
	
		void stop(){
			sp->velocityX(0);
			sp->velocityY(0);
			decaysp->velocityX(0);
			decaysp->velocityY(0);
		}
	    
		unsigned int freeWepns(){
			return weapon.freeCount();
		}
	    unsigned int inUseWepns(){
			return weapon.inUseCount();
		}

		bool collidedWith(const Drawable* d) const {
			return collStrategy->execute(*sp,*d);
		}
		
		bool collidedWithWeapon(const Drawable* d) {
			bool coll = weapon.collidedWith(d,collStrategy);
			if(coll){
				notifyHud();
			}
			return coll;
		}

		void rst() {
			sp->rst();
			decaysp->rst();
			exploded = false;
		}
		
		int getenergyLevel() const {
			return energyLevel;
		}
		
		bool hasExploded() const { return exploded;}

		static float getSafeDistance() { return safeDistance;}
		
		void attachToHud(Hud*);
		void notifyHud() const;

		void right();
		void left();
		void up();
		void down();
		void ai();
		void explode();
		void fire();

	private:
  		Drawable* sp;
  		Drawable* decaysp;
		MultiSprite key;
		ObjectPool<Weapon> weapon;
		CollisionStrategy* collStrategy;
		Player * p; 
		Hud *hud;
		float spWidth;
		float spHeight;
		float worldWidth;
		float worldHeight;
		Vector2f velocity;
		int energyLevel;
		enum MODE {EVADE, ATTACK};
		static float safeDistance;
		static float criticalEnergy;
		MODE currentMode;
		bool exploded;

		Vector2f inv(Vector2f vel)
		{
			vel = 5.0 * vel;
			Vector2f vel1;
			vel1[0] = -1.0 * vel[0];
			vel1[1] = -1.0 * vel[1];
			return vel1;
		}

		Enemy(const Enemy&);
		Enemy();
  		Enemy& operator=(const Enemy&);
};

#endif
