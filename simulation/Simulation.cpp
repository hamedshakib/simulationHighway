#include "Simulation.h"

Simulation::Simulation(unsigned int numberOfAllRound,double SimulationTime,double WarmUpTime,QObject *parent)
	: QObject(parent)
{
	rateOfTimeIncrease = round(rateOfTimeIncrease * 1000) / 1000;
	NumberOfAllRound = numberOfAllRound;
	AllSimulationTime= SimulationTime;
	AllWarmUpTime= WarmUpTime;
	highway = new Highway(this);
	senario1 = new Senario();
	senario2 = new Senario();

	senario1->MaximumSpeedAllowedInPlacesBeforeDisturbance = 11.2;
	senario2->MaximumSpeedAllowedInPlacesBeforeDisturbance = highway->MaxOfSpeedAllowed;

}

Simulation::~Simulation()
{
}

bool Simulation::ResetAll_ForNewRound()
{
	highway->IsThereDisorder = false;
	qDeleteAll(senario1->CarsInHighwaySenario);
	qDeleteAll(senario2->CarsInHighwaySenario);
	senario1->CarsInHighwaySenario.clear();
	senario2->CarsInHighwaySenario.clear();
	senario1->MaximumSpeedAllowedInPlacesBeforeDisturbance = highway->MaxOfSpeedAllowed;
	senario2->MaximumSpeedAllowedInPlacesBeforeDisturbance = highway->MaxOfSpeedAllowed;
	senario1->disorder.disorderStatus = Senario::Disorder::NoDisorder;
	senario2->disorder.disorderStatus = Senario::Disorder::NoDisorder;
	senario1->disorder.PlaceStartedAffectedByDisorder = highway->LocationOfDisorder;
	senario2->disorder.PlaceStartedAffectedByDisorder = highway->LocationOfDisorder;
	senario1->disorder.PlaceEndedAffectedByDisorder = highway->LocationOfDisorder;
	senario2->disorder.PlaceEndedAffectedByDisorder = highway->LocationOfDisorder;

	//qDeleteAll(CarsInHighwaySenario1);
	//qDeleteAll(CarsInHighwaySenario2);
	//CarsInHighwaySenario1.clear();
	//CarsInHighwaySenario2.clear();

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
		if (CurrentTime == senario1->TimeOfStartOfSourceOfDisorder)
		{
			senario1->disorder.disorderStatus = Senario::Disorder::CompleteDisorder;
//bb			senario2->disorder.disorderStatus = Senario::Disorder::CompleteDisorder;
		}
		else if(CurrentTime == senario1->TimeOfEndOfSourceOfDisorder)
		{
			senario1->disorder.disorderStatus = Senario::Disorder::SourceOfDisorderFixed;
//bb			senario2->disorder.disorderStatus = Senario::Disorder::SourceOfDisorderFixed;
		}

		if (TimeNextCarEnter == CurrentTime)
		{
			qDebug() << "Start To Create Car";
			senario1->CarsInHighwaySenario.insert(0, Car::ProcessEnterCarToHighway(highway));
//bb			senario2->CarsInHighwaySenario.insert(0, Car::ProcessEnterCarToHighway(highway));
			qDebug() << "Enter!";
			TimeNextCarEnter = NextCarArrivalTime();
		}


		//Scenario 1
		if(senario1->disorder.disorderStatus!=Senario::Disorder::NoDisorder)
			highway->MaximumSpeedAllowedInPlacesBeforeDisturbance = senario1->MaximumSpeedAllowedInPlacesBeforeDisturbance;
		else
			highway->MaximumSpeedAllowedInPlacesBeforeDisturbance = highway->MaxOfSpeedAllowed;
		for (int carNum = (senario1->CarsInHighwaySenario.count() - 1); carNum >= 0; carNum--)
		{
			Car::CarPosition* carPosition= senario1->CarsInHighwaySenario[carNum]->get_Position();
			senario1->CarsInHighwaySenario[carNum]->ProcessMoveCar(rateOfTimeIncrease, highway);
			ProcessAcceleration(rateOfTimeIncrease, senario1->CarsInHighwaySenario[carNum], highway, senario1);
			if (carPosition->backOfCar >= highway->LenghtOfHighway)
			{
				qDebug() << "delete Car";
				senario1->CarsInHighwaySenario[carNum]->deleteLater();
				//delete senario1->CarsInHighwaySenario[carNum];
				senario1->CarsInHighwaySenario.removeAt(carNum);
				qDebug() << "Exited";
			}
			/*if (carNum == (CarsInHighwaySenario1.count() - 1))
			{
				//qDebug() << "Speed:" << CarsInHighwaySenario1[carNum]->get_Speed();
				//qDebug() << carNum << " Pos:" << CarsInHighwaySenario1[carNum]->get_Position()->backOfCar << "," << CarsInHighwaySenario1[carNum]->get_Position()->frontOfCar;
				mova = carNum;
			}
			*/



			delete carPosition;
		}




		//Scenario 2
		highway->MaximumSpeedAllowedInPlacesBeforeDisturbance = 20;





		if(((int)(CurrentTime*1000))%1000==0)
			qDebug() <<"Time:" << CurrentTime;
		//qDebug() << senario1->CarsInHighwaySenario.count();
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
			senario1->CarsInHighwaySenario.insert(0, Car::ProcessEnterCarToHighway(highway));
			senario2->CarsInHighwaySenario.insert(0, Car::ProcessEnterCarToHighway(highway));
			//CarsInHighwaySenario1.insert(0, Car::ProcessEnterCarToHighway(highway));
			//CarsInHighwaySenario2.insert(0, Car::ProcessEnterCarToHighway(highway));
			TimeNextCarEnter = NextCarArrivalTime();
		}

		for (int carNum = (senario1->CarsInHighwaySenario.count()-1); carNum >= 0; carNum--)
		{
			Car::CarPosition* carPosition = senario1->CarsInHighwaySenario[carNum]->get_Position();
			senario1->CarsInHighwaySenario[carNum]->ProcessMoveCar(rateOfTimeIncrease, highway);
			ProcessAcceleration(rateOfTimeIncrease, senario1->CarsInHighwaySenario[carNum], highway, senario1);
			senario2->CarsInHighwaySenario[carNum]->ProcessMoveCar(rateOfTimeIncrease, highway);
			ProcessAcceleration(rateOfTimeIncrease, senario2->CarsInHighwaySenario[carNum], highway, senario2);
			if (carPosition->backOfCar >= highway->LenghtOfHighway)
			{
				senario1->CarsInHighwaySenario[carNum]->deleteLater();
				senario1->CarsInHighwaySenario.removeAt(carNum);
				senario2->CarsInHighwaySenario[carNum]->deleteLater();
				senario2->CarsInHighwaySenario.removeAt(carNum);
			}
			if (carNum == (senario1->CarsInHighwaySenario.count() - 1))
			{
				//qDebug() << "Speed:" << CarsInHighwaySenario1[carNum]->get_Speed();
				//qDebug() << carNum << " Pos:" << CarsInHighwaySenario1[carNum]->get_Position()->backOfCar << "," << CarsInHighwaySenario1[carNum]->get_Position()->frontOfCar;
				mova = carNum;
			}
			



			delete carPosition;
		}


	}
	qDebug() << "FF";
	//qDebug() << mova << " Pos:" << senario1->CarsInHighwaySenario[mova]->get_Position()->backOfCar << "," << senario1->CarsInHighwaySenario[mova]->get_Position()->frontOfCar;
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
	float ArrivalTime =ListOfTimeOfEntersCars[0];
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

