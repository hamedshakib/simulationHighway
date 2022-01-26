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
	

	enum DisruptionEvent
	{
		occurDisruption,
		FixDisorder
	};

	

	/*struct Events
	{
		double time;
		
	};*/


	bool ResetAll_ForNewRound();
	bool Run();

public slots:
	bool SimulationOfOneRound();

	void GenerateArrivalTimeOfAllCar(int count);
	float NextCarArrivalTime();
	double GenerateTimeUntilEnterNextCar();
	void WarmUp();

	int CheckAndApplyEvents();

private:
	Highway* highway;
	float rateOfTimeIncrease=0.001;
	unsigned int NumberOfAllRound;
	double AllSimulationTime;
	double AllWarmUpTime;
	unsigned int CurrentRoundNumber;
	double CurrentTime;
	QList<Car*> CarsInHighwaySenario1;
	QList<Car*> CarsInHighwaySenario2;
	QList<Car*> ExitedFromHighway;

	QList<double> ListOfTimeOfEntersCars;

	double TimeNextCarEnter;
	double TimeEndDisruption;
	DisruptionEvent HighwayCondition;
};
