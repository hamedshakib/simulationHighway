#pragma once

#include <QObject>
#include "Highway.h"
#include "Car.h"
#include <math.h>

class FuelConsumption : public QObject
{
	Q_OBJECT

public:
	FuelConsumption(QObject *parent);
	~FuelConsumption();

public:
	static double Calculate(Car* car, Highway* highway);

private:
	static float ConvertRainfallTypeToEffectOnSurface(Highway::Rainfall type);
	static float ConvertRainfallTypeToEffectOnAirDensity(Highway::Rainfall type);
	static float ConvertTemperatureToEffectOnSurface(int temperature);
	static float ConvertSpeedToSpeedOnEffectedByWind(float speed,float Wind=0);
};
