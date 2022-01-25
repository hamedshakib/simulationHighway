#include "Simulation.h"

Simulation::Simulation(unsigned int numberOfAllRound,double SimulationTime,double WarmUpTime,QObject *parent)
	: QObject(parent)
{
	NumberOfAllRound = numberOfAllRound;
	AllSimulationTime= SimulationTime;
	AllWarmUpTime= WarmUpTime;
	highway = new Highway(this);
}

Simulation::~Simulation()
{
}

bool Simulation::ResetAll_ForNewRound()
{
	highway->IsThereDisorder = false;
	qDeleteAll(CarsInHighway);
	CarsInHighway.clear();
	return true;
}

bool Simulation::Run()
{
	//Scenario 1
	for (CurrentRoundNumber = 1; CurrentRoundNumber <= NumberOfAllRound; CurrentRoundNumber++)
	{
		ResetAll_ForNewRound();
		SimulationOfOneRound();
		qDebug() << "END Round" << CurrentRoundNumber;
	}







	//Scenario 2
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
	WarmUp();
	for (CurrentTime = 0; (CurrentTime = round(CurrentTime * 1000) / 1000) <= AllSimulationTime; CurrentTime+= rateOfTimeIncrease)
	{
		


		if(CurrentTime==100)
		qDebug() << CurrentTime;
	}
	return true;
}

void Simulation::WarmUp()
{
	//Car* firstCar = Car::ProcessEnterCarToHighway(highway);
	CarsInHighway.insert(0,Car::ProcessEnterCarToHighway(highway));
	NextCarArrivalTime();
	for (float CurrentWarmUpTime = -AllWarmUpTime;((CurrentWarmUpTime = round(CurrentWarmUpTime * 1000) / 1000))< 0; CurrentWarmUpTime += rateOfTimeIncrease)
	{





	}
}


double Simulation::GenerateTimeUntilEnterNextCar()
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

float Simulation::NextCarArrivalTime()
{
	//highway
	return 0;
}

int Simulation::CheckAndApplyEvents()
{
	if()
	return 0;
}
