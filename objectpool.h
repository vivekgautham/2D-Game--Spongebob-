#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H
#include <SDL.h>
#include "vector2f.h"
#include "collisionStrategy.h"
#include <list>
#include <string>

template <class TYPE>
class ObjectPool
{
	public:
		ObjectPool(std::string n):
							name(n),
							free(),
							inUse()
		{
		}

		~ObjectPool()
		{
			typename std::list<TYPE *>::iterator it;
			for(it = free.begin(); it != free.end(); it++)
			{
				delete (*it);
			}
			
			for(it = inUse.begin(); it != inUse.end(); it++)
			{
				delete (*it);
			}
		}
		
		unsigned int freeCount(){
			return free.size();
			}

		unsigned int inUseCount(){
			return inUse.size();
			}

		TYPE* getRes(const Vector2f&,const Vector2f&);
		void retRes(TYPE*);
		unsigned int draw() const;
		unsigned int update(Uint32);
		void load(unsigned int,const Vector2f&,const Vector2f&);
		bool collidedWith(const Drawable*,CollisionStrategy*);

	private:
		std::string name;		
		std::list<TYPE*> free;
		std::list<TYPE*> inUse;
		ObjectPool();
		ObjectPool(const ObjectPool&);
		ObjectPool& operator=(const ObjectPool&);
};

template <class TYPE>
bool ObjectPool<TYPE>::collidedWith(const Drawable* d,CollisionStrategy* c) {
	typename std::list<TYPE *>::iterator it = inUse.begin();
	Drawable* w;	
	while(it != inUse.end()){
		w = dynamic_cast<Drawable *>(*it);
		if(c->execute((*w),(*d))){
			(*it)->reset();
			retRes((*it));
			it = inUse.erase(it);
			return true;
		}
		++it;
	}
	return false;
}

template <class TYPE>
void ObjectPool<TYPE>::load(unsigned int count,const Vector2f& pos,const Vector2f& vel)
{
	while(count > 0)
	{
		getRes(pos,vel);
		count--;
	}
}

template <class TYPE>
TYPE* ObjectPool<TYPE>::getRes(const Vector2f& pos,const Vector2f& vel)
{
	TYPE *res = NULL;
		if (!free.empty())
		{
			res = free.front();
			free.erase(free.begin());
			res->set(pos,vel);
			inUse.push_back(res);
			return res;
		}
		else
		{
			res = new TYPE(name,pos,vel);
			inUse.push_back(res);
			return res;
		}
}

template <class TYPE>
void ObjectPool<TYPE>::retRes(TYPE* res)
{
	free.push_back(res);
}

template <class TYPE>
unsigned int ObjectPool<TYPE>::update(Uint32 ticks)
{
	typename std::list<TYPE *>::iterator it = inUse.begin();
	while(it != inUse.end())
	{
		if((*it)->goneTooFar())
		{
			(*it)->reset();
			retRes((*it));
			it = inUse.erase(it);
		}
		else
		{
			(*it)->update(ticks);
			it++;
		}
	}
	return free.size();
}
		
template <class TYPE>
unsigned int ObjectPool<TYPE>::draw() const
{
	typename std::list<TYPE *>::const_iterator it = inUse.begin();
	for (it = inUse.begin();it != inUse.end();it++)
		(*it)->draw();
	return inUse.size();
}

#endif
	
