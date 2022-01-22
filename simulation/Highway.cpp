#include "Highway.h"

Highway::Highway(QObject *parent)
	: QObject(parent)
{
	NumberOfCarEnterInHour = 10;

	PercentageOfSlopeDistance=0.10;
	PercentageOfAccelerationDistance=0.10;
	PercentageOfSurfaceResistanceDistance=1;
	PercentageOfAerodynamicResistanceDistance=1;
	PercentageOfBendResistanceDistance=0;

}

Highway::~Highway()
{
}

double Highway::GenerateTimeUntilEnterNextCar()
{
	std::random_device rd;
	std::mt19937 genenator(rd());

	//double lamda = 1 / NumberOfCarEnterInHour;
	std::poisson_distribution<> poisson(NumberOfCarEnterInHour);

	//double TimeUntilEnterNextCar =poisson.operator()(genenator);
	double TimeUntilEnterNextCar =poisson(genenator);
	qDebug() << TimeUntilEnterNextCar;
	return TimeUntilEnterNextCar;
	//return 0;
}
