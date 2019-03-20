#include "ComputerGraphics.h"

int main()
{
	ComputerGraphics* cgApp = new ComputerGraphics();

	double previousTime = glfwGetTime();
	double currentTime = 0;
	double deltaTime = 0;

	if (cgApp->startUp() == true)
	{
		while (cgApp->update(deltaTime) == true)
		{
			currentTime = glfwGetTime();
			deltaTime = currentTime - previousTime;
			previousTime = currentTime;

			cgApp->draw();
		}

		cgApp->shutDown();
	}

	delete cgApp;
	return 0;
}