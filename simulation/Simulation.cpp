#include "Simulation.h"

Simulation::Simulation(unsigned int numberOfAllRound,double MaxSimulationTime,double WarmUpTime,QObject *parent)
	: QObject(parent)
{
	rateOfTimeIncrease = round(rateOfTimeIncrease * 1000) / 1000;
	NumberOfAllRound = numberOfAllRound;
	MaxMainSimulationTime = MaxSimulationTime;
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
	senario1->disorder.lastCarInDisorder = nullptr;
	senario2->disorder.lastCarInDisorder = nullptr;
	senario1->TotalFuelConsumption = 0;
	senario2->TotalFuelConsumption = 0;
	senario1->disorder.MaxLengthOfDisorder=0;
	senario2->disorder.MaxLengthOfDisorder=0;

	ListOfTimeOfEntersCars.clear();

	return true;
}


bool Simulation::Run()
{
	for (CurrentRoundNumber = 1; CurrentRoundNumber <= NumberOfAllRound; CurrentRoundNumber++)
	{
		ResetAll_ForNewRound();
		GenerateArrivalTimeOfAllCar(1000);
		SimulationOfOneRound();

		
		qInfo() << "END Round" << CurrentRoundNumber;
		qInfo() << "Main Simulation Time:" << CurrentTime<<"s";
		qInfo() << " fuel Consumption senaroi 1:" << senario1->TotalFuelConsumption<<" J";
		qInfo() << " fuel Consumption senaroi 2:" << senario2->TotalFuelConsumption<<" J";
		qInfo() << " amount of Consumption savings:"<< senario2->TotalFuelConsumption -senario1->TotalFuelConsumption << " J";
		qInfo() << " Percentage of Consumption savings" << (senario2->TotalFuelConsumption - senario1->TotalFuelConsumption) / (senario2->TotalFuelConsumption)<<"%\n";
	}

	return true;
}

bool Simulation::SimulationOfOneRound()
{
	WarmUp();
	
	for (CurrentTime = 0; (CurrentTime = round(CurrentTime * 1000) / 1000) <= MaxMainSimulationTime; CurrentTime+= rateOfTimeIncrease)
	{
		if (CurrentTime == senario1->TimeOfStartOfSourceOfDisorder)
		{
			senario1->disorder.disorderStatus = Senario::Disorder::CompleteDisorder;
			senario2->disorder.disorderStatus = Senario::Disorder::CompleteDisorder;
		}
		else if(CurrentTime == senario1->TimeOfEndOfSourceOfDisorder)
		{
			senario1->disorder.disorderStatus = Senario::Disorder::SourceOfDisorderFixed;
			senario2->disorder.disorderStatus = Senario::Disorder::SourceOfDisorderFixed;
		}
		else if (CurrentTime > senario1->TimeOfEndOfSourceOfDisorder)
		{
			if (senario1->disorder.disorderStatus == Senario::Disorder::NoDisorder && senario2->disorder.disorderStatus == Senario::Disorder::NoDisorder)
			{
				return true;
			}
		}


		if (TimeNextCarEnter == CurrentTime)
		{
			senario1->CarsInHighwaySenario.insert(0, Car::ProcessEnterCarToHighway(highway));
			senario2->CarsInHighwaySenario.insert(0, Car::ProcessEnterCarToHighway(highway));
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
			double movementAmount =senario1->CarsInHighwaySenario[carNum]->ProcessMoveCar(rateOfTimeIncrease, highway);
			ProcessAcceleration(rateOfTimeIncrease, senario1->CarsInHighwaySenario[carNum], highway, senario1);
			senario1->TotalFuelConsumption += FuelConsumption::Calculate(movementAmount, senario1->CarsInHighwaySenario[carNum],highway);
			ProcessDeterminetePlaceAffectedByDisorder(senario1);
			if (carPosition->backOfCar >= highway->LenghtOfHighway)
			{
				senario1->CarsInHighwaySenario[carNum]->deleteLater();
				senario1->CarsInHighwaySenario.removeAt(carNum);
			}
			delete carPosition;
		}



		//Scenario 2
		highway->MaximumSpeedAllowedInPlacesBeforeDisturbance = 20;
		for (int carNum = (senario2->CarsInHighwaySenario.count() - 1); carNum >= 0; carNum--)
		{
			Car::CarPosition* carPosition = senario2->CarsInHighwaySenario[carNum]->get_Position();
			double movementAmount = senario2->CarsInHighwaySenario[carNum]->ProcessMoveCar(rateOfTimeIncrease, highway);
			ProcessAcceleration(rateOfTimeIncrease, senario2->CarsInHighwaySenario[carNum], highway, senario2);
			senario2->TotalFuelConsumption += FuelConsumption::Calculate(movementAmount, senario2->CarsInHighwaySenario[carNum], highway);
			ProcessDeterminetePlaceAffectedByDisorder(senario2);
			if (carPosition->backOfCar >= highway->LenghtOfHighway)
			{
				senario2->CarsInHighwaySenario[carNum]->deleteLater();
				senario2->CarsInHighwaySenario.removeAt(carNum);
			}
			delete carPosition;
		}

	}
	return true;
}

