#include <iostream>
#include <fstream>
#include <SDL/SDL.h>
#include <time.h>
#include "data.h"
#include <vector>

using namespace std;

int width;
int length;
vector<character> objects;
double updateTime;

void wait(double seconds)
{
    double doneTime;
    doneTime = clock() + seconds * CLOCKS_PER_SEC;
    while (clock() < doneTime);
}

void init()
{
    width = 1100;
    length = 700;
    updateTime = 0;
    objects.push_back(character(rand()%(length-10), rand()%(width-10)));
    objects.push_back(character(rand()%(length-10), rand()%(width-10)));
    objects.push_back(character(rand()%(length-10), rand()%(width-10)));
    objects.push_back(character(rand()%(length-10), rand()%(width-10)));
    objects.push_back(character(rand()%(length-10), rand()%(width-10)));
}

class screen
{
    private:
        SDL_Surface *mainframe;
        SDL_Event event;
    public:
        screen()
        {
            if (SDL_Init(SDL_INIT_VIDEO) < 0) 
            {
                cout << "ERROR!!" << SDL_GetError() <<  endl;
            }
            else
                mainframe = SDL_SetVideoMode(width, length, 16, SDL_SWSURFACE | SDL_RESIZABLE);
            drawRect(0, 0, width, length);
        }
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
        void drawRect(int x, int y, int w, int l, int r, int g, int b)
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
            //SDL_UpdateRect(mainframe, x, y, width, length);
        }
        void drawRect(int x, int y, int w, int l)
        {
            drawRect(x,y,w,l,255,255,255);
        }

        bool die()
        {
            SDL_FreeSurface(mainframe);
            SDL_Quit();
            return true;
        }
        ~screen()
        {
            die();
        }
        void update(vector<character> *objects)
        {
            drawRect(0, 0, width, length);
            for(unsigned int i = 0; i < objects->size(); i++)
            {
                drawRect(objects->at(i).loc.y, objects->at(i).loc.x, 10, 10, 255, 190, 10);
            }
            SDL_Flip(mainframe);
            return;
        }
        bool events(vector<character> *objects)
        {
            SDL_PollEvent(&event);
            if (event.type == SDL_QUIT)
                return false;
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                for(unsigned int i = 0; i < objects->size(); i++)
                {
                    objects->at(i).dest.x = event.button.y;
                    objects->at(i).dest.y = event.button.x;
                }
            }
            if (event.type == SDL_VIDEORESIZE)
            {
                width = event.resize.w;
                length = event.resize.h;
                mainframe = SDL_SetVideoMode(width, length, 16, SDL_SWSURFACE | SDL_RESIZABLE);
                drawRect(0,0,width, length);
            }
            return true;
        }

};

int main() {
    srand(time(NULL));
    init();
    cout << "Yo!";
    screen frame;
    bool game = true;
    map *currentMap = new map("filename.lev");
    int count = 0;
    while (game)
    {
        game = frame.events(&objects); //Check for terminating events
        currentMap->update(&objects); //Update char maps
        frame.update(&objects); //Update screen
        count++;
        //repeat
    }
    delete currentMap;
    return 0;
}
