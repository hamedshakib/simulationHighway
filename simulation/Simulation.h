#pragma once

#include <QObject>
#include <qdebug.h>

class Simulation : public QObject
{
	Q_OBJECT

public:
	Simulation(unsigned int numberOfAllRound,QObject *parent=nullptr);
	~Simulation();

	bool ResetAll_ForNewRound();
	bool Run();

public slots:
	bool SimulationOfOneRound();

private:
	unsigned int NumberOfAllRound;
	unsigned int CurrentRoundNumber;
	double CurrentTime;
};
