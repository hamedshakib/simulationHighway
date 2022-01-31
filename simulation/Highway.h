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





	//double CalculateLength_AffectedByDisorderArea();

public:
	bool IsThereDisorder;
	double LenghtOfHighway = 5000;        //   m
	double LocationOfDisorder = 4000;     //   m
	double length_AffectedByDisorderArea; //   m
	float mimimumDistanceBetweenCars=1;   //   m
	float SpeedInDisruption=7;            //   m/s
	float entranceSpeed = 11.2;           //   m/s
	float MaxOfSpeedAllowed = 20;		  //   m/s
	float MaximumSpeedAllowedInPlacesBeforeDisturbance; // m/s
	

	float PercentageOfSlopeDistance=0.1;
	float PercentageOfBendResistanceDistance=0;

	int AmbientTemperature=20;
	float AverageTimeBetweenTwoCarArrivals=9.49;


	Rainfall weatherConditions=Rainfall::NoRain;
	TrafficSituation trafficSituation= TrafficSituation::LowLoad;
	float LongitudinalSlope=10;
	float windEffect=1;
	float AirDensity=1.225;
	float AerodynamicResistance=0.35;
	float RollingResistance=0.01;
	float pavementType=1;
	float MassCorrectionFactorForRotationalInertiaAcceleration=1.05;
	float gravityAcceleration=9.81;

	float PathRadiusFromCentreOfGravity=99999;
	float TotalCorneringStiffness=99999;

};
