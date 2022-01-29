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
	senario1->disorder.disorderStatus = Senario::Disorder::NoDisorder;
	senario2->disorder.disorderStatus = Senario::Disorder::NoDisorder;
	senario1->MaximumSpeedAllowedInPlacesBeforeDisturbance = 11.2;
	senario2->MaximumSpeedAllowedInPlacesBeforeDisturbance = highway->MaxOfSpeedAllowed;
	senario1->disorder.PlaceStartedAffectedByDisorder = highway->LocationOfDisorder;
	senario2->disorder.PlaceStartedAffectedByDisorder = highway->LocationOfDisorder;
	senario1->disorder.PlaceEndedAffectedByDisorder = highway->LocationOfDisorder;
	senario2->disorder.PlaceEndedAffectedByDisorder = highway->LocationOfDisorder;
	senario1->TotalFuelConsumption = 0;
	senario2->TotalFuelConsumption = 0;
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
	//qDebug() << "Run";
	for (CurrentRoundNumber = 1; CurrentRoundNumber <= NumberOfAllRound; CurrentRoundNumber++)
	{
		if (CurrentRoundNumber > 1)
		{
			qDebug() << " fuel Consumption senaroi 1:" << senario1->TotalFuelConsumption;
			qDebug() << " fuel Consumption senaroi 2:" << senario2->TotalFuelConsumption;
		}
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
			//qDebug() << "YYYYYYYYYYYYYYY";
			senario1->disorder.disorderStatus = Senario::Disorder::CompleteDisorder;
			senario2->disorder.disorderStatus = Senario::Disorder::CompleteDisorder;
		}
		else if(CurrentTime == senario1->TimeOfEndOfSourceOfDisorder)
		{
			senario1->disorder.disorderStatus = Senario::Disorder::SourceOfDisorderFixed;
			senario2->disorder.disorderStatus = Senario::Disorder::SourceOfDisorderFixed;
		}

		if (TimeNextCarEnter == CurrentTime)
		{
//bb			qDebug() << "Start To Create Car";
			senario1->CarsInHighwaySenario.insert(0, Car::ProcessEnterCarToHighway(highway));
			senario2->CarsInHighwaySenario.insert(0, Car::ProcessEnterCarToHighway(highway));
//bb			qDebug() << "Enter!";
			TimeNextCarEnter = NextCarArrivalTime();
		}
		//Test
		/*
		if (((int)(CurrentTime * 1000)) % 1000 == 0) {
			qDebug() << "Time:" << CurrentTime;
		}
		*/

		//Scenario 1
		if(senario1->disorder.disorderStatus!=Senario::Disorder::NoDisorder)
			highway->MaximumSpeedAllowedInPlacesBeforeDisturbance = senario1->MaximumSpeedAllowedInPlacesBeforeDisturbance;
		else
			highway->MaximumSpeedAllowedInPlacesBeforeDisturbance = highway->MaxOfSpeedAllowed;
		if ((senario1->disorder.PlaceEndedAffectedByDisorder-senario1->disorder.PlaceStartedAffectedByDisorder)==0 &&CurrentTime > senario1->TimeOfEndOfSourceOfDisorder)
		{
			senario1->disorder.disorderStatus = Senario::Disorder::NoDisorder;
		}
		for (int carNum = (senario1->CarsInHighwaySenario.count() - 1); carNum >= 0; carNum--)
		{
			Car::CarPosition* carPosition= senario1->CarsInHighwaySenario[carNum]->get_Position();
			double movementAmount =senario1->CarsInHighwaySenario[carNum]->ProcessMoveCar(rateOfTimeIncrease, highway);
			ProcessAcceleration(rateOfTimeIncrease, senario1->CarsInHighwaySenario[carNum], highway, senario1);
			senario1->TotalFuelConsumption += FuelConsumption::Calculate(movementAmount, senario1->CarsInHighwaySenario[carNum],highway);
			ProcessDeterminetePlaceAffectedByDisorder(senario1);
			if (carPosition->backOfCar >= highway->LenghtOfHighway)
			{
//				qDebug() << "delete Car";
				senario1->CarsInHighwaySenario[carNum]->deleteLater();
				//delete senario1->CarsInHighwaySenario[carNum];
				senario1->CarsInHighwaySenario.removeAt(carNum);
//				qDebug() << "Exited";
			}

			/////Test start
			/*
			if (((int)(CurrentTime * 1000)) % 1000 == 0) {
				qDebug() << "Car " << carNum << "Pos:" << carPosition->frontOfCar << " Speed:" << senario1->CarsInHighwaySenario[carNum]->get_Speed();
			}
			*/




			/*if (carNum == (CarsInHighwaySenario1.count() - 1))
			{
				//qDebug() << "Speed:" << CarsInHighwaySenario1[carNum]->get_Speed();
				//qDebug() << carNum << " Pos:" << CarsInHighwaySenario1[carNum]->get_Position()->backOfCar << "," << CarsInHighwaySenario1[carNum]->get_Position()->frontOfCar;
				mova = carNum;
			}
			*/



			delete carPosition;
		}

		//test
		/*
		if (((int)(CurrentTime * 1000)) % 1000 == 0) {
			_sleep(100);
			qDebug() << "Cur:" << CurrentTime;
			qDebug() << "Disorder start:" << senario1->disorder.PlaceStartedAffectedByDisorder << " End:" << senario1->disorder.PlaceEndedAffectedByDisorder;
			system("cls");
		}
		*/


		//Scenario 2
		highway->MaximumSpeedAllowedInPlacesBeforeDisturbance = 20;
		if ((senario2->disorder.PlaceEndedAffectedByDisorder - senario2->disorder.PlaceStartedAffectedByDisorder) == 0 && CurrentTime > senario2->TimeOfEndOfSourceOfDisorder)
		{
			senario2->disorder.disorderStatus = Senario::Disorder::NoDisorder;
		}
		for (int carNum = (senario2->CarsInHighwaySenario.count() - 1); carNum >= 0; carNum--)
		{
			Car::CarPosition* carPosition = senario2->CarsInHighwaySenario[carNum]->get_Position();
			double movementAmount = senario2->CarsInHighwaySenario[carNum]->ProcessMoveCar(rateOfTimeIncrease, highway);
			ProcessAcceleration(rateOfTimeIncrease, senario2->CarsInHighwaySenario[carNum], highway, senario2);
			senario2->TotalFuelConsumption += FuelConsumption::Calculate(movementAmount, senario2->CarsInHighwaySenario[carNum], highway);
			ProcessDeterminetePlaceAffectedByDisorder(senario2);
			if (carPosition->backOfCar >= highway->LenghtOfHighway)
			{
				//				qDebug() << "delete Car";
				senario2->CarsInHighwaySenario[carNum]->deleteLater();
				//delete senario1->CarsInHighwaySenario[carNum];
				senario2->CarsInHighwaySenario.removeAt(carNum);
				//				qDebug() << "Exited";
			}

			/////Test start
			//if (((int)(CurrentTime * 1000)) % 1000 == 0) {
			//	qDebug() << "Car " << carNum << "Pos:" << carPosition->frontOfCar << " Speed:" << senario2->CarsInHighwaySenario[carNum]->get_Speed();
			//}




			/*if (carNum == (CarsInHighwaySenario1.count() - 1))
			{
				//qDebug() << "Speed:" << CarsInHighwaySenario1[carNum]->get_Speed();
				//qDebug() << carNum << " Pos:" << CarsInHighwaySenario1[carNum]->get_Position()->backOfCar << "," << CarsInHighwaySenario1[carNum]->get_Position()->frontOfCar;
				mova = carNum;
			}
			*/



			delete carPosition;
		}



		//if(((int)(CurrentTime*1000))%1000==0)
		//	qDebug() <<"Time:" << CurrentTime;
		//qDebug() << senario1->CarsInHighwaySenario.count();
	}
	return true;
}

