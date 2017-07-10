#include "manager.h"
#include "extractSurface.h"
class ScaledSpritesCompare {
	public:
		bool operator()(const Drawable* lhs, const Drawable* rhs) {
			return lhs->getScale() < rhs->getScale();
		}
};

Manager::~Manager() { 
  std::list<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    delete (*ptr);
    ++ptr;
  }
  ptr = bubbles.begin(); 
  while ( ptr != bubbles.end() ) {
    delete (*ptr);
    ++ptr;
  }
  ptr = fishes.begin(); 
  while ( ptr != fishes.end() ) {
    delete (*ptr);
    ++ptr;
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  sound(),
  shallow("shallow", Gamedata::getInstance().getXmlInt("shallow/factor") ),
  coral("coral", Gamedata::getInstance().getXmlInt("coral/factor") ),
  middle("middle", Gamedata::getInstance().getXmlInt("middle/factor") ),
  deep("deep", Gamedata::getInstance().getXmlInt("deep/factor") ),
  viewport( Viewport::getInstance() ),
  p("jellyfish"),
  e("biterfish",&p),
  h("main",&p,&e),
  sprites(),
  bubbles(),
  fishes(),
  currentSprite(),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  godMode(false),
  countDownTimer(Gamedata::getInstance().getXmlInt("clock/cdtimer")*1000)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  makeSprites();
  currentSprite = sprites.end();
  viewport.setObjectToTrack(p.getSprite()); // Tracking Player
}

void Manager::makeSprites(){
  for(int i=0;i<Gamedata::getInstance().getXmlInt("bubble/count");i++)
  	bubbles.push_back( new BubbleSprite("bubble") );
  for(int i=0;i<Gamedata::getInstance().getXmlInt("fish/count");i++)
  	fishes.push_back( new FishMultiSprite("fish") );
  sprites.push_back( new SpongbobMultiSprite("spongebob"));

  bubbles.sort(ScaledSpritesCompare());
  fishes.sort(ScaledSpritesCompare());
}  

void Manager::rst(){
  	countDownTimer = Gamedata::getInstance().getXmlInt("clock/cdtimer") * 1000;
	godMode = false;
	sound.startMusic();
  	currentSprite = sprites.end();
  	viewport.setObjectToTrack(p.getSprite()); // Tracking Player
	p.rst();
	h.rst();
	e.rst();
    std::list<Drawable*>::iterator ptr = bubbles.begin();
	while ( ptr != bubbles.end()){
		(*ptr)->rst();
    	++ptr;
	}
    std::list<Drawable*>::iterator ptr1 = fishes.begin();
	while ( ptr1 != fishes.end()){
		(*ptr1)->rst();
    	++ptr1;
	}
	std::list<Drawable*>::iterator sprite = sprites.begin();
	while ( sprite != sprites.end()){
		(*sprite)->rst();
		++sprite;
	}
}

std::string convToMinFmt(unsigned int secs) {
	std::stringstream minsec;
	unsigned int min=0,sec=0;
	sec = secs % 60;
	min = secs/60;
	if (sec < 10)
		minsec << min << ":0" << sec; 
	else
		minsec << min << ":" << sec; 
	return minsec.str();
}


