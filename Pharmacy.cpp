#include "Pharmacy.h"

Client::Client(int i)
{
	id = i;
	time = 0;
}

Pharmacy::Pharmacy(ModelParamaters* m)
{
	int i;
	serve1 = nullptr;
	serve2 = nullptr;
	qServe1 = new Client * [buff];
	qServe2 = new Client * [buff];
	mp = m;
	for (auto i = 0; i < buff; i++)
	{
		qServe1[i] = nullptr;
		qServe2[i] = nullptr;
	}
	toArrival = (int)(getExp(inputRate) / mp->K);
	if (toArrival == 0)
	{
		toArrival = 1;
	}
	toServe1 = -1;
	toServe2 = -1;
	fromOut = -1;

}

Pharmacy::~Pharmacy()
{
	if (serve1)
	{
		delete serve1;
	}
	if (serve2)
	{
		delete serve2;
	}
	for (auto i = 0; i < buff; i++)
	{
		if(qServe1[i])
		{
			delete qServe1[i];
		}
		if (qServe2[i])
		{
			delete qServe2[i];
		}
	}
	delete[] qServe1;
	delete[] qServe2;
}

void Pharmacy::Arrival()
{
	int i;
	Client* ptr;
	mp->entered++;
	if (mp->total < mp->K * mp->Dump)
	{
		mp->dump << mp->total << " - поступила новая заявка." << endl;
	}
	toArrival = (int)(getExp(inputRate) / mp->K);
	if (toArrival == 0)
	{
		toArrival = 1;
	}
	i = Choice(); // Choose where to get the client
	if (i == 0) // there are no places, the application is lost
	{
		if (mp->total < mp->K * mp->Dump)
		{
			mp->dump << "Мест нет, завяка отвержена." << endl;
		}
		mp->reject++;
		return;
	}
	// There are places, we generate a request
	ptr = new Client(mp->entered);
	// The memory will be returned in the CompieteO method after the completion of the service
	// at the first or at the second cashier. Or in the destructor, if by the time
	// the completion of the simulation, the application will still be in the system

	if (i == 1)
	{
		if (!serve1) // We get service to the 1st pharmacist
		{
			if (mp->total < mp->K * mp->Dump)
			{
				mp->dump << "заявка получила номер " << mp->entered << " и поступила на обслуживание к первому окну." << endl;
			}
			toServe1 = (int)(getNormal(serveRate, serveDisp) / mp->K);
			if (toServe1 <= 0)
			{
				toServe1 = 1;

			}
			serve1 = ptr;
		}
		else // Application queues
		{
			qServe1[QLength(1)] = ptr;
			if (mp->total < mp->K * mp->Dump)
			{
				mp->dump << "заявка получила номер " << mp->entered << " и поступила в очередь к первому окну. Номер в очереди: " << QLength(1) << endl;
			}
		}
	}

	else  // We get service for the 2nd pharmacy
	{
		if (!serve2)
		{
			if (mp->total < mp->K * mp->Dump)
			{
				mp->dump << "заявка получила номер " << mp->entered << " и поступила на обслуживание к второму окну." << endl;
			}
			toServe2 = (int)(getNormal(serveRate, serveDisp) / mp->K);
			if (toServe2 <= 0)
			{
				toServe2 = 1;
			}
			serve2 = ptr;
		}
		else
		{
			qServe2[QLength(2)] = ptr;
			if (mp->total < mp->K * mp->Dump)
			{
				mp->dump << "заявка получила номер " << mp->entered << " и поступила в очередь к второму окну. Номер в очереди: " << QLength(2) << endl;
			}
		}
	}
}


void Pharmacy::Complete(int i)
{
	int j;
	mp->completed++;
	// If this is not the first case, we make statistics
	if (fromOut != -1)
	{
		mp->time_outbank << fromOut << endl;
		mp->outAve = mp->outAve * (1 - 1.0 / (mp->completed - 1)) + ((double)fromOut) / (mp->completed - 1);
	}
	fromOut = 0; // Reset Counter
	if (i == 1) // Serves the first pharmacist
	{
		// Trace, statistics on average time spent
		if (mp->total < mp->K * mp->Dump)
		{
			mp->dump << mp->total << " - заявка номер " << serve1->id << " покинула систему." << endl;
		}
		mp->sojourn << serve1->time << endl;
		mp->sojAve = mp->sojAve * (1 - 1.0 / mp->completed) + ((double)(serve1->time)) / mp->completed;
		delete serve1;
		if (QLength(1) != 0)// If the queue is not empty, put the next request for service
		{
			serve1 = qServe1[0];
			for (auto j = 0; j < buff - 1; j++)
			{
				qServe1[j] = qServe1[j + 1]; // Queue Shift
			}
			qServe1[buff - 1] = nullptr;
			toServe1 = (int)(getNormal(serveRate, serveDisp) / mp->K);
			if (toServe1 <= 0)
			{
				toServe1 = 1;
			}
		}
		else
		{
			serve1 = nullptr;
			qServe1[0] = nullptr;
			toServe1 = -1;
		}
	}
	else // Serves the second pharmacist
	{
		// Trace, statistics on average time spent
		if (mp->total < mp->K * mp->Dump)
		{
			mp->dump << mp->total << " - заявка номер " << serve2->id << " покинула систему." << endl;
		}
		mp->sojourn << serve2->time << endl;
		mp->sojAve = mp->sojAve * (1 - 1.0 / mp->completed) + ((double)(serve2->time)) / mp->completed;
		delete serve2;
		if (QLength(2) != 0)// If the queue is not empty, put the next request for service
		{
			serve2 = qServe2[0];
			for (auto j = 0; j < buff - 1; j++)
			{
				qServe2[j] = qServe2[j + 1]; // Queue Shift
			}
			qServe2[buff - 1] = nullptr;
			toServe2 = (int)(getNormal(serveRate, serveDisp) / mp->K);
			if (toServe2 <= 0)
			{
				toServe2 = 1;
			}
		}
		else
		{
			serve2 = nullptr;
			qServe2[0] = nullptr;
			toServe2 = -1;
		}
	}

}