void Simulation::WarmUp()
{
	highway->MaximumSpeedAllowedInPlacesBeforeDisturbance = 20;
	CurrentTime = -AllWarmUpTime;
	TimeNextCarEnter = NextCarArrivalTime();
	for (float CurrentWarmUpTime = -AllWarmUpTime;((CurrentWarmUpTime = round(CurrentWarmUpTime * 1000) / 1000))< 0; CurrentWarmUpTime += rateOfTimeIncrease)
	{
		CurrentTime = CurrentWarmUpTime;
		if (TimeNextCarEnter == CurrentWarmUpTime)
		{
			senario1->CarsInHighwaySenario.insert(0, Car::ProcessEnterCarToHighway(highway));
			senario2->CarsInHighwaySenario.insert(0, Car::ProcessEnterCarToHighway(highway));
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
			delete carPosition;
		}


	}
}


double Simulation::GenerateTimeUntilEnterNextCar()
{
	std::random_device rd;
	std::mt19937 genenator(rd());

	std::normal_distribution<double> distribution(highway->AverageTimeBetweenTwoCarArrivals, 2.50);

	double TimeUntilEnterNextCar = distribution.operator()(genenator);
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
	car->CheckAndApply_ChangeAccelerationByDriver(time);
	Car::CarPosition* carPosition = car->get_Position();
	//No Disorder
	if (senario->disorder.disorderStatus == Senario::Disorder::NoDisorder)
	{
		if (car->get_Speed() < highway->MaxOfSpeedAllowed)
		{
			car->set_Acceleration(car->MaxAcceleration,true);
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
				car->set_Acceleration(car->MaxAcceleration,true);
			}
			//senario 1 or 2
			 else if (abs(car->get_Speed() - highway->MaximumSpeedAllowedInPlacesBeforeDisturbance)< (car->MaxAcceleration * rateOfTimeIncrease))
			{
				car->set_Acceleration(0);
				car->set_Speed(highway->MaximumSpeedAllowedInPlacesBeforeDisturbance);
			}
			//senario 1 In keep to driving suddenly occer Disorder
			else if (car->get_Speed() > highway->MaximumSpeedAllowedInPlacesBeforeDisturbance)
			{
				car->set_Acceleration(-1*car->MaxAcceleration,true);
			}
		}
		//near Disorder
		else if (carPosition->frontOfCar < (senario->disorder.PlaceStartedAffectedByDisorder))
		{
			//senario 1 and 2
			if (car->get_Speed() > highway->SpeedInDisruption)
			{
				car->set_Acceleration(-1 * car->MaxAcceleration,true);
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
				car->set_Acceleration(car->MaxAcceleration,true);
			}
			else if (car->get_Speed() >= highway->MaxOfSpeedAllowed)
			{
				car->set_Acceleration(0);
				car->set_Speed(highway->MaxOfSpeedAllowed);
			}
		}
	}
	

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
				if (senario->disorder.PlaceStartedAffectedByDisorder > senario->disorder.PlaceEndedAffectedByDisorder)
					senario->disorder.PlaceStartedAffectedByDisorder = senario->disorder.PlaceEndedAffectedByDisorder;
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
		bool IsThereCarInDisorder = true;
		for (int carNum = 0; carNum < senario->CarsInHighwaySenario.count(); carNum++)
		{
			if (senario->CarsInHighwaySenario[carNum]->get_Speed() == highway->SpeedInDisruption)
			{
				Car::CarPosition* carPos = senario->CarsInHighwaySenario[carNum]->get_Position();
				senario->disorder.PlaceStartedAffectedByDisorder = carPos->backOfCar - 1;
				if (senario->disorder.PlaceStartedAffectedByDisorder > senario->disorder.PlaceEndedAffectedByDisorder)
					senario->disorder.PlaceStartedAffectedByDisorder = senario->disorder.PlaceEndedAffectedByDisorder;
				IsThereCarInDisorder = true;
				delete carPos;
				break;
			}
		}

		//Determine End Of Disorder
		//find Last Car in Disorder
		senario->disorder.lastCarInDisorder = nullptr;
		for (int carNum = senario->CarsInHighwaySenario.count() - 1; carNum >= 0; carNum--)
		{
			if (senario->CarsInHighwaySenario[carNum]->get_Speed() == highway->SpeedInDisruption)
			{
				senario->disorder.lastCarInDisorder = senario->CarsInHighwaySenario[carNum];
				Car::CarPosition* LastCarPos = senario->CarsInHighwaySenario[carNum]->get_Position();
				senario->disorder.PlaceEndedAffectedByDisorder = LastCarPos->backOfCar - 1;
				delete LastCarPos;
				break;
			}
		}
		if (senario->disorder.lastCarInDisorder == nullptr || IsThereCarInDisorder == false || senario->disorder.PlaceStartedAffectedByDisorder == senario->disorder.PlaceEndedAffectedByDisorder)
		{
			senario->disorder.PlaceEndedAffectedByDisorder = senario->disorder.PlaceStartedAffectedByDisorder;
			senario->disorder.disorderStatus = Senario::Disorder::NoDisorder;
		}
		if (senario->disorder.PlaceEndedAffectedByDisorder - senario->disorder.PlaceStartedAffectedByDisorder > senario->disorder.MaxLengthOfDisorder)
		{
			senario->disorder.MaxLengthOfDisorder=senario->disorder.PlaceEndedAffectedByDisorder - senario->disorder.PlaceStartedAffectedByDisorder;
		}
		
	}
	
	return 0;
}
