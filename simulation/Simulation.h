#pragma once

#include <QObject>
#include <qdebug.h>
#include <qlist.h>
#include "Car.h"
#include "Highway.h"

class Simulation : public QObject
{
	Q_OBJECT

public:
	Simulation(unsigned int numberOfAllRound, double SimulationTime, double WarmUpTime,QObject *parent=nullptr);
	~Simulation();

	/*
	class Event
	{
		enum EventType
		{
			EnterToHighway,
			InitialReductionSpeed,
			EndOfInitialReductionSpeed,
			ApproachingDisturbedArea,
			ReachingDisturbedArea,
			ExitFromDisturbedArea,
			ExitFromHighway
		};
		double time;
		EventType eventType;
		Car* relatedCar;
	};
	*/

	/*
	struct Disorder
	{
		enum DisorderStatus
		{
			CompleteDisorder,
			sourceOfDisorderFixed,
			NoDisorder
		};

		DisorderStatus disorderStatus;
		double StartingPlaceAffectedByDisorder;
		double EndingPlaceAffectedByDisorder;
	};
	*/

	class Senario
	{
	public:

		struct Disorder
		{
			enum DisorderStatus
			{
				CompleteDisorder,
				SourceOfDisorderFixed,
				NoDisorder
			};

			DisorderStatus disorderStatus;
			double PlaceStartedAffectedByDisorder;
			double PlaceEndedAffectedByDisorder;
		};
		Disorder disorder;
		double TimeOfStartOfSourceOfDisorder = 0;
		double TimeOfEndOfSourceOfDisorder = 600;
		QList<Car*> CarsInHighwaySenario;
		float MaximumSpeedAllowedInPlacesBeforeDisturbance = 20;
		double LocationRangeNearToDisorder = 192;

	};


	bool ResetAll_ForNewRound();
	bool Run();

public slots:
	bool SimulationOfOneRound();

	void GenerateArrivalTimeOfAllCar(int count);
	float NextCarArrivalTime();
	double GenerateTimeUntilEnterNextCar();
	void WarmUp();

	int CheckAndApplyEvents();
	bool ProcessAcceleration(double time,Car* car,Highway *highway,Senario* senario);
	bool ProcessDeterminetePlaceAffectedByDisorder(Senario* senario);

private:
	Highway* highway;
	float rateOfTimeIncrease=0.001;
	unsigned int NumberOfAllRound;
	double AllSimulationTime;
	double AllWarmUpTime;
	unsigned int CurrentRoundNumber;
	float CurrentTime;
	//QList<Car*> CarsInHighwaySenario1;
	//QList<Car*> CarsInHighwaySenario2;
	//QList<Car*> ExitedFromHighway;

	QList<double> ListOfTimeOfEntersCars;

	float TimeNextCarEnter;


	//double TimeOfStartOfSourceOfDisorder = 0;
	//double TimeOfEndOfSourceOfDisorder = 600;

	//Disorder Senario1_Disorder;
	//Disorder Senario2_Disorder;

	Senario *senario1,*senario2;

};
