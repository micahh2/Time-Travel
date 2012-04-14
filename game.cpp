#include <iostream>
#include <fstream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <time.h>
#include "data.h"
#include <vector>

using namespace std;

//Global Variables defined here
double rate;
thing dragBox(0,0,0,0);
int frameRate;
double gameSpeed;
map *currentMap;

//Initializes all of the global variables
//It's called at the begining of main
void init()
{
	//width = 3100;
	//length = 2700;
	frameRate = 6;
	//Need a better way...
	//gameSpeed = 999999999999;
}

//It's got the whole window in its hands...
//It deals with drawing to the screen and
//handeling events
//It relies on "objects" being passed in
class screen
{
	private:
		SDL_Surface *mainframe;
		SDL_Event event;
		vector<SDL_Surface*> images;
		int eventx;
		int eventy;
		bool drag;
		int selectedSize;
		thing actualDrag;
		vector<SDL_Event> keyEvents;

		//The code for loading images into surfaces, mostly
		//used to but them in the "images" vector.
		SDL_Surface* loadImage(string filename)
		{
			SDL_Surface* loadedImage = NULL;
			loadedImage = IMG_Load(filename.c_str());
			if(loadedImage != NULL)
			{
				cout << "HEY" << endl;
				loadedImage = SDL_DisplayFormatAlpha(loadedImage);
				if(loadedImage != NULL)
				{
					SDL_SetColorKey(loadedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB(loadedImage->format, 0, 0xFF, 0xFF));
				}

			}
			return loadedImage;
		}

	public:
		thing camera; //Very important keeps
					  //size of the viewing window and
					  //the off-set/position of the camera
		//Initializes everthing for the screen class
		screen()
		{
			camera = thing(0,0,1100,700);
			actualDrag = thing(0,0,0,0);
			if (SDL_Init(SDL_INIT_VIDEO) < 0) 
			{
				cout << "ERROR!! " << SDL_GetError() <<  endl;
			}
			else
				mainframe = SDL_SetVideoMode(camera.size.x, camera.size.y, 16, SDL_SWSURFACE | SDL_RESIZABLE);

			images.push_back(loadImage("img/greenhat.png"));
			images.push_back(loadImage("img/greenhat2.png"));
			images.push_back(loadImage("img/bluehat.png"));
			images.push_back(loadImage("img/bluehat2.png"));
			images.push_back(loadImage("img/purplehat.png"));
			images.push_back(loadImage("img/purplehat2.png"));
			images.push_back(loadImage("img/redhat.png"));
			drag = false;
			//Event pos set
			eventx = 0;
			eventy = 0;
			selectedSize = 0;
		}
		//Very basic draw, just draws a pixle at position x,y with the "color"
		void draw(int x, int y, Uint32 color)
		{
			if (SDL_MUSTLOCK(mainframe))
				SDL_LockSurface(mainframe);
			Uint16 *bufp;

			bufp = (Uint16 *)mainframe->pixels + y*mainframe->pitch/2 + x;
			*bufp = color;

			if (SDL_MUSTLOCK(mainframe))
				SDL_UnlockSurface(mainframe);
		}
		//Used for drawing rectangles, relies on the draw function
		//If no r,g,b values are given it draws white.
		void drawRect(int x, int y, int w, int l, int r=255, int g=255, int b=255)
		{
			Uint32 color = SDL_MapRGB(mainframe->format, r, g, b);
			int x2 = x;
			int y2 = y;
			while (x2 < w+x)
			{
				while (y2 < l+y)
				{
					draw(x2, y2, color);
					y2++;
				}
				y2 = y;
				x2++;
			}
		}

		//The death of a screen is always a sad time
		//But it mostly just involves freeing up all of the
		//SDL_Surface (s)
		bool die()
		{
			for(unsigned int i = 0; i < images.size(); i++)
			{
				SDL_FreeSurface(images[i]);
				images.erase(images.begin()+i);
			}
			SDL_FreeSurface(mainframe);
			SDL_Quit();
			return true;
		}

