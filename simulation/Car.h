#pragma once

#include <QObject>
#include "Highway.h"
//#include "Simulation.h"

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
	float DriverReactionTime=2;
	static Car* ProcessEnterCarToHighway(Highway* highway);


public slots:
	bool set_Speed(float speed);
	float get_Speed();
	bool ChangeSpeed(float amount);

	void set_Acceleration(float acceleration);
	float get_Acceleration();

	bool set_Position(double frontOfCarPosition);
	CarPosition* get_Position();
	bool ChangePosition(double amount);

	double MoveCar(double time);
	double ProcessMoveCar(double time, Highway* highway);

	bool StartReactionTime();
	float get_RemainingReactionTime();
	//bool CheckAndApplyAcceleration(double time, Highway* highway);
	//bool ProcessAccelerationAfterMove(double time, Highway* highway,Simulation::Disorder::DisorderStatus status);
	//bool ProcessCheckAndChangeAcceleration(double time, Highway* highway, Simulation::Disorder::DisorderStatus status);
	bool ApplyAcceleration(double time);


private:      
	float m_Speed;
	float m_Acceleration;
	float m_RemainingReactionTime;
	CarPosition m_CarPosition;
	CarEventType nextCarEventType;



public:
	float Weight;
	float Length=2.600;
	float RotationalMassOfVehicle;
	float FrontalArea;
	float EngineEfficiency;
	float Driving=1;
	float MinimumPowerForKeepTurnOn = 10000;
};