void Manager::draw() const {
  unsigned int i = 0;
  unsigned int j = 0;
  std::list<Drawable*>::const_iterator ptr = bubbles.begin();
  std::list<Drawable*>::const_iterator ptr1 = fishes.begin();
  shallow.draw();
  for (;i < bubbles.size()/4;ptr++,i++)
	(*ptr)->draw();
  for (;j < fishes.size()/4;ptr1++,j++)
	(*ptr1)->draw();
  coral.draw();
  for (;i < bubbles.size()/2;ptr++,i++)
	(*ptr)->draw();
  for (;j < fishes.size()/2;ptr1++,j++)
	(*ptr1)->draw();
  middle.draw();
  for (;i < 3*bubbles.size()/4;ptr++,i++)
	(*ptr)->draw();
  for (;j < 3*fishes.size()/4;ptr1++,j++)
	(*ptr1)->draw();
  deep.draw();
  for (;i < bubbles.size();ptr++,i++)
	(*ptr)->draw();
  for (;j < fishes.size();ptr1++,j++)
	(*ptr1)->draw();
  
  h.draw(); // draws clock as well
  ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    (*ptr)->draw();
    ++ptr;
  }
  if(e.hasExploded() && !(e.getSprite()->isExploding()))
  		IOManager::getInstance().printMessageCenteredAt("Congratulations You Win -- You have freed the SpongeBob  ", 200);
  else
  		e.draw();

  if(p.hasExploded() && !(p.getSprite()->isExploding()) && countDownTimer !=0)
  		IOManager::getInstance().printMessageCenteredAt("Jelly Fish has Exploded -- You Lose -- Game Over ", 200);
  else
  		p.draw();

  if(countDownTimer == 0)
  		IOManager::getInstance().printMessageCenteredAt("Timer has Expired -- You Lose -- Game Over ", 200);

  if (h.isDisp() || (clock.getSeconds() < h.getinitSecs())) {
  	if (!godMode) 
  		IOManager::getInstance().printMessageAt(("Time Left: " + ::convToMinFmt(countDownTimer/1000)) + " God Mode OFF", 10, 50);
  	else  
  		IOManager::getInstance().printMessageAt(("Time Left: " + ::convToMinFmt(countDownTimer/1000)) + " God Mode ON", 10, 50);
  }

  viewport.draw();
  io.printMessageAt(title, 10, 450);
  SDL_Flip(screen);
}

// Move this to IOManager
void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

bool Manager::checkForCollisions() {
	std::list<Drawable*>::iterator sprite = fishes.begin();
	while ( sprite != fishes.end()){
		if (p.collidedWith(*sprite) && !(p.hasExploded())){
				(*sprite)->explode();
			return true;
		}
		++sprite;
	}
	
	if (p.collidedWithWeapon(e.getSprite()))
	{
		if(e.getenergyLevel() <= 0){
			e.explode();
			sound[2];
		}
		return true;
	}

	if (!godMode && !(p.hasExploded())){
    	if (p.collidedWith(e.getSprite()))
    	{
    		if(p.getenergyLevel() <= 0)
			{
    			p.explode();
				sound[2];
			}
    		return true;
    	}
    	if (e.collidedWithWeapon(p.getSprite()))
    	{
    		if(p.getenergyLevel() <= 0){
    			p.explode();
				sound[2];
    			//stop();
    		}
    		return true;
    	}
	}

	return false;
}

void Manager::switchSprite() {
  currentSprite = sprites.begin();
  /*if ( currentSprite == sprites.end() ) {
    currentSprite = sprites.begin();
  }*/
  viewport.setObjectToTrack(*currentSprite);
}

void Manager::update() {
  h.update(); //hud updates clock as well
  Uint32 ticks = clock.getTicksSinceLastFrame();
  if(!godMode && countDownTimer != 0 && (!e.hasExploded()) && !(p.hasExploded()))
  	countDownTimer -= ticks;
  std::list<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    (*ptr)->update(ticks);
    ++ptr;
  }

  ptr = bubbles.begin();
  while ( ptr != bubbles.end() ) {
    (*ptr)->update(ticks);
    ++ptr;
  }
  
  ptr = fishes.begin();
  while ( ptr != fishes.end() ) {
    (*ptr)->update(ticks);
    ++ptr;
  }
  
  p.update(ticks);
  e.update(ticks);
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  if(checkForCollisions())
  {
	 // io.printMessageAt("Coll",200,200);
  }
  if(countDownTimer < 10 && !p.hasExploded()){
	p.explode();
	sound[2];
	countDownTimer = 0;
	//stop();
  }
  if (p.inUseWepns() > 0)
  {
	  sound[0];
  }
  if (e.inUseWepns() > 1)
  {
	  sound[1];
  }
  
  if (e.hasExploded() && !(e.getSprite()->isExploding()) && (currentSprite != sprites.begin())){
		switchSprite();
		sprites.front()->explode();
  }

  shallow.update();
  coral.update();
  middle.update();
  deep.update();
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  clock.start();

  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
	  p.move(keystate,&event);//player move itself dep on keystate:Avoid Micromanaging
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_t] ) {
          switchSprite();
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDLK_m]) {
          clock.toggleSloMo();
        }
		if (keystate[SDLK_r]){
			rst();
		}

		if (keystate[SDLK_g]){
			godMode = !godMode;
		}

		if (keystate[SDLK_F1]) {
			h.toggleHud();
		}

		if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
      }
    }
    draw();
    update();
  }
}
