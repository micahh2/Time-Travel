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
		dim colTry;
        dim loc;
        dim size;
        dim region;

        void init(int locx, int locy, int insizex, int insizey);
        thing(int locx, int locy, int insizex, int insizey);
        thing();
        bool aStar();
};

class character : public thing
{
    public:
        bool selected;
        dim dest;
        int speed;
        bool pause;

        character(int locx, int locy);
        character(int locx, int locy, int insizex, int insizey);
        character(int locx, int locy, int insizex, int insizey, int inspeed);
        void init(int locx, int locy, int inspeed);
        bool collision();
};


collisionType collide(const thing object1, const thing object2, const dim test);
bool collide(const thing object1, const thing object2);

class map
{
    public:
        string *characters;
        dim mapDim;
        int numChars;
        fstream levelFile;

        map(string fileName);
        vector<character>* update(vector<character> *objects, int length, int width, dim biggest);
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
