#include "menus.h"

menu::menu(int lx, int *ly, int *sx, int sy, menu *prnt, menu hold[], int num)
{
	parent = prnt;
	if(hold != NULL)
		for(int i = 0; i < num; i++)
			holding.push_back(&(hold[i]));
	sizex=sx;
	sizey=sy;
	locx = lx;
	locy = ly;
}

dim menu::getSize()
{
	dim temp;
	if(sizex != NULL)
		temp.x = *sizex;
	else
		temp.x = 100;
	temp.y = sizey;
	return temp;
}

dim menu::getLoc()
{
	dim temp;
	temp.x = locx;
	if(locy == NULL)
		temp.y = 0;
	else
		temp.y = *locy;
	if(*locy > 0)
		temp.y -= sizey;
	return temp;
}

void menu::click()
{
	return;
}

