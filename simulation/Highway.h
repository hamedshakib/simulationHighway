#pragma once

#include <QObject>
#include <random>
#include <iostream>
#include <qdebug.h>

class Highway : public QObject
{
	Q_OBJECT

public:
	Highway(QObject *parent=nullptr);
	~Highway();

	enum  Rainfall
	{
		NoRain,
		BeginningRain,
		FullRain
	};

	enum TrafficSituation
	{
		LowLoad,
		MediumLoad,
		HighLoad
	};

	double GenerateTimeUntilEnterNextCar();


public:
	unsigned int NumberOfCarEnterInHour;
	Rainfall weatherConditions;
	int AmbientTemperature;
	float LongitudinalSlope;
	TrafficSituation trafficSituation;
	float windEffect;
	float AirDensity;
	float AerodynamicResistance;
	float RollingResistance;
	float pavementType;
	float MassCorrectionFactorForRotationalInertiaAcceleration;
	float gravityAcceleration;

	float PathRadiusFromCentreOfGravity;
	float TotalCorneringStiffness;
};
