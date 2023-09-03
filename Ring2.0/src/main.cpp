#include "Core/Application.h"
#include "Core/Log.h"

int main()
{
	Log::Init();

	Application* app = new Application();

	app->Run();
	delete app;

	return 0;
}