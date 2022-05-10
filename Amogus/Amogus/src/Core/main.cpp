#include "source.h"

Application* g_app = nullptr;
extern Application* CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	g_app = CreateApplication(argc, argv);
	g_app->Init();

	return 0;
}