		//The deconstructor, it calls die()
		//I might move every thing back to here
		//and get rid of the die() function
		~screen()
		{
			die();
		}
		//Update() is where the magic happens,
		//It Draws everything to the screen
		void update(vector<character> *objects)
		{
			drawRect(0, 0, camera.size.x, camera.size.y);
			//drawRect(loc.x, loc.y, size.x, size.y, r, g, b);
			SDL_Rect square;
			//SDL_Rect* clip = NULL;
			for(unsigned int i = 0; i < objects->size(); i++)
			{
				if(collide(camera, objects->at(i)))
				{
					//dim size = objects->at(i).size;;
					dim loc = objects->at(i).loc;
					loc.x-=camera.loc.x;
					loc.y-=camera.loc.y;

					square.x = loc.x;
					square.y = loc.y;

					int index = 0;
					if (objects->at(i).type == green)
						index = 0;
					if (objects->at(i).type == blue)
						index = 2;
					if (objects->at(i).type == purple)
						index = 4;
					if (objects->at(i).selected)
						index++;
					if (objects->at(i).type == red)
						index = 6;
					
					SDL_BlitSurface(images[index], NULL, mainframe, &square);
				}
			}
			if (dragBox.on)
				drawRect(dragBox.loc.x, dragBox.loc.y, dragBox.size.x, dragBox.size.y, 0, 255, 255);

			SDL_Flip(mainframe);

			return;
		}

