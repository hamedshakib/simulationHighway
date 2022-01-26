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
	qDebug() << "Run";
	//Scenario 1
	for (CurrentRoundNumber = 1; CurrentRoundNumber <= NumberOfAllRound; CurrentRoundNumber++)
	{
		ResetAll_ForNewRound();
		SimulationOfOneRound();
		qDebug() << "Scenario 1 END Round" << CurrentRoundNumber;
	}







	//Scenario 2
	for (CurrentRoundNumber = 1 ; CurrentRoundNumber <= NumberOfAllRound; CurrentRoundNumber++)
	{
		ResetAll_ForNewRound();
		SimulationOfOneRound();
		qDebug() << "Scenario 2 END Round" << CurrentRoundNumber;
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
	qDebug() << "***";
	CurrentTime = -AllWarmUpTime;
	CarsInHighway.insert(0,Car::ProcessEnterCarToHighway(highway));
	TimeNextCarEnter=NextCarArrivalTime();
	qDebug() << "***"<< TimeNextCarEnter;
	for (float CurrentWarmUpTime = -AllWarmUpTime;((CurrentWarmUpTime = round(CurrentWarmUpTime * 1000) / 1000))< 0; CurrentWarmUpTime += rateOfTimeIncrease)
	{
		qDebug() << "Time:" << CurrentTime;
		CurrentTime = CurrentWarmUpTime;
		if (TimeNextCarEnter == CurrentWarmUpTime)
		{
			CarsInHighway.insert(0, Car::ProcessEnterCarToHighway(highway));
			TimeNextCarEnter = NextCarArrivalTime();
		}

		for (int carNum = (CarsInHighway.count()-1); carNum >= 0; carNum--)
		{
			CarsInHighway[carNum]->ProcessMoveCar(rateOfTimeIncrease);
			qDebug() << carNum << " Pos:" << CarsInHighway[carNum]->get_Position()->backOfCar << "," << CarsInHighway[carNum]->get_Position()->frontOfCar;




		}


	}
}


/*
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
*/

double Simulation::GenerateTimeUntilEnterNextCar()
{
	std::random_device rd;
	std::mt19937 genenator(rd());


	//std::default_random_engine generator;
	std::normal_distribution<double> distribution(highway->AverageTimeBetweenTwoCarArrivals, 2.50);

	double TimeUntilEnterNextCar = distribution.operator()(genenator);
	//double TimeUntilEnterNextCar = distribution(generator);

	return TimeUntilEnterNextCar;
}

float Simulation::NextCarArrivalTime()
{
	float TimeNextCarArrival = CurrentTime + GenerateTimeUntilEnterNextCar();
	qDebug() << "AAAT:" << CurrentTime;
	qDebug() << "BBB:" << GenerateTimeUntilEnterNextCar();
	TimeNextCarArrival = round(TimeNextCarArrival * 1000) / 1000;
	qDebug() << "CCC:" << TimeNextCarArrival;
	return TimeNextCarArrival;
}

int Simulation::CheckAndApplyEvents()
{
	
	return 0;
}
