//The Concept for this file is to carry the most generic
//custom data types needed
//It currently has most of the functions for:
//	thing
//	character
//	idlist
//	character
//I don't particularly like what it has become and I'm trying
//to work it into a more congruent file.

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "data.h"
#include <vector>
#include <stdlib.h>
#include <cmath>

using namespace std;


//.............
// thing class
//.............
int thing::nextId = 0;

thing::thing(int locx, int locy, int insizex, int insizey)
{
    id = nextId;
	nextId++;
	colId = -1;
	colTry.x = 0;
	colTry.y = 0;
	loc.x = locx;
	loc.y = locy;
	size.x = insizex;
	size.y = insizey;
	updated = false;
}
bool thing::aStar()
{
	cout << "I'm not ready to A*!" << endl;
	return true;
}

//.............
// character class
//.............
character::character(int locx, int locy, int insizex, int insizey, int inspeed, int team, hatType t) : thing(locx, locy, insizex, insizey)
{
	type = t;
	teamId = team;
	id = nextId++;
	dest.x = locx;
	dest.y = locy;
	speed = inspeed;
	pause = false;
	selected = false;
}
bool character::atRest()
{
	if(dest.x == loc.x && loc.y == dest.y)
		return true;
	return false;
}

//.............
// idlist class
//.............
idlist::idlist(vector<character> *objects)
{
	init(objects);
}
void idlist::init(vector<character> *objects)
{
	list.resize(objects->size()+10,-1);
	for(unsigned int i = 0; i < objects->size(); i++)
	{
		int size = list.size();
		if(objects->at(i).id >= size)
			list.resize(objects->at(i).id*1.5,-1);
		list[objects->at(i).id] = i;
	}
}
void idlist::update(vector<character> *objects)
{
	for(unsigned int i = 0; i < objects->size(); i++)
	{
		int size = list.size();
		if(objects->at(i).id >= size)
			list.resize(objects->at(i).id*1.5,-1);
		list[objects->at(i).id] = i;
	}
}
int idlist::get(int colNum)
{
	int size = list.size();
	if(colNum>size)
		cout << "Major bad goose goose." << endl << endl << endl;
	int ans = list[colNum];
	if(ans < 0)
	{
		for(unsigned int i = 0; i < list.size(); i++)
		{
			cout << i << " : " << list[i] << endl;
		}
		cout << "Really bad goose goose.. " << colNum << endl;
	}
	return ans;
}

//.............
// map class
//.............
map::map(string filetype)
{
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
		levelFile >> width;
		levelFile >> length;
		levelFile >> numChars;
		if(levelFile.fail())
		{
			cout << "Bad level file, couldn't load basic info.";
		}
	}
	biggest.x = 0;
	biggest.y = 0;
	for (int i =0; i<100; i++)
	{
		int tsize = 40;//rand()%20+20;
		int hs = tsize/2+1;
		hatType hat;
		int team = 0;
		if(99==i)
			team=1;
		if(i<30)
		{
			team=2;
			tsize=80;
			hat = rock;
		}
		if(team==0)
		{
			hat = green;
			if(rand()%5==0)
				hat = blue;
		}
		if(team==1)
			hat = red;
		
		character temp(rand()%(width-tsize)+hs, rand()%(length-tsize)+hs, tsize, tsize, rand()%10+2, team, hat);
		for (unsigned int j = 0; j < objects.size(); j++)
		{ 
			if (collide(temp, objects[j]))
			{
				temp = character(rand()%(width-tsize)+hs, rand()%(length-tsize)+hs, tsize, tsize, rand()%10+2, team, hat);
				j = -1;
			}
		}
		objects.push_back(temp);
		if(tsize + temp.speed > biggest.x)
			biggest.x = tsize + temp.speed;
		if(tsize + temp.speed > biggest.y)
			biggest.y = tsize + temp.speed;
	}
	enemies.push_back(enemy());
}

