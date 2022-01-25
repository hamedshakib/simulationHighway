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
	};
	*/
	enum DisruptionEvent
	{
		occurDisruption,
		FixDisorder
	};

	bool ResetAll_ForNewRound();
	bool Run();

public slots:
	bool SimulationOfOneRound();

	float NextCarArrivalTime(Highway *highway);
	double GenerateTimeUntilEnterNextCar(Highway* highway);
	void WarmUp();

private:
	unsigned int NumberOfAllRound;
	double AllSimulationTime;
	double AllWarmUpTime;
	unsigned int CurrentRoundNumber;
	double CurrentTime;
	QList<Car*> CarsInHighway;
	DisruptionEvent nextDisruptionEvent;
};
