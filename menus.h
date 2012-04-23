#include "data.h"
#ifndef MENUS
#define MENUS
class menu : public thing
{
	private:
		vector<menu*> holding;
		menu *parent;
		int *sizex;
		int sizey;
		int locx;
		int *locy;

	public:
		//Menu
		menu(int lx=0, int* ly=NULL, int *sx=NULL, int sy = 10, menu *prnt = NULL, menu hold[] = NULL, int num = 0);
		dim getSize();
		dim getLoc();
		void click();
};
#endif
