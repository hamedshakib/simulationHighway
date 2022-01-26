#pragma once

#include <QObject>
#include "Highway.h"

class Car : public QObject
{
	Q_OBJECT

public:
	Car(float Weight=2100,float FrontalArea=2.52,float EngineEfficiency=0.27,QObject *parent=nullptr);
	~Car();

	
	struct CarPosition
	{
		double frontOfCar;
		double backOfCar;
	};

	enum CarEventType
	{
		EnterToHighway,
		InitialReductionSpeed,
		EndOfInitialReductionSpeed,
		ApproachingDisturbedArea,
		ReachingDisturbedArea,
		ExitFromDisturbedArea,
		ExitFromHighway
	};

	float MaxAcceleration = 2.5;
	static Car* ProcessEnterCarToHighway(Highway* highway);


public slots:
	bool set_Speed(float speed);
	float get_Speed();
	bool ChangeSpeed(int amount);

	void set_Acceleration(float acceleration);
	float get_Acceleration();

	bool set_Position(double frontOfCarPosition);
	CarPosition* get_Position();
	bool ChangePosition(double amount);

	void MoveCar(double time);
	void ProcessMoveCar(double time);
	bool CheckAndApplyAcceleration(double time);


private:      
	float m_Speed;
	float m_Acceleration;
	CarPosition m_CarPosition;
	CarEventType nextCarEventType;



public:
	float Weight;
	float Length;
	float RotationalMassOfVehicle;
	float FrontalArea;
	float EngineEfficiency;
	float Driving;
	float MinimumPowerForKeepTurnOn = 10000;
};
