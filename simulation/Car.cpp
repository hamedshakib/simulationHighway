#include "Car.h"

Car::Car(float Weight,float FrontalArea,float EngineEfficiency, QObject *parent)
	: QObject(parent)
{

	this->Weight = Weight;
	this->FrontalArea = FrontalArea;
	this->EngineEfficiency = EngineEfficiency;
	this->RotationalMassOfVehicle = Weight;

	m_CarPosition.frontOfCar = 0;
	m_CarPosition.backOfCar = m_CarPosition.frontOfCar - Length;

	m_Reaction.RemainingReactionTime = 0;
	m_Reaction.IsInDriverReaction = false;
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

void Car::set_Acceleration(float acceleration, bool NeedReactTime)
{
	if (NeedReactTime == false)
	{
		m_Acceleration = acceleration;
		return;
	}
	else
	{
		if (m_Reaction.IsInDriverReaction == false)
		{
			m_Reaction.FutureAcceleration = acceleration;
			m_Reaction.StartReactionTime();
		}
		else if (m_Reaction.IsInDriverReaction == true)
		{
			if (m_Reaction.FutureAcceleration != acceleration)
			{
				m_Reaction.FutureAcceleration = acceleration;
				m_Reaction.StartReactionTime();
			}
		}
	}
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

/*
bool Car::CheckAndApplyAcceleration(double time, Highway* highway)
{
	if (m_Acceleration != 0)
	{
		//qDebug() << "did1"<< time;
		if (ChangeSpeed(m_Acceleration * time))
		{
			if (m_Acceleration>0)
				if (m_Speed >= highway->MaximumSpeedAllowedInPlacesBeforeDisturbance)
				{
					m_Acceleration = 0;
					m_Speed = highway->MaximumSpeedAllowedInPlacesBeforeDisturbance;
				}
			else if(m_Acceleration<0)
				if(m_Speed<= highway->MaximumSpeedAllowedInPlacesBeforeDisturbance)
				
			return true;
		}
	}
	else
		return false;
}
*/

double Car::MoveCar(double time)
{
	double movementAmount = time * m_Speed;
	if (ChangePosition(movementAmount))
		return movementAmount;
	else
		return 0;
}

double Car::ProcessMoveCar(double time, Highway* highway)
{
	return MoveCar(time);
	//CheckAndApplyAcceleration(time,highway);
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

/*
bool Car::ProcessAccelerationAfterMove(double time, Highway* highway, Simulation::Disorder::DisorderStatus status)
{
	/*
	if (CheckAndApplyAcceleration(time, highway))
	{
		if (m_Acceleration > 0) 
		{
			if (m_Speed >= highway->MaximumSpeedAllowedInPlacesBeforeDisturbance)
			{
				m_Acceleration = 0;
				m_Speed = highway->MaximumSpeedAllowedInPlacesBeforeDisturbance;
			}
		}
		else if (m_Acceleration < 0) 
		{
			if (status == Simulation::Disorder::DisorderStatus::NoDisorder) 
			{
				if (m_Speed <= highway->MaximumSpeedAllowedInPlacesBeforeDisturbance)
				{
					m_Acceleration = 0;
					m_Speed= highway->MaximumSpeedAllowedInPlacesBeforeDisturbance
				}
			}
			else if (status == Simulation::Disorder::DisorderStatus::sourceOfDisorderFixed)
			{
				if (m_Speed <= )
			}
		}

	}
	
	if(status == Simulation::Disorder::DisorderStatus::NoDisorder)
	{ 
		if (m_Speed < highway->MaximumSpeedAllowedInPlacesBeforeDisturbance)
		{
			m_Acceleration = MaxAcceleration;
			ChangeSpeed(m_Acceleration * time);
		}
	}
	else if (status == Simulation::Disorder::DisorderStatus::CompleteDisorder)
	{

	}
	else if (status == Simulation::Disorder::DisorderStatus::sourceOfDisorderFixed)
	{

	}

}
*/

bool Car::ApplyAcceleration(double time)
{
	ChangeSpeed(time * m_Acceleration);
	return true;
}

bool Car::DriverReaction::StartReactionTime()
{
	//m_RemainingReactionTime = 
	RemainingReactionTime = MaxDriverReactionTime;
	IsInDriverReaction = true;
	return true;
}
float Car::DriverReaction::get_RemainingReactionTime()
{
	return RemainingReactionTime;
}

bool Car::CheckAndApply_ChangeAccelerationByDriver(double time)
{
	if (m_Reaction.IsInDriverReaction == true)
	{
		m_Reaction.RemainingReactionTime -= time;
		if (m_Reaction.RemainingReactionTime <= 0)
		{
			m_Acceleration = m_Reaction.FutureAcceleration;
			m_Reaction.IsInDriverReaction = false;
			m_Reaction.RemainingReactionTime = 0;
			return true;
		}
	}
	return false;
}

Car::DriverReaction Car::get_DriverReaction()
{
	return m_Reaction;
}

