#pragma once

#include <QObject>

class Car : public QObject
{
	Q_OBJECT

public:
	Car(float Weight=2100,float FrontalArea=2.52,float EngineEfficiency=0.27,QObject *parent=nullptr);
	~Car();

public slots:
	bool set_Speed(float speed);
	float get_Speed();
	bool ChangeSpeed(int amount);

	bool set_Position(double position);
	double get_Position();
	bool ChangePosition(double amount);

	void MoveCar(double time);


private:
	float m_Speed;
	double m_Position;

public:
	float Weight;
	float RotationalMassOfVehicle;
	float FrontalArea;
	float EngineEfficiency;
	float Driving;
	float Acceleration;
};
