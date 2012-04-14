#include <string>
#include <vector>
#ifndef DATA
#define DATA 

using namespace std;

struct dim
{
    int x;
    int y;
};
enum collisionType
{
     neither,
     xmove,
     ymove,
     both
};
enum hatType
{
	green,
	blue,
	red,
	purple
};

//Character inherits from thing
class thing
{
    protected:
        static int nextId;
    public:
        static vector<thing> known;
        bool on;
		bool updated;
		bool crash;
        int id;
        int colId;
		vector<int> colIds;
		dim colTry;
        dim loc;
        dim size;
        dim region;

        thing(int locx=0, int locy=0, int insizex=10, int insizey=10);
        bool aStar();
};

class character : public thing
{
    public:
        bool selected;
        dim dest;
        int speed;
        bool pause;
		int teamId;
		//int img;
		hatType type;

        character(int locx=0, int locy=0, int insizex=0, int insizey=0, int inspeed=0, int team=0, hatType t=green);
        bool atRest();
		//Not sure what I want to do with that yet...
        //bool collision();
};


collisionType collide(const thing object1, const thing object2, const dim test);
bool collide(const thing object1, const thing object2);

class enemy
{
	public:
		int teamId;
		string name;
		enemy(int team=1, string n="The Hipsters");
		bool update(vector<character> *objects, int width, int length);
};

class map
{
	private:
		dim biggest;
        fstream levelFile;
		vector<enemy> enemies;
    public:
		int width;
		int length;
		//Remove
        //string *characters;
        //dim mapDim;
        int numChars;
		vector<character> objects;

        map(string fileName);
        vector<character>* update();
        ~map();

};

class idlist
{
	private:
		vector<int> list;
	public:
		void init(vector<character> *objects);
		idlist(vector<character> *objects);
		int get(int colNum);
		void update(vector<character> *objects);
};


#endif
