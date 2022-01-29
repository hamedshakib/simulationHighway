#include "qcoreapplication.h"
#include <qdebug.h>
#include "Simulation.h"
//#include "Highway.h"
//#include "Car.h"


int main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv);

	Simulation simulation(10,1200,250);
	simulation.Run();

	return app.exec();
}