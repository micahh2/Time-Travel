#include <iostream>
#include <fstream>
#include <SDL/SDL.h>
#include <time.h>
#include "data.h"
#include <vector>

using namespace std;

int width;
int length;
double rate;
vector<character> objects;
thing dragBox(0,0,0,0);
thing camera(10,10,width,length);
int frameRate;
double gameSpeed;
dim biggest;

//Waits for you, so you don't have to!
//Useless?
void wait(double seconds)
{
    double doneTime;
    doneTime = clock() + seconds * CLOCKS_PER_SEC;
    while (clock() < doneTime);
}

//Initializes all of the global variables
void init()
{
    width = 1100;
    length = 700;
    frameRate = 6;
    //Need a better way...
    //gameSpeed = 999999999999;
    biggest.x = 0;
    biggest.y = 0;
    for (int i =0; i<900; i++)
    {
        int tsize = rand()%10+10;
        int hs = tsize/2+1;
        character temp(rand()%(width-tsize)+hs, rand()%(length-tsize)+hs, tsize, tsize, rand()%10+1);
        for (unsigned int j = 0; j < objects.size(); j++)
        { 
            if (collide(temp, objects[j]))
            {
                temp = character(rand()%(width-tsize)+hs, rand()%(length-tsize)+hs, tsize, tsize, rand()%10+1);
                j = 0;
            }
        }
        objects.push_back(temp);
        if(tsize + temp.speed > biggest.x)
            biggest.x = tsize + temp.speed;
        if(tsize + temp.speed > biggest.y)
            biggest.y = tsize + temp.speed;
    }
}

//Its got the whole window in its hands...
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
            drawRect(0, 0, width, length);
            for(unsigned int i = 0; i < objects->size(); i++)
            {
                int r,g,b;
                if (objects->at(i).selected)
                {
                    r=0;
                    g=255;
                    b=0;
                }
                else
                {
                    r=190;
                    g=0;
                    b=255;
                }
                drawRect(objects->at(i).loc.x, objects->at(i).loc.y, objects->at(i).size.x, objects->at(i).size.y, r, g, b);
            }
            if (dragBox.on)
            {
                drawRect(dragBox.loc.x, dragBox.loc.y, dragBox.size.x, dragBox.size.y, 0, 255, 255);
            }
            SDL_Flip(mainframe);
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
                    //cout << "I'm setting the down click at x:" << eventx << " y:" << eventy << endl;
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
                    for(unsigned int i = 0; i < objects->size(); i++)
                    {
                        //The first one is the lowest y loc + the lowest x loc and the coresponding size values;
                        //The second one is the other x and y loc ..
                        thing comp1;
                        thing comp2;
                        if(objects->at(i).loc.x < dragBox.loc.x)
                        {
                            comp1.loc.x = objects->at(i).loc.x;
                            comp1.size.x = objects->at(i).size.x;
                            comp2.loc.x = dragBox.loc.x;
                            comp2.size.x = dragBox.size.x;
                        }
                        else
                        {
                            comp1.loc.x = dragBox.loc.x;
                            comp1.size.x = dragBox.size.x;
                            comp2.loc.x = objects->at(i).loc.x;
                            comp2.size.x = objects->at(i).size.x;
                        }

                        if(objects->at(i).loc.y < dragBox.loc.y)
                        {
                            comp1.loc.y = objects->at(i).loc.y;
                            comp1.size.y = objects->at(i).size.y;
                            comp2.loc.y = dragBox.loc.y;
                            comp2.size.y = dragBox.size.y;
                        }
                        else
                        {
                            comp1.loc.y = dragBox.loc.y;
                            comp1.size.y = dragBox.size.y;
                            comp2.loc.y = objects->at(i).loc.y;
                            comp2.size.y = objects->at(i).size.y;
                        }
                        if(abs(comp1.loc.x-comp2.loc.x)<=comp1.size.x && abs(comp1.loc.y-comp2.loc.y)<=comp1.size.y)
                        {
                            objects->at(i).selected = true;
                        }
                        else
                            objects->at(i).selected = false;
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
                        objects->at(i).selected=false;
                }
                else
                {
                    bool clickedObj = false;
                    //Check if you are clicking on a character in the objects vector
                    if (!clickedObj && abs(event.button.x-eventx) <=3 && abs(event.button.y-eventy) <=3)
                    {
                        for(unsigned int i = 0; i < objects->size() && !clickedObj; i++)
                        {
                            if(event.button.x-objects->at(i).loc.x<=objects->at(i).size.x && event.button.x-objects->at(i).loc.x>=0)
                                if (event.button.y-objects->at(i).loc.y<=objects->at(i).size.y && event.button.y-objects->at(i).loc.y>=0)
                                {
                                    clickedObj = true;
                                    for(unsigned int j = 0; j < objects->size(); j++)
                                        objects->at(j).selected = false;
                                    objects->at(i).selected = true;
                                }
                        }
                        //Sets the dest for all the selected objects
                        for(unsigned int i = 0; i < objects->size(); i++)
                        {
                            if(objects->at(i).selected)
                            {
                                objects->at(i).dest.x = event.button.x-objects->at(i).size.x/2;
                                objects->at(i).dest.y = event.button.y-objects->at(i).size.y/2;
                                if(objects->at(i).dest.x < 0)
                                    objects->at(i).dest.x = 0;
                                if(objects->at(i).dest.y < 0)
                                    objects->at(i).dest.y = 0;
                                if(objects->at(i).dest.x > width)
                                    objects->at(i).dest.x = width;
                                if(objects->at(i).dest.y > length)
                                    objects->at(i).dest.y = length;
                            }
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
    int count = 1;
    double tempo = 0;
    while (game)
    {
        //FIXME
        tempo = clock();
        game = frame.events(&objects); //Check for terminating events
        cout << "Checking events took: " << clock()-tempo << endl;
        tempo = clock();
        currentMap->update(&objects, length, width, biggest); //Update char maps
        cout << "Updating objects took: " << clock()-tempo << endl;
        if(count%frameRate == 0)
        {
            tempo = clock();
            frame.update(&objects); //Update screen
            cout << "Drawing to the screen took: " << clock()-tempo << endl;
        }
        count++;
        //repeat
    }
    delete currentMap;
    return 0;
}
