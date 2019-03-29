#include "ComputerGraphics.h"

int main()
{
	ComputerGraphics* cgApp = new ComputerGraphics();

	// Calculates Deltatime
	double previousTime = glfwGetTime();
	double currentTime = 0;
	double deltaTime = 0;

	// Loops the application as long as it's running
	if (cgApp->startUp() == true)
	{
		while (cgApp->update((float)deltaTime) == true)
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