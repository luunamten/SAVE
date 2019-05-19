#include "GLContext.h"
#include <stdexcept>
#include <iostream>

int main(int arg, char **args) {
	try {
		GLContext::initContext();
	}
	catch (std::runtime_error &e) {
		std::cout << e.what();
	}
	return	0;
}