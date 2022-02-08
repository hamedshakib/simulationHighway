#include "qcoreapplication.h"
#include "Simulation.h"

int main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv);

	Simulation simulation(10,1500,250);
	simulation.Run();

	return app.exec();
}