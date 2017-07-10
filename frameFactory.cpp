#include "frameFactory.h"
#include "extractSurface.h"
#include "ioManager.h"
#include "vector2f.h"
#include "SDL/SDL_rotozoom.h"

FrameFactory::~FrameFactory() {
  	std::map<std::string, SDL_Surface*>::iterator  itrsurfaces;
  	std::map<std::string, std::vector<SDL_Surface*> >::iterator itrmultiSurfaces;
  	std::map<std::string, Frame*>::iterator itrframes;
  	std::map<std::string, std::vector<Frame*> >::iterator itrmultiFrames;
  
  	std::vector<SDL_Surface*>::iterator itrscaledSurfaces;
  	std::vector<Frame*>::iterator itrscaledFrames;
  	std::vector<std::vector<SDL_Surface*> >::iterator itrmultiscaledSurfaces;
  	std::vector<std::vector<Frame*> >::iterator itrmultiscaledFrames;
		
	for(itrsurfaces = surfaces.begin();itrsurfaces != surfaces.end(); itrsurfaces++)
	{
		SDL_FreeSurface(itrsurfaces->second);
	}

	for(itrscaledSurfaces = scaledSurfaces.begin();itrscaledSurfaces != scaledSurfaces.end(); itrscaledSurfaces++)
	{
		SDL_FreeSurface(*itrscaledSurfaces);
	}

	for(itrmultiSurfaces = multiSurfaces.begin();itrmultiSurfaces != multiSurfaces.end(); itrmultiSurfaces++)
	{
		for(unsigned int i=0; i < (itrmultiSurfaces->second).size(); i++)
		{
			SDL_FreeSurface((itrmultiSurfaces->second)[i]);
		}
	}

	for(itrmultiscaledSurfaces = multiscaledSurfaces.begin();itrmultiscaledSurfaces != multiscaledSurfaces.end(); itrmultiscaledSurfaces++)
	{
		for(itrscaledSurfaces = (*itrmultiscaledSurfaces).begin();itrscaledSurfaces != (*itrmultiscaledSurfaces).end(); itrscaledSurfaces++)
		{
			SDL_FreeSurface(*itrscaledSurfaces);
		}
	}

	for(itrframes = frames.begin();itrframes != frames.end(); itrframes++)
	{
		delete itrframes->second;
	}
	
	for(itrscaledFrames = scaledFrames.begin();itrscaledFrames != scaledFrames.end(); itrscaledFrames++)
	{
		delete *itrscaledFrames;
	}

	for(itrmultiFrames = multiFrames.begin();itrmultiFrames != multiFrames.end(); itrmultiFrames++)
	{
		for(unsigned int i=0; i < (itrmultiFrames->second).size(); i++)
		{
			delete (itrmultiFrames->second)[i];
		}
	}
	
	for(itrmultiscaledFrames = multiscaledFrames.begin();itrmultiscaledFrames != multiscaledFrames.end(); itrmultiscaledFrames++)
	{
		for(itrscaledFrames = (*itrmultiscaledFrames).begin();itrscaledFrames != (*itrmultiscaledFrames).end(); itrscaledFrames++)
		{
			delete *itrscaledFrames;
		}
	}

}

FrameFactory& FrameFactory::getInstance() {
  static FrameFactory factory;
  return factory;
}

Frame* FrameFactory::getFrame(const std::string& name) {
    std::map<std::string, Frame*>::const_iterator pos = frames.find(name); 
  if ( pos == frames.end() ) {
    SDL_Surface * const surface =
      IOManager::getInstance().loadAndSet(
          gdata.getXmlStr(name+"/file"),
          gdata.getXmlBool(name+"/transparency"));
    surfaces[name] = surface;
    Frame * const frame =new Frame(name, surface);
    frames[name] = frame;
    return frame;
  }
  else {
    return pos->second;
  }
}

Frame* FrameFactory::getFrame(const std::string& name, double scale) {
    SDL_Surface * surface1 =
      IOManager::getInstance().loadAndSet(
          gdata.getXmlStr(name+"/file"),
          gdata.getXmlBool(name+"/transparency"));
	SDL_Surface *surface;
	surface = rotozoomSurface(surface1, 0, scale, SMOOTHING_ON);
    Frame * const frame = new Frame(surface, surface->w,surface->h,
					Gamedata::getInstance().getXmlInt(name+"/srcX"), 
					Gamedata::getInstance().getXmlInt(name+"/srcY")); 
	scaledSurfaces.push_back(surface);
	scaledFrames.push_back(frame);
	SDL_FreeSurface(surface1);
    return frame;
}

std::vector<Frame*> FrameFactory::getFrames(const std::string& name) {
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<Frame*> >::const_iterator 
    pos = multiFrames.find(name); 
  if ( pos != multiFrames.end() ) {
    return pos->second;
  }

  // It wasn't in the map, so we have to make the vector of Frames:
  SDL_Surface* surface = IOManager::
     getInstance().loadAndSet(gdata.getXmlStr(name+"/file"), true);
  unsigned numberOfFrames = gdata.getXmlInt(name+"/frames");
  std::vector<Frame*> frames;
  std::vector<SDL_Surface*> surfaces;
  frames.reserve(numberOfFrames);
  Uint16 srcX = gdata.getXmlInt(name+"/srcX");
  Uint16 srcY = gdata.getXmlInt(name+"/srcY");
  Uint16 width = gdata.getXmlInt(name+"/width");
  Uint16 height = gdata.getXmlInt(name+"/height");

  SDL_Surface* surf;
  for (unsigned i = 0; i < numberOfFrames; ++i) {
    unsigned frameX = i * width + srcX;
   surf = ExtractSurface::getInstance().
               get(surface, width, height, frameX, srcY); 
    surfaces.push_back( surf );
    frames.push_back( new Frame(name, surf) );
  }
  SDL_FreeSurface(surface);
  multiSurfaces[name] = surfaces;
  multiFrames[name] = frames;
  return frames;
}

std::vector<Frame*> FrameFactory::getFrames(const std::string& name, double scale) {
  
  SDL_Surface* surface = IOManager::
     getInstance().loadAndSet(gdata.getXmlStr(name+"/file"), true);
  unsigned numberOfFrames = gdata.getXmlInt(name+"/frames");
  std::vector<Frame*> frames;
  std::vector<SDL_Surface*> surfaces;
  frames.reserve(numberOfFrames);
  Uint16 srcX = gdata.getXmlInt(name+"/srcX");
  Uint16 srcY = gdata.getXmlInt(name+"/srcY");
  Uint16 width = gdata.getXmlInt(name+"/width");
  Uint16 height = gdata.getXmlInt(name+"/height");
  SDL_Surface *surf1,*surf;
  for (unsigned i = 0; i < numberOfFrames; ++i) {
    unsigned frameX = i * width + srcX;
   surf1 = ExtractSurface::getInstance().
               get(surface, width, height, frameX, srcY); 
	surf = rotozoomSurface(surf1, 0, scale, SMOOTHING_ON);
	surfaces.push_back( surf );
    frames.push_back( new Frame(surf, surf->w,surf->h,
					Gamedata::getInstance().getXmlInt(name+"/srcX"), 
					Gamedata::getInstance().getXmlInt(name+"/srcY"))); 
	SDL_FreeSurface(surf1);
  }
  SDL_FreeSurface(surface);
  multiscaledSurfaces.push_back(surfaces);
  multiscaledFrames.push_back(frames);
  return frames;
}

