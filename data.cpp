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

vector<character>* map::update(vector<character> *objects)
{
    //Temp vars | DELETE
    int pastNum = -1;
    int pastTempx = -1;
    int pastTempy = -1;
    for(unsigned int i = 0; i < objects->size(); i++)
    {

        int colNum = 0;
        int tempx = objects->at(i).loc.x;
        int tempy = objects->at(i).loc.y; 
        if (abs(objects->at(i).loc.x-objects->at(i).dest.x)>=1+(objects->at(i).speed/2))
        {
            if(objects->at(i).loc.x<objects->at(i).dest.x)
                tempx+=objects->at(i).speed;
            if(objects->at(i).loc.x>objects->at(i).dest.x)
                tempx-=objects->at(i).speed;
        }
        if (abs(objects->at(i).loc.y-objects->at(i).dest.y)>=1+(objects->at(i).speed/2))
        {
            if(objects->at(i).loc.y<objects->at(i).dest.y)
                tempy+=objects->at(i).speed;
            if(objects->at(i).loc.y>objects->at(i).dest.y)
                tempy-=objects->at(i).speed;
        }
        for(unsigned int j = 0; j < objects->size() && colNum !=3; j++)
        {
            if (objects->at(i).id != objects->at(j).id)
            {
                int temp = collide(objects->at(i), objects->at(j), tempx, tempy);
                switch(temp)
                {
                    case 1:
                        if(colNum == 2)
                        {
                            colNum = 3;
                            break;
                        }
                    case 2:
                        if(colNum == 1)
                        {
                            colNum = 3;
                            break;
                        }
                    case 3:
                    case 0:
                    default:
                        colNum = temp;
                }
                if(temp == 4)
                {
                    cout << "BUGG!!" << endl;
                    if(pastNum != -1)
                    {
                        cout << "The past num is: " << pastNum << endl;
                        cout << "The past cordanates: " << pastTempx << ", " << pastTempy << endl << endl;
                    }
                }
                //DELETE
                pastNum = temp;
                pastTempx = tempx;
                pastTempy = tempy;
            }
        }
        if(colNum == 1 || colNum == 0)
            objects->at(i).loc.x = tempx;
        if (colNum == 2 || colNum == 0)
            objects->at(i).loc.y = tempy;
    }
    return objects;
}

//function collide
//returns 0 if no collision
//returns 1 if x collision
//returns 2 if y collision
//returns 3 if x and y collision
//FIXME
//Need an ENUM to make things more clear?
int map::collide(const character object1, const character object2, const int tempx, const int tempy)
{
    int crash = 0;

    // Need to have more polite/smart blocks.
    int sizex = object1.size.x;
    int sizey = object1.size.y;
    int size2x = object1.size.x;
    int size2y = object1.size.y;

    if (object2.loc.x < tempx)
        sizex = object2.size.x;
    if (object2.loc.y < tempy)
        sizey = object2.size.y;

    if (object2.loc.x < object1.loc.x)
        size2x = object2.size.x;
    if (object2.loc.y < object1.loc.y)
        size2y = object2.size.y;

    if (abs(object2.loc.x-tempx) < sizex && abs(object2.loc.y-object1.loc.y) < size2y) 
        crash+=1;
    if (abs(object2.loc.y-tempy) < sizey && abs(object2.loc.x-object1.loc.x) < size2x) 
        crash+=2;
    if (crash==0)
    {
        if (abs(object2.loc.x-tempx) < sizex && abs(object2.loc.y-tempy) < sizey) 
            crash = rand()%2+1;

    }
    if (abs(object2.loc.x-object1.loc.x) < size2x && abs(object2.loc.y-object1.loc.y) < size2y) 
        crash = 4;

    return crash;
}

map::~map()
{
    levelFile.close();
    //delete [] characters;
}
