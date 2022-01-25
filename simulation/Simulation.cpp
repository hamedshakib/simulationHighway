#include "Simulation.h"

Simulation::Simulation(unsigned int numberOfAllRound,double SimulationTime,double WarmUpTime,QObject *parent)
	: QObject(parent)
{
	NumberOfAllRound = numberOfAllRound;
	AllSimulationTime= SimulationTime;
	AllWarmUpTime= WarmUpTime;
}

Simulation::~Simulation()
{
}

bool Simulation::ResetAll_ForNewRound()
{
	qDeleteAll(CarsInHighway);
	CarsInHighway.clear();
	return true;
}

bool Simulation::Run()
{
	for (CurrentRoundNumber = 1 ; CurrentRoundNumber <= NumberOfAllRound; CurrentRoundNumber++)
	{
		ResetAll_ForNewRound();
		SimulationOfOneRound();
		qDebug() << "END Round" << CurrentRoundNumber;
	}
	return true;
}

bool Simulation::SimulationOfOneRound()
{
	for (CurrentTime = 0; CurrentTime <= AllSimulationTime; CurrentTime+=0.001)
	{

		CurrentTime = round(CurrentTime * 1000) / 1000;
		if(CurrentTime==100)
		qDebug() << CurrentTime;
	}
	return true;
}

void Simulation::WarmUp()
{
	for (float CurrentWarmUpTime = 0; CurrentWarmUpTime <= AllWarmUpTime; CurrentWarmUpTime += 0.001)
	{
		CurrentWarmUpTime = round(CurrentWarmUpTime * 1000) / 1000;

	}
}


double Simulation::GenerateTimeUntilEnterNextCar(Highway *highway)
{
	std::random_device rd;
	std::mt19937 genenator(rd());

	//double lamda = 1 / NumberOfCarEnterInHour;
	std::poisson_distribution<> poisson(highway->NumberOfCarEnterInHour);

	//double TimeUntilEnterNextCar =poisson.operator()(genenator);
	double TimeUntilEnterNextCar = poisson(genenator);
	qDebug() << TimeUntilEnterNextCar;
	return TimeUntilEnterNextCar;
	//return 0;
}

float Simulation::NextCarArrivalTime(Highway* highway)
{
	highway
}
