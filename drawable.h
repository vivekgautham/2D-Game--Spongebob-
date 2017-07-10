#ifndef DRAWABLE__H
#define DRAWABLE__H
#include <SDL.h>
#include <iostream>
#include <string>
#include "vector2f.h"
#include "gamedata.h"
#include "frame.h"
 
// Drawable is an Abstract Base Class (ABC) that
// specifies the methods that derived classes may
// and must have.
class Drawable {
public:
  Drawable(const std::string& n, const Vector2f& pos, const Vector2f& vel): 
    name(n), position(pos), velocity(vel) {}

  Drawable(const Drawable& s) : 
    name(s.name), position(s.position), velocity(s.velocity)
    { }

  virtual ~Drawable() {}

  const std::string& getName() const { return name; }
  void setName(const std::string& n) { name = n;    }
  virtual const Frame* getFrame() const = 0;

  virtual void draw() const = 0;
  virtual void update(Uint32 ticks) = 0;
  virtual double getScale() const { return 1 ;}

  float X() const { return position[0]; }
  void X(float x) { position[0] = x;    }

  float Y() const { return position[1]; }
  void Y(float y) { position[1] = y;    }

  float velocityX() const  { return velocity[0]; }
  void velocityX(float vx) { velocity[0] = vx;   }
  float velocityY() const  { return velocity[1]; }
  void velocityY(float vy) { velocity[1] = vy;   }

  const Vector2f& getVelocity() const   { return velocity; }
//  const double& getXVelocity() const   { return velocity[0]; }
  void setVelocity(const Vector2f& vel) { velocity = vel;  }
  const Vector2f& getPosition() const   { return position; }
  void setPosition(const Vector2f& pos) { position = pos;  }
  const std::string& getName() {return name;}
  virtual bool collidedWith(const Drawable*) const { 
    throw std::string("No collidedWith");  
  }
	
  virtual void explode() { throw name + std::string("Can't Explode");}
  virtual bool isExploding() const { throw name + std::string("No Explosion");}

  virtual void rst() {
        setPosition(Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),Gamedata::getInstance().getXmlInt(name+"/startLoc/y")));
	}
  virtual void stop() {
        setVelocity(Vector2f(0,0));
	}
private:
  std::string name;
  Vector2f position;
  Vector2f velocity;
};
#endif
