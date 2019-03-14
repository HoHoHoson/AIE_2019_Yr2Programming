#include "ComputerGraphics.h"

int main()
{
	ComputerGraphics* cgApp = new ComputerGraphics();

	if (cgApp->startUp() == true)
	{
		while (cgApp->update() == true)
			cgApp->draw();

		cgApp->shutDown();
	}

	delete cgApp;
	return 0;
}