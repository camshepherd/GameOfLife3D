#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#include "IntrepidLemming.h"

int main() {
	IntrepidLemming* theLemming = new IntrepidLemming();
	// cannot multithread stuff with the opengl stuff easily, needs to be in the main thread.
	//do not run the IntrepidLemming in its own thread for that reason
	theLemming->run();
}
