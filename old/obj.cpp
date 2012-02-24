#include "data.h"

obj::obj()
{
    size.x = 10;
    size.y = 10;
    counter++;
    id = counter;
}
obj::obj(string image, string nm, int typ, dim tSize)
{
    img = image;
    name = nm;
    size = tSize;
    type = typ;
    counter++;
    id = counter;
}
bool obj::die()
{
    placeTo()
}
