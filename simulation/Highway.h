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


public:
	double LenghtOfHighway = 5000;
	double LocationOfDisorder = 4000;
	double length_AffectedByDisorderArea;
	float SpeedInDisruption=20;

	float PercentageOfSlopeDistance=0.1;
	float PercentageOfBendResistanceDistance=0;

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
