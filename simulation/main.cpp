#include "qcoreapplication.h"
#include <qdebug.h>
#include "Simulation.h"
#include "Highway.h"


int main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv);

	Highway hiway;
	int sum = 0;
	for (int i = 0; i < 10; i++)
	{
		sum+=hiway.GenerateTimeUntilEnterNextCar();
	}
	qDebug() << sum;

	//Simulation simulation(4);
	//simulation.Run();

	return app.exec();
}