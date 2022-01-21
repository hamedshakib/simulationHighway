#include "Car.h"

Car::Car(QObject *parent)
	: QObject(parent)
{
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

bool Car::ChangeSpeed(int amount)
{
	if ((m_Speed + amount) >= 0)
	{
		m_Speed += amount;
		return true;
	}
	else
		return false;
}

bool Car::set_Position(double position)
{
	if (position > 0)
	{
		m_Position = position;
		return true;
	}
	else
		return false;

}

double Car::get_Position()
{
	return m_Position;
}

bool Car::ChangePosition(double amount)
{
	if (amount > 0)
	{
		m_Position += amount;
		return true;
	}
	else
		return false;
}

void Car::MoveCar(double time)
{
	double movementAmount = time * m_Speed;
	ChangePosition(movementAmount);
}

