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
bool thing::aStar()
{
    cout << "I'm not ready to A*!" << endl;
    return true;
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

vector<character>* map::update(vector<character> *objects, int length, int width, dim biggest)
{
    for(unsigned int i = 0; i < objects->size(); i++)
    {
        objects->at(i).region.x = objects->at(i).loc.x/biggest.x;
        objects->at(i).region.y = objects->at(i).loc.y/biggest.y;
    }
    //int saved = 0;
    for(unsigned int i = 0; i < objects->size(); i++)
    {
        dim loc;
        loc.x = objects->at(i).loc.x;
        loc.y = objects->at(i).loc.y;
        dim dest;
        dest.x = objects->at(i).dest.x;
        dest.y = objects->at(i).dest.y;
        int speedx = objects->at(i).speed;
        int speedy = objects->at(i).speed;
        //Find the proposed x and y
        //Determine the speed factor
        int factorx = ((dest.x-loc.x+1)/(abs(dest.x-loc.x)+1))*2-1;
        int factory = ((dest.y-loc.y+1)/(abs(dest.y-loc.y)+1))*2-1;
        if(abs(dest.x-loc.x)<speedx)
            speedx = abs(dest.x-loc.x);
        if(abs(dest.y-loc.y)<speedy)
            speedy = abs(dest.y-loc.y);
        dim temp;
        temp.x = loc.x+(speedx*factorx);
        temp.y = loc.y+(speedy*factory);
        collisionType crash = neither;
        if(!(objects->at(i).dest.x == loc.x && objects->at(i).loc.y == dest.y))
        {
            for(unsigned int j = 0; j < objects->size() && crash != both; j++)
            {
                dim region1 = objects->at(i).region;
                dim region2 = objects->at(j).region;
                if(j != i && abs(region1.x-region2.x) <= 1 && abs(region1.y-region2.y) <= 1)
                {
                    collisionType col = collide(objects->at(i), objects->at(j), temp);
                    if (crash != both)
                    {
                        if(col == xmove)
                        {
                            if(crash!=ymove)
                                crash = xmove;
                            else
                                crash = both;
                        }
                        if(col == ymove)
                        {
                            if(crash!=xmove)
                                crash = ymove;
                            else
                                crash = both;
                        }
                        if(col == both)
                            crash = both;
                    }
                }
            }
        if(crash != xmove && crash != both)
           objects->at(i).loc.x = temp.x;
        if(crash != ymove && crash != both)
           objects->at(i).loc.y = temp.y;
        }
    }
    return objects;
}

//function collide
collisionType collide(const thing object1, const thing object2, const dim test)
{
    dim size = object1.size;
    dim size2 = object1.size;
    collisionType crash = neither;
    //If the x,y location of object2 is closer to the top corner than the proposed x,y values
    if(object2.loc.x < test.x)
        size.x = object2.size.x;
    if(object2.loc.y < test.y)
        size.y = object2.size.y;
    //If the x,y location of object2 is closer to the top corner than the current x,y values
    if(object2.loc.x < object1.loc.x)
        size2.x = object2.size.x;
    if(object2.loc.y < object1.loc.y)
        size2.y = object2.size.y;

    //Check collision using the proposed x value and the current y value
    if((abs(object2.loc.x-test.x) < size.x && abs(object2.loc.y-object1.loc.y) < size2.y) || test.x < 0)
            crash = xmove;
    //Check collision using the proposed y value and the current x value
    if((abs(object2.loc.x-object1.loc.x) < size2.x && abs(object2.loc.y-test.y) < size.y) || test.y < 0)
    {
        if(crash == xmove || crash == both)
            crash = both;
        else 
            crash = ymove;
    }
    //If both "moves" independantly work make sure that they work together
    if(crash==neither)
        if(abs(object2.loc.x-test.x) < size.x && abs(object2.loc.y-test.y)< size.y)
        {
            if(rand()%2 == 0) //Either xmove or ymove, I don't care
                crash = ymove;
            else
                crash = xmove;
        }
    return crash;
}
bool collide(const thing object1, const thing object2)
{
    //FIXME make effient 
    collisionType col = collide(object1, object2, object1.loc);
    if (col == neither)
        return false;
    return true;
}

map::~map()
{
    levelFile.close();
    //delete [] characters;
}
