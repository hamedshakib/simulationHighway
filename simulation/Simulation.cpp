#include "Simulation.h"

Simulation::Simulation(unsigned int numberOfAllRound,QObject *parent)
	: QObject(parent)
{
	NumberOfAllRound = numberOfAllRound;
	CurrentRoundNumber = 1;
}

Simulation::~Simulation()
{
}

bool Simulation::ResetAll_ForNewRound()
{
	CurrentTime = 0;
	return true;
}

bool Simulation::Run()
{
	for (    ; CurrentRoundNumber <= NumberOfAllRound; CurrentRoundNumber++)
	{
		ResetAll_ForNewRound();
		SimulationOfOneRound();
		qDebug() << "END Round" << CurrentRoundNumber;
	}
	return true;
}

bool Simulation::SimulationOfOneRound()
{
	for (; CurrentTime <= 500; CurrentTime = CurrentTime + 0.001)
	{
		qDebug() << CurrentTime;
	}
	return true;
}
