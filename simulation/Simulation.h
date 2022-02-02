#pragma once

#include <QObject>
#include <qdebug.h>
#include <qlist.h>
#include "Car.h"
#include "Highway.h"
#include "FuelConsumption.h"

class Simulation : public QObject
{
	Q_OBJECT

public:
	Simulation(unsigned int numberOfAllRound, double MaxSimulationTime, double WarmUpTime,QObject *parent=nullptr);
	~Simulation();



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
			double MaxLengthOfDisorder;
			double PlaceStartedAffectedByDisorder;
			double PlaceEndedAffectedByDisorder;
			Car* lastCarInDisorder;
		};
		Disorder disorder;
		double TimeOfStartOfSourceOfDisorder = 0;
		double TimeOfEndOfSourceOfDisorder = 900;
		QList<Car*> CarsInHighwaySenario;
		float MaximumSpeedAllowedInPlacesBeforeDisturbance;
		double MaximumDistanceToSlowDownForDisturbance = 192;
		float TotalFuelConsumption;
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
	double MaxMainSimulationTime;
	double AllWarmUpTime;
	unsigned int CurrentRoundNumber;
	float CurrentTime;

	QList<double> ListOfTimeOfEntersCars;

	float TimeNextCarEnter;

	Senario *senario1,*senario2;

};
