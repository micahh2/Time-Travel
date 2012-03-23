#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "data.h"
#include <vector>
#include <stdlib.h>

using namespace std;


int thing::nextId = 0;

void thing::init(int locx, int locy, int insizex, int insizey)
{
    id = nextId++;
    colId = 0;
    loc.x = locx;
    loc.y = locy;
    size.x = insizex;
    size.y = insizey;
}
thing::thing(int locx, int locy, int insizex, int insizey)
{
    init(locx, locy, insizex, insizey);
}
thing::thing()
{
    init(0,0,0,0);
}

character::character(int locx, int locy, int insizex, int insizey, int inspeed) : thing(locx, locy, insizex, insizey)
{
    init(locx, locy, inspeed);
}
character::character(int locx, int locy, int insizex, int insizey) : thing(locx, locy, insizex, insizey)
{
    init(locx, locy, 10);
}
character::character(int locx, int locy) : thing(locx, locy, 5, 5)
{
    init(locx, locy, 10);
}

void character::init(int locx, int locy, int inspeed)
{
    id = nextId++;
    dest.x = locx;
    dest.y = locy;
    speed = inspeed;
    pause = false;
    selected = true;
}

bool character::collision()
{
    //aStar(id, dest);
    return true;
}

map::map(string filetype)
{
    cout << "The end is near" << endl;
    levelFile.open("level1.lev", fstream::in | fstream::out);
    string tempData = "";
    if (levelFile.good())
    {
        cout << "File loading... " << endl;
        levelFile >> tempData;
    }
    if (tempData != "UNLEVELPARATT")
        cout << "Thats not a real level file!!";
    else
    {
        levelFile >> mapDim.x;
        levelFile >> mapDim.y;
        levelFile >> numChars;
        if(levelFile.fail())
        {
            cout << "Bad level file, couldn't load basic info.";
        }
    }
}

vector<character>* map::update(vector<character> *objects)
{
    for (unsigned int i = 0; i < objects->size(); i++)
    {
            int x = objects->at(i).loc.x;
            int y = objects->at(i).loc.y;
            int tempx = objects->at(i).loc.x;
            int tempy = objects->at(i).loc.y;
            
            int destx = objects->at(i).dest.x;
            int desty = objects->at(i).dest.y;
            int speed = objects->at(i).speed;
            
            if (abs(x-destx)>=1+(speed/2))
            {
                if(x<destx)
                    tempx+=speed;
                if(x>destx)
                    tempx-=speed;
            }
            if (abs(y-desty)>=2+(speed/2))
            {
                if(y<desty)
                    tempy+=speed;
                if(y>desty)
                    tempy-=speed;
            }
            bool crashy = false;
            bool crashx = false;
            // Add two step collision checking
            // Need to have more polite/smart blocks.
            //B   B   B
            // B  B  B
            for(unsigned int j=0; j<objects->size() && !crashx && !crashy; j++)
            {
                if (j != i)
                {
                    int sizex = objects->at(j).size.x;
                    int sizey = objects->at(j).size.y;
                    if (objects->at(j).loc.x > tempx)
                        sizex = objects->at(i).size.x;
                    if (objects->at(j).loc.y > tempy)
                        sizey = objects->at(i).size.y;
                    if (abs(objects->at(j).loc.x-tempx) < sizex && abs(objects->at(j).loc.y-tempy) < sizey) 
                    {
                        objects->at(i).colId = objects->at(j).id;
                        crashx = true;
                        crashy = true;
                    }
                }
            }
            //Semi useless to have two if(s) right now... But later it will be good.
            if (!crashx)
                objects->at(i).loc.x = tempx;
            if (!crashy)
                objects->at(i).loc.y = tempy;
    }

    return objects;
}

map::~map()
{
    levelFile.close();
    //delete [] characters;
}

