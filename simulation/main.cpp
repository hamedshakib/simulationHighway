#include "qcoreapplication.h"
#include <qdebug.h>
#include "Simulation.h"
//#include "Highway.h"
//#include "Car.h"


int main(int argc, char* argv[])
{
	QCoreApplication app(argc, argv);

	/*
	Highway hiway;
	int sum = 0;
	for (int i = 0; i < 10; i++)
	{
		sum+=hiway.GenerateTimeUntilEnterNextCar();
	}
	qDebug() << sum;
	*/

	
	/*
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(28.48, 5.0);

	for (int i = 0; i < 100; i++)
	{
		
	qDebug()<<distribution(generator);
	}
	*/



	//qDebug() << "Before start";
	Simulation simulation(4,3600,250);
	simulation.Run();

	return app.exec();
}