void Pharmacy::Transition(int i)
{
	int tmp;
	mp->transition++;
	if (i == 2) // From the second stage to the first
	{
		tmp = qServe2[QLength(2) - 1]->id;
		if (mp->total < mp->K * mp->Dump)
		{
			mp->dump << mp->total << " - переход заявки номер " << tmp << " с полосы " << i << endl;
		}
		qServe1[QLength(1)] = qServe2[QLength(2) - 1];
		qServe2[QLength(2) - 1] = nullptr;
	}
	else // From the first line to the second
	{
		tmp = qServe1[QLength(1) - 1]->id;
		if (mp->total < mp->K * mp->Dump)
		{
			mp->dump << mp->total << " - переход заявки номер " << tmp << " с полосы " << i << endl;
		}
		// Rearrange the order from the tail of the first line to the tail of the second
		qServe2[QLength(2)] = qServe1[QLength(1) - 1];
		qServe1[QLength(1) - 1] = nullptr;
	}
}

void Pharmacy::Run()
{
	int j;
	int realTime;
	if (toServe1 > 0)
	{
		toServe1--;
	}
	if (fromOut != -1)
	{
		fromOut++;
	}
	if (toServe1 == 0)
	{
		Complete(1);
	}
	if (toServe2 > 0)
	{
		toServe2--;
	}
	if (toServe2 == 0)
	{
		Complete(2);
	}
	toArrival--;
	if (toArrival == 0)
	{
		Arrival();
	}
	// Check the transition condition
	if ((QLength(2) - QLength(1)) > 1)
	{
		Transition(2);
	}
	else if ((QLength(1) - QLength(2)) > 1)
	{
		Transition(1);
	}
	// Increment the time counter of all requests in the system
	if (serve1 != nullptr)
	{
		(serve1->time)++;
	}
	if (serve2 != nullptr)
	{
		(serve2->time)++;
	}

	for (int i = 0; i < buff; i++)
	{
		if (qServe1[i] != nullptr) (qServe1[i]->time)++;
		if (qServe2[i] != nullptr) (qServe2[i]->time)++;
	}
	// Each unit of real time recalculation of average values
	if ((mp->total + 1) % mp->K == 0)
	{
		realTime = (mp->total + 1) / mp->K;
		j = Busy() + QLength(1) + QLength(2);
		mp->num_inbank << j << endl;
		mp->numAve = mp->numAve * (1 - 1.0 / realTime) + ((double)j) / realTime;
		mp->que1Ave = mp->que1Ave * (1 - 1.0 / realTime) + ((double)QLength(1)) / realTime;
		mp->que2Ave = mp->que2Ave * (1 - 1.0 / realTime) + ((double)QLength(2)) / realTime;
		mp->roAve = mp->roAve * (1 - 1.0 / realTime) + ((double)Busy()) / realTime;
	}



}

int Pharmacy::Choice()
{
	int k1, k2;
	k1 = QLength(1);
	k2 = QLength(2);
	if (k1 < k2 || serve1 == nullptr) // the first queue is less than the second or there are no queues, the second cashier is busy, the first is not
	{
		return 1;
	}
	else if (k1 > k2 || serve2 == nullptr || k2 != buff) // the first line is larger than the second or there are no requests in the system or the lines are equal and not empty
	{
		return 2;
	}
	else // no waiting places
	{
		return 0; 
	}
}

int Pharmacy::Busy()
{
	if (serve1 == nullptr && serve2 == nullptr)
	{
		return 0; // If Both Are Free
	}
	else if (serve1 == nullptr || serve2 == nullptr) // One of them is free
	{
		return 1;
	}
	else // If both are busy
	{
		return 2;
	}
}

int Pharmacy::QLength(int k)
{
	if (k == 1)
	{
		for (int i = 0; i < buff; i++)
		{
			if (qServe1[i] == nullptr)
			{
				return(i);
			}
		}
		return buff;
	}
	else
	{
		for (int i = 0; i < buff; i++)
		{
			if (qServe2[i] == nullptr)
			{
				return(i);
			}
		}
		return buff;
	}
}
