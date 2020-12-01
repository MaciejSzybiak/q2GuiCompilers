#include "Core.h"
#include "Application.h"

int main() {
	LOG_TRACE("Test trace message");
	LOG_INFO("Test info message");
	LOG_WARNING("Test warning message");
	LOG_ERROR("Test error message");

	Q2Compilers::Application app(std::string("Q2Compilers"));
	app.Run();

	return 0;
}
