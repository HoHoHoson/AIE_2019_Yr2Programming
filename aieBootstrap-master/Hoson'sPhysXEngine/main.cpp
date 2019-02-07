#include "Hoson_sPhysXEngineApp.h"

int main() {
	
	// allocation
	auto app = new Hoson_sPhysXEngineApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}