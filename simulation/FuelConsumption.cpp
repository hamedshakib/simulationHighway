#include "FuelConsumption.h"

FuelConsumption::FuelConsumption(QObject *parent)
	: QObject(parent)
{
}

FuelConsumption::~FuelConsumption()
{
}

float ConvertRainfallTypeToEffectOnSurface(Highway::Rainfall type)
{
	switch (type)
	{
	case Highway::NoRain:
		return 1;
	case Highway::BeginningRain:
		return 0.48;
	case Highway::FullRain:
		return 0.7;
	}
}

float ConvertRainfallTypeToEffectOnAirDensity(Highway::Rainfall type)
{
	switch (type)
	{
	case Highway::NoRain:
		return 1;
	case Highway::BeginningRain:
		return 1.001260;
	case Highway::FullRain:
		return 1.001260;
	}
}

float ConvertTemperatureToEffectOnSurface(int temperature)
{
	// To Do
	/*switch (temperature)
	{
	case 10
	default:
		break;
	}
	*/
}

static float ConvertSpeedToSpeedOnEffectedByWind(float speed, float Wind = 0)
{
	return (speed * (1 + Wind));
}

double FuelConsumption::Calculate(Car* car, Highway* highway)
{
	 (car->Weight*highway->gravityAcceleration*std::sin(highway->LongitudinalSlope *3.141592/180))
	+(highway->MassCorrectionFactorForRotationalInertiaAcceleration* car->RotationalMassOfVehicle* car->Driving* car->Acceleration)
	+(ConvertRainfallTypeToEffectOnSurface(highway->weatherConditions)* highway->RollingResistance* car->Weight * highway->gravityAcceleration * std::cos(highway->LongitudinalSlope * 3.141592 / 180)* ConvertTemperatureToEffectOnSurface(highway->AmbientTemperature)* highway->pavementType)
	+(0.5* ConvertRainfallTypeToEffectOnAirDensity(highway->weatherConditions)* highway->AirDensity*car->FrontalArea* highway->AerodynamicResistance* pow(ConvertSpeedToSpeedOnEffectedByWind(car->get_Speed()),2)*car->Driving)
	+((pow(car->Weight,2)* pow(car->get_Speed(),4)*car->Driving* highway->pavementType)/(pow(highway->PathRadiusFromCentreOfGravity,2)*highway->TotalCorneringStiffness))*0 
}
