#include <Amogus.h>

class Runtime : public Application
{
public:

	void onInit() override
	{
		
	}

	void onUpdate(float dt) override
	{

	}

	void onRender(float dt) override
	{

	}

	void onImGui() override
	{
		
	}
};

Application* CreateApplication(int argc, char** argv)
{
	return new Runtime();
}