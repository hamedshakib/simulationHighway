#include "FuelConsumption.h"

FuelConsumption::FuelConsumption(QObject *parent)
	: QObject(parent)
{
}

FuelConsumption::~FuelConsumption()
{
}

static float ConvertRainfallTypeToEffectOnSurface(Highway::Rainfall type)
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

static float ConvertRainfallTypeToEffectOnAirDensity(Highway::Rainfall type)
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

static float ConvertTemperatureToEffectOnSurface(int temperature)
{
	// To Do
	/*switch (temperature)
	{
	case 10
	default:
		break;
	}
	*/

	return 0;
}

static float ConvertSpeedToSpeedOnEffectedByWind(float speed, float Wind = 0)
{
	return (speed * (1 + Wind));
}

static float ConvertTemperatureToEffectOnEngineEfficiency(int temperature)
{
	return 0;
}

static double Calculate(double travelledDistance, Car* car, Highway* highway)
{
	
	double FuelConsumed = ((car->Weight * highway->gravityAcceleration * std::sin(highway->LongitudinalSlope * 3.141592 / 180)) * highway->PercentageOfSlopeDistance
		+ (highway->MassCorrectionFactorForRotationalInertiaAcceleration * car->RotationalMassOfVehicle * car->Driving * car->get_Acceleration())
		+ (ConvertRainfallTypeToEffectOnSurface(highway->weatherConditions) * highway->RollingResistance * car->Weight * highway->gravityAcceleration * std::cos(highway->LongitudinalSlope * 3.141592 / 180) * ConvertTemperatureToEffectOnSurface(highway->AmbientTemperature) * highway->pavementType)
		+ (0.5 * ConvertRainfallTypeToEffectOnAirDensity(highway->weatherConditions) * highway->AirDensity * car->FrontalArea * highway->AerodynamicResistance * pow(ConvertSpeedToSpeedOnEffectedByWind(car->get_Speed()), 2) * car->Driving)
		+ ((pow(car->Weight, 2) * pow(car->get_Speed(), 4) * car->Driving * highway->pavementType) / (pow(highway->PathRadiusFromCentreOfGravity, 2) * highway->TotalCorneringStiffness)) * highway->PercentageOfBendResistanceDistance
		+ (car->MinimumPowerForKeepTurnOn / car->get_Speed()))
		* highway->windEffect *(1/(car->EngineEfficiency * ConvertTemperatureToEffectOnEngineEfficiency(highway->AmbientTemperature)));
		

	
	return FuelConsumed * travelledDistance;
}
