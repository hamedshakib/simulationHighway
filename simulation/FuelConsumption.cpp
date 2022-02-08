#include "FuelConsumption.h"

FuelConsumption::FuelConsumption(QObject *parent)
	: QObject(parent)
{
}

FuelConsumption::~FuelConsumption()
{
}


float FuelConsumption::ConvertRainfallTypeToEffectOnSurface(Highway::Rainfall type)
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

float FuelConsumption::ConvertRainfallTypeToEffectOnAirDensity(Highway::Rainfall type)
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

float FuelConsumption::ConvertTemperatureToEffectOnSurface(int temperature)
{
	float fahrenheitDegree = (temperature*1.6)+32;
	float effect;
	if (fahrenheitDegree >= 22.5 && fahrenheitDegree < 27.5)
		effect = 1.1351;
	else if (fahrenheitDegree >= 27.5 && fahrenheitDegree < 32.5)
		effect = 1.1183;
	else if (fahrenheitDegree >= 32.5 && fahrenheitDegree < 37.5)
		effect = 1.1018;
	else if (fahrenheitDegree >= 37.5 && fahrenheitDegree < 42.5)
		effect = 1.0855;
	else if (fahrenheitDegree >= 42.5 && fahrenheitDegree < 47.5)
		effect = 1.0694;
	else if (fahrenheitDegree >= 47.5 && fahrenheitDegree < 52.5)
		effect = 1.0536;
	else if (fahrenheitDegree >= 52.5 && fahrenheitDegree < 57.5)
		effect = 1.0380;
	else if (fahrenheitDegree >= 57.5 && fahrenheitDegree < 62.5)
		effect = 1.0226;
	else if (fahrenheitDegree >= 62.5 && fahrenheitDegree < 67.5)
		effect = 1.0075;
	else if (fahrenheitDegree >= 67.5 && fahrenheitDegree < 72.5)
		effect = 1.0000;
	else if (fahrenheitDegree >= 72.5 && fahrenheitDegree < 77.5)
		effect = 1.0000;
	else if (fahrenheitDegree >= 77.5 && fahrenheitDegree < 82.5)
		effect = 1.0000;
	else if (fahrenheitDegree >= 82.5 && fahrenheitDegree < 87.5)
		effect = 1.0000;
	else if (fahrenheitDegree >= 87.5 && fahrenheitDegree < 92.5)
		effect = 1.0099;
	else if (fahrenheitDegree >= 92.5 && fahrenheitDegree < 97.5)
		effect = 1.0242;
	else if (fahrenheitDegree >= 97.5 && fahrenheitDegree < 102.5)
		effect = 1.0387;
	else if (fahrenheitDegree >= 102.5 && fahrenheitDegree < 107.5)
		effect = 1.0534;
	else if (fahrenheitDegree >= 107.5 && fahrenheitDegree < 112.5)
		effect = 1.0683;


	return effect;
}

float FuelConsumption::ConvertSpeedToSpeedOnEffectedByWind(float speed, float Wind)
{
	return (speed * (1 + Wind));
}

float FuelConsumption::ConvertTemperatureToEffectOnEngineEfficiency(int temperature)
{
	return 1 - ((float)temperature - 10) * 0.0005;
}

double FuelConsumption::Calculate(double travelledDistance, Car* car, Highway* highway)
{
	
	double FuelConsumed = ((car->Weight * highway->gravityAcceleration * std::sin(highway->LongitudinalSlope * 3.141592 / 180)) * highway->PercentageOfSlopeDistance
		+ (highway->MassCorrectionFactorForRotationalInertiaAcceleration * car->RotationalMassOfVehicle * car->Driving * (car->get_Acceleration() > 0? car->get_Acceleration(): 0))
		+ (ConvertRainfallTypeToEffectOnSurface(highway->weatherConditions) * highway->RollingResistance * car->Weight * highway->gravityAcceleration * std::cos(highway->LongitudinalSlope * 3.141592 / 180) * ConvertTemperatureToEffectOnSurface(highway->AmbientTemperature) * highway->pavementType)
		+ (0.5 * ConvertRainfallTypeToEffectOnAirDensity(highway->weatherConditions) * highway->AirDensity * car->FrontalArea * highway->AerodynamicResistance * pow(ConvertSpeedToSpeedOnEffectedByWind(car->get_Speed()), 2) * car->Driving)
		+ ((pow(car->Weight, 2) * pow(car->get_Speed(), 4) * car->Driving * highway->pavementType) / (pow(highway->PathRadiusFromCentreOfGravity, 2) * highway->TotalCorneringStiffness)) * highway->PercentageOfBendResistanceDistance
		+ (car->MinimumPowerForKeepTurnOn / car->get_Speed()))
		* highway->windEffect *(1/(car->EngineEfficiency * ConvertTemperatureToEffectOnEngineEfficiency(highway->AmbientTemperature)));
		

	return FuelConsumed * travelledDistance;
}


