#include "source.h"

Application* g_app = nullptr;
extern Application* CreateApplication();

int main()
{
	g_app = CreateApplication();
	g_app->Init();

	return 0;
}