vector<character>* map::update()
{
	idlist idnums(&objects);
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		objects.at(i).region.x = objects.at(i).loc.x/biggest.x;
		objects.at(i).region.y = objects.at(i).loc.y/biggest.y;
		objects.at(i).updated = false;
	}
	int saved = 0;
	int wasted = 0;
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		if(!objects[i].atRest())
		{
			dim loc = objects.at(i).loc;
			dim dest = objects.at(i).dest;

			double speedx = objects.at(i).speed;
			double speedy = objects.at(i).speed;
			double x = abs(dest.x - loc.x);
			double y = abs(dest.y - loc.y);

			double slope = 1;
			if (x!=0 && y!=0)
				slope = y/x;
			if(x>y)
				speedy *= slope;
			if(y>x)
				speedx *= 1/slope;

			//Find the proposed x and y
			//Determine the speed factor
			int factorx = ((dest.x-loc.x+1)/((int)x+1))*2-1;
			int factory = ((dest.y-loc.y+1)/((int)y+1))*2-1;
			if (factorx != 1 && factorx != -1)
				cout << "Error! Factor " << factorx << endl << endl;;
			if(x<speedx)
				speedx = x;
			if(y<speedy)
				speedy = y;

			dim temp;
			int retry = 0;

			temp.x = loc.x+(speedx*factorx);
			temp.y = loc.y+(speedy*factory);
			collisionType crash = neither;
			dim region1 = objects.at(i).region;
			for(unsigned int j = 0; j < objects.size() && (crash != both || true); j++)
			{
				dim region2 = objects.at(j).region;
				if(abs(region1.x-region2.x) <= 1 && abs(region1.y-region2.y) <= 1 && j!=i)
				{
					if(!objects.at(i).crash || (!objects.at(idnums.get(objects.at(i).colId)).updated && (objects.at(i).colTry.x != temp.x || objects.at(i).colTry.y != temp.y)))
					{
						collisionType col = collide(objects.at(i), objects.at(j), temp);
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
							{
								crash = both;
								objects.at(i).colId = objects.at(j).id;
							}
							if(col != neither)
								objects.at(i).colIds.push_back(objects.at(j).id);
							else
								wasted++;
						}
					}
					else
					{
						crash = both;
						saved++;
						retry = 1;
						if(saved%(objects.size()/5+1)==0 &&false)
						{
							objects.at(i).updated = true;
							objects.at(i).crash = false;
						}
					}
				}
			}
			if(crash != xmove && crash != both)
				objects.at(i).loc.x = temp.x;
			if(crash != ymove && crash != both)
				objects.at(i).loc.y = temp.y;
			if(crash == both)
			{
				if(retry != 1)
				{
					objects.at(i).updated = false;
					objects.at(i).crash = true;
					objects.at(i).colTry.x = temp.x;
					objects.at(i).colTry.y = temp.y;
				}
			}
			else
			{
				if(retry != 1)
				{
					objects.at(i).updated = true;
					objects.at(i).crash = false;
				}
			}
		}
	}
	cout << "Wasted: " << wasted << endl;
	double et = clock();
	for(unsigned int i = 0; i < enemies.size(); i++)
	{
		enemies[i].update(&objects, length, width);
	}
	cout << "Time to update enemies: " << clock()-et << endl;
	return &objects;
}

//Global function collide
collisionType collide(const thing object1, const thing object2, const dim test)
{
	collisionType crash = neither;
	if((pow((test.x+object1.size.x/2)-(object2.loc.x+object2.size.x/2), 2) + pow((test.y+object1.size.y/2)-(object2.loc.y+object2.size.y/2), 2)) < (pow(object1.size.x, 2) + pow(object1.size.y, 2)/4)+(pow(object2.size.x, 2) + pow(object2.size.y, 2)/4))
	{
		dim size = object1.size;
		dim size2 = object1.size;
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
		if(abs(object2.loc.x-test.x) < size.x && abs(object2.loc.y-test.y)< size.y && crash == neither)
			cout << "Help!!" << endl;
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

//.............
//enemy class
//.............
enemy::enemy(int team, string nm)
{
	name = nm;
	teamId = team;
}
bool enemy::update(vector<character> *objects, int length, int width)
{
	idlist table(objects);
	for(unsigned int i = 0; i < objects->size(); i++)
	{

		if(objects->at(i).teamId == teamId)
		{
			if(objects->at(i).atRest() || rand()%100==0)
			{
				objects->at(i).dest.x = rand()%(width-10) + 5;
				objects->at(i).dest.y = rand()%(length-10) + 5;
			}
			//
		}
		for(unsigned int j = 0; j < objects->at(i).colIds.size(); j++)
		{
			character *hit  = &(objects->at(table.get(objects->at(i).colIds.at(j))));
			if(objects->at(i).type==red || hit->type==red)
			{
				if(objects->at(i).type==red)
				{
					if(hit->type==green)
					{
						hit->teamId = teamId;
						hit->type = red;
					}
				}
				if(hit->type==red)
				{
					if(objects->at(i).type==green)
					{
						objects->at(i).teamId = teamId;
						objects->at(i).type = red;
					}
				}
			}
		}
		objects->at(i).colIds.clear();
	}
	return true;
}
