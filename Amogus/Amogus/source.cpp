#include "source.h"

Init::Init(int argc, char* argv[]) {

	Render();
	Input();
}

void Init::Render() {

}

void Init::Input() {



}

void Init::Logging() {

	cout << "Input active." << endl;
	cout << "Input 0 to exit the service." << endl;
	cout << ">";
	int key = NULL;

	cin >> key;
	cin.get();

	if (key == 1) {
		//exit(0);
	}

}

Init::~Init(void) {

}