#include "Highway.h"

Highway::Highway(QObject *parent)
	: QObject(parent)
{
	NumberOfCarEnterInHour = 10;
	length_AffectedByDisorderArea=0;

}

Highway::~Highway()
{
}


