#include "Car.h"

Car::Car(float Weight,float FrontalArea,float EngineEfficiency, QObject *parent)
	: QObject(parent)
{

	Length=2.600;
	m_CarPosition.frontOfCar = 0;
	m_CarPosition.backOfCar = m_CarPosition.frontOfCar - Length;
}

Car::~Car()
{
}

bool Car::set_Speed(float speed)
{
	if (speed > 0)
	{
		m_Speed = speed;
		return true;
	}
	else
		return false;
}

float Car::get_Speed()
{
	return m_Speed;
}

bool Car::set_Position(double position)
{
	if (position > 0)
	{
		m_CarPosition.frontOfCar = position;
		m_CarPosition.backOfCar = position - Length;
		return true;
	}
	else
		return false;

}

Car::CarPosition* Car::get_Position()
{
	CarPosition* carpos = new CarPosition();
	carpos->backOfCar  = m_CarPosition.backOfCar;
	carpos->frontOfCar = m_CarPosition.frontOfCar;
	return carpos;
}

bool Car::ChangePosition(double amount)
{
	if (amount > 0)
	{
		m_CarPosition.backOfCar  += amount;
		m_CarPosition.frontOfCar += amount;
		return true;
	}
	else
		return false;
}

void Car::set_Acceleration(float acceleration)
{
	m_Acceleration = acceleration;
}

float Car::get_Acceleration()
{
	return m_Acceleration;
}

bool Car::ChangeSpeed(float amount)
{
	if ((m_Speed + amount) >= 0)
	{
		//qDebug() << "did2"<< amount;
		m_Speed += amount;
		return true;
	}
	else
		return false;
}

bool Car::CheckAndApplyAcceleration(double time, Highway* highway)
{
	if (m_Acceleration != 0)
	{
		//qDebug() << "did1"<< time;
		if (ChangeSpeed(m_Acceleration * time))
		{
			if (m_Speed >= highway->MaximumSpeedAllowedInPlacesBeforeDisturbance)
			{
				m_Acceleration = 0;
				m_Speed = highway->MaximumSpeedAllowedInPlacesBeforeDisturbance;
			}
			return true;
		}
	}
	else
		return false;
}

void Car::MoveCar(double time)
{
	double movementAmount = time * m_Speed;
	ChangePosition(movementAmount);
}

void Car::ProcessMoveCar(double time, Highway* highway)
{
	MoveCar(time);
	CheckAndApplyAcceleration(time,highway);
}


Car* Car::ProcessEnterCarToHighway(Highway* highway)
{
	Car* car = new Car();
	
	float CurrentCarSpeed = highway->entranceSpeed;
	car->set_Speed(CurrentCarSpeed);
	if (highway->MaximumSpeedAllowedInPlacesBeforeDisturbance > CurrentCarSpeed)
	{
		car->set_Acceleration(car->MaxAcceleration);
		//qDebug() <<"shh:" << car->MaxAcceleration;
	}
	
	return car;
}