		//This needs a rewrite as it can be quite confusing
		//to try and follow
		//But it handles all of the input from SDL and returns true to keep the game going
		//or false if the "X" button has been pressed
		bool events(vector<character> *objects)
		{
			SDL_PollEvent(&event);
			if (event.type == SDL_QUIT)
				return false;
			if (event.type == SDL_MOUSEBUTTONDOWN || drag)
			{
				if (!drag)
				{
					eventx = event.button.x;
					eventy = event.button.y;
				}
				//If the selection box is to be drawn
				//Only start checking for selected things if the mouse has moved
				else if (abs(event.button.x-eventx) >=12 || abs(event.button.y-eventy) >= 12)
				{
					dragBox.on = true;
					if (eventx > event.button.x)
					{
						dragBox.size.x = eventx - event.button.x;
						dragBox.loc.x = event.button.x;
					}
					else
					{
						dragBox.size.x = event.button.x - eventx;
						dragBox.loc.x = eventx;
					}
					if (eventy > event.button.y)
					{
						dragBox.size.y = eventy - event.button.y;
						dragBox.loc.y = event.button.y;
					}
					else
					{
						dragBox.size.y = event.button.y - eventy;
						dragBox.loc.y = eventy;
					}
					actualDrag = dragBox;
					actualDrag.loc.x = dragBox.loc.x + camera.loc.x;
					actualDrag.loc.y = dragBox.loc.y + camera.loc.y;
					for(unsigned int i = 0; i < objects->size(); i++)
					{
						if(collide(actualDrag, objects->at(i)))
						{
							objects->at(i).selected = true;
							selectedSize++;
						}
						else
						{
							selectedSize--;
							objects->at(i).selected = false;
						}
					}

				}
				drag = true;
			}
			if(event.type == SDL_MOUSEBUTTONUP && drag)
			{
				dragBox.on = false;
				drag = false;
				if (event.button.button == SDL_BUTTON_RIGHT)
				{
					for(unsigned int i = 0; i < objects->size(); i++)
						if(objects->at(i).teamId == 0)
							objects->at(i).selected=false;
					selectedSize = 0;
				}
				else
				{
					bool clickedObj = false;
					//Check if you are clicking on a character in the objects vector
					if (abs(event.button.x-eventx) <=3 && abs(event.button.y-eventy) <=3)
					{
						for(unsigned int i = 0; i < objects->size() && !clickedObj; i++)
						{
							if(collide(character(event.button.x+camera.loc.x,event.button.y+camera.loc.y,1,1), objects->at(i)))
							{
								clickedObj = true;
								for(unsigned int j = 0; j < objects->size(); j++)
									if(objects->at(i).teamId == 0)
										objects->at(j).selected = false;
								if(objects->at(i).teamId == 0)
									objects->at(i).selected = true;
								selectedSize = 1;
							}
						}
						if(!clickedObj)
						{
						//Sets the dest for all the selected objects
						for(unsigned int i = 0; i < objects->size(); i++)
						{
						if(objects->at(i).teamId == 0)
						{
							if(objects->at(i).selected)
							{
								objects->at(i).dest.x = event.button.x-objects->at(i).size.x/2 + camera.loc.x;
								objects->at(i).dest.y = event.button.y-objects->at(i).size.y/2 + camera.loc.y;
								if(objects->at(i).dest.x < 0)
									objects->at(i).dest.x = 0;
								if(objects->at(i).dest.y < 0)
									objects->at(i).dest.y = 0;
								if(objects->at(i).dest.x > currentMap->width)
									objects->at(i).dest.x = currentMap->width-objects->at(i).size.x;
								if(objects->at(i).dest.y > currentMap->length)
									objects->at(i).dest.y = currentMap->length-objects->at(i).size.y;
							}
						}
						}
						}
					}
				}
			}
			//When the screen is being resized
			if (event.type == SDL_VIDEORESIZE)
			{
				camera.size.x = event.resize.w;
				camera.size.y = event.resize.h;
				mainframe = SDL_SetVideoMode(camera.size.x, camera.size.y, 16, SDL_SWSURFACE | SDL_RESIZABLE);
				//drawRect(0,0,width, length);
			}
			//From here down we deal with the arrow keys
			//The system is to accept all key presses into a vector and
			//act on them until a keyup event deletes the from that vector
			if(event.type == SDL_KEYUP);
			{
				for(unsigned int i =0; i <keyEvents.size(); i++)
					if(keyEvents[i].key.keysym.sym == event.key.keysym.sym)
					{
						cout << "Deleting!" << endl;
						keyEvents.erase(keyEvents.begin()+i);
					}
			}
			if(event.type == SDL_KEYDOWN)
			{
				cout << "Size: " << keyEvents.size() << endl;
				bool found = false;
				do
				{
				for(unsigned int i =0; i < keyEvents.size() && !found; i++)
					if(keyEvents[i].key.keysym.sym == event.key.keysym.sym)
						found = true;

				if(!found)
				{
					keyEvents.push_back(event);
					cout << "Adding the key!" << endl;
				}
				else
					cout << "Pressed!" << endl;
				}while(found==false);

			}
			//Going through all of the current key events
			for(unsigned int i =0; i < keyEvents.size(); i++)
			{
				//cout << "Thats the key!" << endl;
				int plus = 0;
				if(keyEvents[i].key.keysym.sym == SDLK_UP)
				{
					plus = 5;
					while(camera.loc.y<plus)
						plus--;
					camera.loc.y-=plus;
				}
				if(keyEvents[i].key.keysym.sym == SDLK_DOWN)
				{
					plus = 5;
					while(camera.loc.y+camera.size.y>currentMap->length-plus)
						plus--;

					camera.loc.y+=plus;
				}
				if(keyEvents[i].key.keysym.sym == SDLK_LEFT)
				{
					plus = 5;
					while(camera.loc.x<plus)
						plus--;
					camera.loc.x-=plus;
				}
				if(keyEvents[i].key.keysym.sym == SDLK_RIGHT)
				{
					plus = 5;
					while(camera.loc.x+camera.size.x>currentMap->width-plus)
						plus--;
					camera.loc.x+=plus;
				}
			}
			return true;
		}

};

//The main
//Is fairly basic,
//I've tried to keep it simple
int main() {
	srand(time(NULL));
	init();
	cout << "Yo!";
	screen frame;
	bool game = true;
	currentMap = new map("filename.lev");
	int count = 1;
	double tempo = 0;
	while (game)
	{
		//FIXME
		tempo = clock();
		game = frame.events(&(currentMap->objects)); //Check for terminating events
		cout << "Checking events took: " << clock()-tempo << endl;
		tempo = clock();
		currentMap->update(); //Update char maps
		cout << "Updating objects took: " << clock()-tempo << endl;
		if(count%frameRate == 0)
		{
			tempo = clock();
			frame.update(&(currentMap->objects)); //Update screen
			cout << "Drawing to the screen took: " << clock()-tempo << endl;
		}
		count++;
		//repeat
	}
	delete currentMap;
	return 0;
}
