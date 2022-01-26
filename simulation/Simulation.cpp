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
	qDeleteAll(CarsInHighwaySenario1);
	qDeleteAll(CarsInHighwaySenario2);
	CarsInHighwaySenario1.clear();
	CarsInHighwaySenario2.clear();

	ListOfTimeOfEntersCars.clear();

	return true;
}

/*
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
*/

bool Simulation::Run()
{
	qDebug() << "Run";
	for (CurrentRoundNumber = 1; CurrentRoundNumber <= NumberOfAllRound; CurrentRoundNumber++)
	{
		ResetAll_ForNewRound();
		GenerateArrivalTimeOfAllCar(1000);
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
		if (TimeNextCarEnter == CurrentTime)
		{
			CarsInHighwaySenario1.insert(0, Car::ProcessEnterCarToHighway(highway));
			CarsInHighwaySenario2.insert(0, Car::ProcessEnterCarToHighway(highway));
			TimeNextCarEnter = NextCarArrivalTime();
		}
		//Scenario 1
		highway->MaximumSpeedAllowedInPlacesBeforeDisturbance = 11.2;
		for (int carNum = (CarsInHighwaySenario1.count() - 1); carNum >= 0; carNum--)
		{
			CarsInHighwaySenario1[carNum]->ProcessMoveCar(rateOfTimeIncrease, highway);
			if (CarsInHighwaySenario1[carNum]->get_Position()->backOfCar >= highway->LenghtOfHighway)
			{
				CarsInHighwaySenario1[carNum]->deleteLater();
				CarsInHighwaySenario1.removeAt(carNum);
			}
			/*if (carNum == (CarsInHighwaySenario1.count() - 1))
			{
				//qDebug() << "Speed:" << CarsInHighwaySenario1[carNum]->get_Speed();
				//qDebug() << carNum << " Pos:" << CarsInHighwaySenario1[carNum]->get_Position()->backOfCar << "," << CarsInHighwaySenario1[carNum]->get_Position()->frontOfCar;
				mova = carNum;
			}
			*/




		}




		//Scenario 2
		highway->MaximumSpeedAllowedInPlacesBeforeDisturbance = 20;




		if(CurrentTime==100)
		qDebug() << CurrentTime;
	}
	return true;
}

void Simulation::WarmUp()
{
	int mova;
	//Car* firstCar = Car::ProcessEnterCarToHighway(highway);
	highway->MaximumSpeedAllowedInPlacesBeforeDisturbance = 20;
	qDebug() << "**";
	CurrentTime = -AllWarmUpTime;
	//CarsInHighway.insert(0,Car::ProcessEnterCarToHighway(highway));
	TimeNextCarEnter = NextCarArrivalTime();
	qDebug() << "***"<< TimeNextCarEnter;
	for (float CurrentWarmUpTime = -AllWarmUpTime;((CurrentWarmUpTime = round(CurrentWarmUpTime * 1000) / 1000))< 0; CurrentWarmUpTime += rateOfTimeIncrease)
	{
		//qDebug() << "Time:" << CurrentWarmUpTime <<"nextTime:"<< TimeNextCarEnter;
		CurrentTime = CurrentWarmUpTime;
		if (TimeNextCarEnter == CurrentWarmUpTime)
		{
			CarsInHighwaySenario1.insert(0, Car::ProcessEnterCarToHighway(highway));
			CarsInHighwaySenario2.insert(0, Car::ProcessEnterCarToHighway(highway));
			TimeNextCarEnter = NextCarArrivalTime();
		}

		for (int carNum = (CarsInHighwaySenario1.count()-1); carNum >= 0; carNum--)
		{
			CarsInHighwaySenario1[carNum]->ProcessMoveCar(rateOfTimeIncrease,highway);
			CarsInHighwaySenario2[carNum]->ProcessMoveCar(rateOfTimeIncrease, highway);
			if (CarsInHighwaySenario1[carNum]->get_Position()->backOfCar >= highway->LenghtOfHighway)
			{
				CarsInHighwaySenario1[carNum]->deleteLater();
				CarsInHighwaySenario1.removeAt(carNum);
				CarsInHighwaySenario2[carNum]->deleteLater();
				CarsInHighwaySenario2.removeAt(carNum);
			}
			/*if (carNum == (CarsInHighwaySenario1.count() - 1))
			{
				//qDebug() << "Speed:" << CarsInHighwaySenario1[carNum]->get_Speed();
				//qDebug() << carNum << " Pos:" << CarsInHighwaySenario1[carNum]->get_Position()->backOfCar << "," << CarsInHighwaySenario1[carNum]->get_Position()->frontOfCar;
				mova = carNum;
			}
			*/




		}


	}
	qDebug() << mova << " Pos:" << CarsInHighwaySenario1[mova]->get_Position()->backOfCar << "," << CarsInHighwaySenario1[mova]->get_Position()->frontOfCar;
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
	double ArrivalTime =ListOfTimeOfEntersCars[0];
	ListOfTimeOfEntersCars.pop_front();
	ArrivalTime = round(ArrivalTime * 1000) / 1000;
	return ArrivalTime;
}


void Simulation::GenerateArrivalTimeOfAllCar(int count)
{
	float Time = -AllWarmUpTime;
	float TimeNextCarArrival;
	ListOfTimeOfEntersCars.append(Time);
	for (int i = 0; i < count; i++)
	{
		double beet = GenerateTimeUntilEnterNextCar();
		TimeNextCarArrival = Time + beet;
		Time = TimeNextCarArrival;
		ListOfTimeOfEntersCars.append(TimeNextCarArrival);
	}
}

int Simulation::CheckAndApplyEvents()
{
	
	return 0;
}