void Simulation::WarmUp()
{
	int mova;
	//Car* firstCar = Car::ProcessEnterCarToHighway(highway);
	highway->MaximumSpeedAllowedInPlacesBeforeDisturbance = 20;
//	qDebug() << "**";
	CurrentTime = -AllWarmUpTime;
	//CarsInHighway.insert(0,Car::ProcessEnterCarToHighway(highway));
	TimeNextCarEnter = NextCarArrivalTime();
//	qDebug() << "***"<< TimeNextCarEnter;
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
	//qDebug() << "FF";
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
	//No Disorder
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
		if (carPosition->frontOfCar < (senario->disorder.PlaceStartedAffectedByDisorder - senario->MaximumDistanceToSlowDownForDisturbance*(highway->MaximumSpeedAllowedInPlacesBeforeDisturbance/highway->MaxOfSpeedAllowed)))
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
				car->set_Speed(highway->MaximumSpeedAllowedInPlacesBeforeDisturbance);
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
			else if (abs(car->get_Speed()- highway->SpeedInDisruption)< (car->MaxAcceleration * rateOfTimeIncrease))
			{
				car->set_Acceleration(0);
				car->set_Speed(highway->SpeedInDisruption);

			}
		}
		//in Disorder
		else if (carPosition->frontOfCar < (senario->disorder.PlaceEndedAffectedByDisorder))
		{
			car->set_Acceleration(0);
			car->set_Speed(highway->SpeedInDisruption);
		}
		//after Disorder
		else if (carPosition->backOfCar >= (senario->disorder.PlaceEndedAffectedByDisorder))
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
	
	if (senario->disorder.disorderStatus == Senario::Disorder::CompleteDisorder)
	{
		bool IsThereCarInDisorder = false;
		for (int carNum = 0; carNum < senario->CarsInHighwaySenario.count(); carNum++)
		{
			if (senario->CarsInHighwaySenario[carNum]->get_Speed() == highway->SpeedInDisruption)
			{
				Car::CarPosition* carPos = senario->CarsInHighwaySenario[carNum]->get_Position();
				senario->disorder.PlaceStartedAffectedByDisorder = carPos->backOfCar- 1;
				IsThereCarInDisorder = true;
				delete carPos;
				break;
			}
		}
		if (IsThereCarInDisorder == false)
		{
			senario->disorder.PlaceStartedAffectedByDisorder = senario->disorder.PlaceEndedAffectedByDisorder;
		}
	}
	else if (senario->disorder.disorderStatus == Senario::Disorder::SourceOfDisorderFixed)
	{
		bool IsThereCarInDisorder = false;
		for (int carNum = 0; carNum < senario->CarsInHighwaySenario.count(); carNum++)
		{
			if (senario->CarsInHighwaySenario[carNum]->get_Speed() == highway->SpeedInDisruption)
			{
				Car::CarPosition* carPos = senario->CarsInHighwaySenario[carNum]->get_Position();
				if (IsThereCarInDisorder == false)
				{
					senario->disorder.PlaceStartedAffectedByDisorder = carPos->backOfCar + 1;
					IsThereCarInDisorder = true;
				}
				senario->disorder.PlaceStartedAffectedByDisorder= carPos->frontOfCar;
				delete carPos;
			}
		}
		if (IsThereCarInDisorder == false)
		{
			senario->disorder.disorderStatus = Senario::Disorder::NoDisorder;
		}
	}
	
	return 0;
}
