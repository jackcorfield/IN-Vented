 #include "source.h"

Application* g_app = nullptr;

int main() 
{
	g_app = new Application();
	g_app->Init();

	return 0;
}