#pragma once

#include <QObject>
#include "Highway.h"
#include "qsharedpointer.h"

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

	class DriverReaction
	{
	public:
		float MaxDriverReactionTime = 2;
		bool IsInDriverReaction;
		float RemainingReactionTime;
		float FutureAcceleration;

		bool StartReactionTime();
		float get_RemainingReactionTime();
	};
	
	float MaxAcceleration = 2.5;
	static Car* ProcessEnterCarToHighway(Highway* highway);


public slots:
	bool set_Speed(float speed);
	float get_Speed();
	bool ChangeSpeed(float amount);

	void set_Acceleration(float acceleration, bool NeedReactTime = false);
	float get_Acceleration();

	bool set_Position(double frontOfCarPosition);
	CarPosition* get_Position();
	bool ChangePosition(double amount);

	double MoveCar(double time);
	double ProcessMoveCar(double time, Highway* highway);

	DriverReaction get_DriverReaction();
	bool CheckAndApply_ChangeAccelerationByDriver(double time);
	bool ApplyAcceleration(double time);


private:      
	float m_Speed;
	float m_Acceleration;
	DriverReaction m_Reaction;
	CarPosition m_CarPosition;



public:
	float Weight;
	float Length=2.600;
	float RotationalMassOfVehicle;
	float FrontalArea;
	float EngineEfficiency;
	float Driving=1;
	float MinimumPowerForKeepTurnOn = 10000;
};
