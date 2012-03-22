#include <iostream>
#include <fstream>
#include <SDL/SDL.h>
#include <time.h>
#include "data.h"
#include <vector>

using namespace std;

int width;
int length;
int rate;
vector<character> objects;
thing dragBox(0,0,0,0);
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
    rate = 0;
    updateTime = 0;
    //dragbox = thing(0, 0, 0, 0);
    for (int i =0; i<10; i++)
    {
        int tsize = rand()%30+1;
        int hs = tsize/2;
        objects.push_back(character(rand()%(length-tsize)+hs, rand()%(width-tsize)+hs, tsize, tsize, rand()%10+1));
    }
}

class screen
{
    private:
        SDL_Surface *mainframe;
        SDL_Event event;
        int eventx;
        int eventy;
        bool drag;
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
            drag = false;
            //Event pos set
            eventx = 0;
            eventy = 0;
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
            if (rate == 0)
            {
                drawRect(0, 0, width, length);
                for(unsigned int i = 0; i < objects->size(); i++)
                {
                    drawRect(objects->at(i).loc.y, objects->at(i).loc.x, objects->at(i).size.x, objects->at(i).size.y, 255, 190, 10);
                }
                if (dragBox.on)
                {
                    drawRect(dragBox.loc.x, dragBox.loc.y, dragBox.size.x, dragBox.size.y, 0, 255, 255);
                }
                rate = 5;
                SDL_Flip(mainframe);
            }
            else
                rate--;
            return;
        }

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
                    cout << "I'm setting the down click at x:" << eventx << " y:" << eventy << endl;
                }
                //If the selection box is to be drawn
                else
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
                }
                drag = true;
            }
            //Moved drag to only change when the mouse hasn't moved
            if(event.type == SDL_MOUSEBUTTONUP)
            {
                dragBox.on = false;
                drag = false;
                if (abs(event.button.x-eventx) <=3 && abs(event.button.y-eventy) <=3)
                {
                    //Sets the dest for all the *selected* objects
                    for(unsigned int i = 0; i < objects->size(); i++)
                    {
                        if(objects->at(i).selected)
                        {
                            objects->at(i).dest.x = event.button.y;
                            objects->at(i).dest.y = event.button.x;
                        }
                    }
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