bool Simulation::ProcessAcceleration(double time, Car* car,Highway* highway, Senario* senario)
{
	Car::CarPosition* carPosition = car->get_Position();
	if (senario->disorder.disorderStatus == Senario::Disorder::NoDisorder)
	{
		if (car->get_Speed() < highway->MaxOfSpeedAllowed)
		{
			car->set_Acceleration(car->MaxAcceleration);
		}
		else if (car->get_Speed() >= highway->MaxOfSpeedAllowed)
		{
			car->set_Acceleration(0);
			car->set_Speed(highway->MaxOfSpeedAllowed);
		}
	}
	else if (senario->disorder.disorderStatus == Senario::Disorder::CompleteDisorder || senario->disorder.disorderStatus == Senario::Disorder::SourceOfDisorderFixed)
	{
		//complete before Disorder
		if (carPosition->frontOfCar < (senario->disorder.PlaceStartedAffectedByDisorder - senario->LocationRangeNearToDisorder))
		{
			//senario 2 In Enter
			if (car->get_Speed() < highway->MaximumSpeedAllowedInPlacesBeforeDisturbance)
			{
				car->set_Acceleration(car->MaxAcceleration);
			}
			//senario 1 or 2
//			else if (car->get_Speed() == highway->MaximumSpeedAllowedInPlacesBeforeDisturbance)
/*need eslah*/ else if (abs(car->get_Speed() - highway->MaximumSpeedAllowedInPlacesBeforeDisturbance)< (car->MaxAcceleration * rateOfTimeIncrease))
			{
				car->set_Acceleration(0);
			}
			//senario 1 In keep to driving suddenly occer Disorder
			else if (car->get_Speed() > highway->MaximumSpeedAllowedInPlacesBeforeDisturbance)
			{
				car->set_Acceleration(-1*car->MaxAcceleration);
			}
		}
		//near Disorder
		else if (carPosition->frontOfCar < (senario->disorder.PlaceStartedAffectedByDisorder))
		{
			//senario 1 and 2
			if (car->get_Speed() > highway->SpeedInDisruption)
			{
				car->set_Acceleration(-1 * car->MaxAcceleration);
			}
			//senario 1 and 2 In Reached to speed of Disorder
			else if (car->get_Speed() == highway->SpeedInDisruption)
			{
				car->set_Acceleration(0);
			}
		}
		//in Disorder
		else if (carPosition->frontOfCar < (senario->disorder.PlaceEndedAffectedByDisorder))
		{
			car->set_Acceleration(0);
		}
		//after Disorder
		else if (carPosition->backOfCar >= (senario->disorder.PlaceEndedAffectedByDisorder))
		{
			if (car->get_Speed() < highway->MaxOfSpeedAllowed)
			{
				car->set_Acceleration(car->MaxAcceleration);
			}
		}
	}
	/*
	else if (senario->disorder.disorderStatus == Senario::Disorder::SourceOfDisorderFixed)
	{





	}*/



	delete carPosition;
	car->ApplyAcceleration(time);
	return true;
}

bool Simulation::ProcessDeterminetePlaceAffectedByDisorder(Senario* senario)
{
	/*
	if (senario->disorder.disorderStatus == Senario::Disorder::CompleteDisorder)
	{
		senario->disorder.PlaceStartedAffectedByDisorder=
	}
	else if (senario->disorder.disorderStatus == Senario::Disorder::SourceOfDisorderFixed)
	{

	}
	*/
	return 0;
}
