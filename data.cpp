#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "data.h"
#include <vector>
#include <stdlib.h>

using namespace std;

int character::nextId = 0;
character::character(int locx, int locy, int insize, int inspeed)
{
    id = nextId++;
    loc.x = locx;
    loc.y = locy;
    dest.x = locx;
    dest.y = locy;
    size = insize;
    speed = inspeed;
    pause = false;
}
character::character(int locx, int locy, int insize)
{
    id = nextId++;
    loc.x = locx;
    loc.y = locy;
    dest.x = locx;
    dest.y = locy;
    size = insize;
    speed = 5;
    pause = false;
}
character::character(int locx, int locy)
{
    id = nextId++;
    loc.x = locx;
    loc.y = locy;
    dest.x = locx;
    dest.y = locy;
    size = 10;
    speed = 5;
    pause = false;
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
            int destx = objects->at(i).dest.x;
            int desty = objects->at(i).dest.y;
            int speed = objects->at(i).speed;
            int tempx = objects->at(i).loc.x;
            int tempy = objects->at(i).loc.y;
            if (abs(x-destx)>=2+(speed/2))
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
            //Need to come up with better checking
            //B   B   B
            // B  B  B
            for(unsigned int j=0; j<objects->size() && !crashx && !crashy; j++)
            {
                if (j != i)
                {
                    int sizex = objects->at(j).size;
                    int sizey = objects->at(j).size;
                    if (objects->at(j).loc.x > tempx)
                        sizex = objects->at(i).size;
                    if (objects->at(j).loc.y > tempy)
                        sizey = objects->at(i).size;
                    if (abs(objects->at(j).loc.x-tempx) < sizex && abs(objects->at(j).loc.y-tempy) < sizey) 
                    {
                        objects->at(i).colId = objects->at(j).id;
                        crashx = true;
                        crashy = true;
                    }
                }
            }
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

