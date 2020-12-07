#include "Core.h"
#include "Application.h"

int main(int argc, char **argv)
{
	Q2Compilers::Application app(argc, argv, "Q2Compilers");
	app.Run();

	return 0;
}
