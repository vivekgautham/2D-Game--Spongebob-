#ifndef PLAYER_H
#define PLAYER_H
#include "jellymultisprite.h"
#include "gamedata.h"
#include "math.h"
#include "objectpool.h"
#include "weapon.h"
#include "collisionStrategy.h"
#define PI 3.1415

class Hud;
class Enemy;

class Player 
{
	public:
		Player(const std::string& name);
		virtual	~Player();

		virtual void update(Uint32 ticks);
				
		virtual void draw() const {
			if (!exploded || sp->isExploding()){
				sp->draw();
				if(ks[SDLK_c])
					spchbub.draw();

				weapon.draw();
			}
		}

	    unsigned int freeWepns(){
			return weapon.freeCount();
		}
	    unsigned int inUseWepns(){
			return weapon.inUseCount();
		}

		const Drawable* getSprite() const {return sp;}
		Vector2f getPosition() const {return sp->getPosition();}
		float getX() const { return sp->X();}
		float getY() const { return sp->Y();}
		
		void rst() {
			sp->rst();
			exploded = false;
		}
	
		void stop(){
			sp->velocityX(0);
			sp->velocityY(0);
		}

		CollisionStrategy* getCollStrat(){
			return collStrategy;
		}

		bool collidedWith(const Drawable* d) const {
			return collStrategy->execute(*sp,*d);
		}
		
		bool collidedWithWeapon(const Drawable* d) {
			bool coll = weapon.collidedWith(d,collStrategy);
			if(coll){
				notifyHud(2);
			}
			return coll;
		}
		
		bool isRight() const {
			return facingRight;
		}
		
		int getenergyLevel() const {
			return energyLevel;
		}

		bool hasExploded() const { return exploded;}

		void attachToHud(Hud*);
		void notifyHud(int) const;

		void attachToEnemy(Enemy*);
		void notifyEnemy();
		
		void right();
		void left();
		void up();
		void down();
		void move(Uint8*,SDL_Event*);
		void explode();

	private:
		std::string nam;
		Drawable* sp;
		Sprite spchbub;
		ObjectPool<Weapon> weapon;
		Hud* hud;
		Enemy* en;
		CollisionStrategy* collStrategy;
		float spWidth;
		float spHeight;
		float worldWidth;
		float worldHeight;
		Uint8* ks;
		Vector2f velocity;
		Vector2f leapStart;
		int timer;
		double ang;
		float leapRadius;
		bool leap;
		bool facingRight;
		int energyLevel;
		bool exploded;

		Vector2f inv(Vector2f vel)
		{
			vel = 2.0 * vel;
			Vector2f vel1;
			vel1[0] = -1.0 * vel[0];
			vel1[1] = -1.0 * vel[1];
			return vel1;
		}
		
		Vector2f parametric()
		{
			Vector2f u(0,0);
			u[0] = leapStart[0] - leapRadius * cos(ang);
			u[1] = leapStart[1] - leapRadius * sin(ang);
			return u;
		}

		Player(const Player&);
		Player() ;
  		Player& operator=(const Player&);
};
		

#endif
