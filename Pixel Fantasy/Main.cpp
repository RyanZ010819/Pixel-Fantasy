#include "Application.h"

int main()
{
	Config conf;
	Application app(conf);
	app.runLoop();
	return 0;
}
