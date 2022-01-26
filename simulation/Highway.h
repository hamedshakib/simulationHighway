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




	double CalculateLength_AffectedByDisorderArea();


public:
	bool IsThereDisorder;
	double LenghtOfHighway = 5000;        //   m
	double LocationOfDisorder = 4000;     //   m
	double length_AffectedByDisorderArea; //   m
	float SpeedInDisruption=7;            //   m/s
	float entranceSpeed = 11.2;           //   m/s
	float MaximumSpeedAllowedInPlacesBeforeDisturbance;

	float PercentageOfSlopeDistance=0.1;
	float PercentageOfBendResistanceDistance=0;

	int AmbientTemperature=20;
	float AverageTimeBetweenTwoCarArrivals=9.49;


	Rainfall weatherConditions=Rainfall::NoRain;
	TrafficSituation trafficSituation= TrafficSituation::LowLoad;
	float LongitudinalSlope;
	float windEffect=1;
	float AirDensity=1.225;
	float AerodynamicResistance;
	float RollingResistance;
	float pavementType;
	float MassCorrectionFactorForRotationalInertiaAcceleration;
	float gravityAcceleration;

	float PathRadiusFromCentreOfGravity;
	float TotalCorneringStiffness;

};
