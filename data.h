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
//Character inherits from thing
class thing
{
    protected:
        static int nextId;
    public:
        bool on;
        int id;
        int colId;
        dim loc;
        dim size;

        void init(int locx, int locy, int insizex, int insizey);
        thing(int locx, int locy, int insizex, int insizey);
        thing();
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

class map
{
    public:
        string *characters;
        dim mapDim;
        int numChars;
        fstream levelFile;

        map(string fileName);
        vector<character>* update(vector<character> *objects);
        ~map();

};

#endif
