#include "PhysicsProjectv2App.h"

int main() {
	
	// allocation
	auto app = new PhysicsProjectv2App();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}