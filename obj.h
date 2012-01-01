#include <stdlib.h>
#include "data.h"
#ifndef OBJ
#define OBJ

// This header file contains the basic types that aren't reall useable in the game.
// Specific types are defined in "units.h" and "buildings.h"
// With the exception of time markers

using namespace std;

//The mother of all objects
//No size no place, just a name and an ID
//The counter makes sure that no object gets the same ID.
class obj
{
	protected:
		static int counter;
	public:
		int id;
		string name;

		obj();
		obj(string nm);
    bool die(vector<obj> *placeTo);

};
class thing : public obj
{
  protected:
    int health;
	public:
		string img;
    dim place;
    dim size;
    vector<ob> created;
    bool attack();
    bool defend();
    bool stop();
    int getHealth();
};

class building : public thing
{
    protected:
        int progress;
    public:
        char type;
        char weapon;
        int getProgress();
        bool build(int amount);
};

class marker : public obj
{
    public:
        bool isTeathered;
        obj teather;
        marker();
        marker(obj t);
};
class force : public obj
{
    public:
        dim direction;
        dim size;
        int power;
        force();
        force(dim stdir, int stpwr);
};
#endif
