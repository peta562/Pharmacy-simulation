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
	toArrival = (int)((rand() % 2 + 1) * mp->K);
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
	toArrival = (int)((rand() % 2 + 1) * mp->K);
	if (toArrival == 0)
	{
		toArrival = 1;
	}
	i = Choice(); // Выбираем куда встать клиенту
	if (i == 0) // мест нет, заявка теряется
	{
		if (mp->total < mp->K * mp->Dump)
		{
			mp->dump << "Мест нет, завяка отвержена." << endl;
		}
		mp->reject++;
		return;
	}
	//Места есть, генерируем заявку
	ptr = new Client(mp->entered);
	//Память будет возвращена в методе CompieteO после завершения обслуживания 
	//у первого либо у второго кассира. Или в деструкторе, если к моменту 
	//завершения моделирования заявка еще будет находиться в системе 
	if (i == 1)
	{
		if (!serve1) //Поступаем на обслуживание к 1-му аптекарю
		{
			if (mp->total < mp->K * mp->Dump)
			{
				mp->dump << "заявка получила номер " << mp->entered << " и поступила на обслуживание к первому окну." << endl;
			}
			toServe1 = (int)((rand() % 3 + 1) * mp->K);
			if (toServe1 <= 0)
			{
				toServe1 = 1;

			}
			serve1 = ptr;
		}
		else //Заявка встает в очередь
		{
			qServe1[QLength(1)] = ptr;
			if (mp->total < mp->K * mp->Dump)
			{
				mp->dump << "заявка получила номер " << mp->entered << " и поступила в очередь к первому окну. Номер в очереди: " << QLength(1) << endl;
			}
		}
	}

	else  // Поступаем на обслуживание к 2-му аптекаю
	{
		if (!serve2)
		{
			if (mp->total < mp->K * mp->Dump)
			{
				mp->dump << "заявка получила номер " << mp->entered << " и поступила на обслуживание к второму окну." << endl;
			}
			toServe2 = (int)((rand() % 3 + 1) * mp->K);
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
	// Если это не первых уход, делаем статистические записи
	if (fromOut != -1)
	{
		mp->time_outbank << fromOut << endl;
		mp->outAve = mp->outAve * (1 - 1.0 / (mp->completed - 1)) + ((float)fromOut) / (mp->completed - 1);
	}
	fromOut = 0; // Сброс счетчика
	if (i == 1) // Обслуживает первый аптекарь
	{
		// Трассировка, статистика по среднему времени пребывания
		if (mp->total < mp->K * mp->Dump)
		{
			mp->dump << mp->total << " - заявка номер " << serve1->id << " покинула систему." << endl;
		}
		mp->sojourn << serve1->time << endl;
		mp->sojAve = mp->sojAve * (1 - 1.0 / mp->completed) + ((float)(serve1->time)) / mp->completed;
		delete serve1;
		if (QLength(1) != 0)// Если очередь не пуста, ставим на обслуживание очередную заявку
		{
			serve1 = qServe1[0];
			for (auto j = 0; j < buff - 1; j++)
			{
				qServe1[j] = qServe1[j + 1]; // Сдвиг очереди
			}
			qServe1[buff - 1] = nullptr;
			toServe1 = (int)((rand() % 3 + 1) * mp->K);
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
	else // Обслуживает второй аптекарь
	{
		// Трассировка, статистика по среднему времени пребывания
		if (mp->total < mp->K * mp->Dump)
		{
			mp->dump << mp->total << " - заявка номер " << serve2->id << " покинула систему." << endl;
		}
		mp->sojourn << serve2->time << endl;
		mp->sojAve = mp->sojAve * (1 - 1.0 / mp->completed) + ((float)(serve2->time)) / mp->completed;
		delete serve2;
		if (QLength(2) != 0)// Если очередь не пуста, ставим на обслуживание очередную заявку
		{
			serve2 = qServe2[0];
			for (auto j = 0; j < buff - 1; j++)
			{
				qServe2[j] = qServe2[j + 1]; // Сдвиг очереди
			}
			qServe2[buff - 1] = nullptr;
			toServe2 = (int)((rand() % 3 + 1) * mp->K);
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
	if (i == 1) // Из первой очереди во вторую
	{
		tmp = qServe1[QLength(1) - 1]->id;
		if (mp->total < mp->K * mp->Dump)
		{
			mp->dump << mp->total << " - переход заявки номер " << tmp << "с полосы " << i << endl;
		}
		//Переставляем заявку из хвоста первой очереди в хвост второй
		qServe2[QLength(2)] = qServe1[QLength(1) - 1];
		qServe1[QLength(1) - 1] = nullptr;
	}
	else //Из второй очереди в первую
	{
		tmp = qServe2[QLength(2) - 1]->id;
		if (mp->total < mp->K * mp->Dump)
		{
			mp->dump << mp->total << " - переход заявки номер " << tmp << "с полосы " << i << endl;
		}
		qServe1[QLength(1)] = qServe2[QLength(2) - 1];
		qServe2[QLength(2) - 1] = nullptr;
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
		fromOut--;
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
	//Проверка условия перехода
	if ((QLength(2) - QLength(1)) > 1)
	{
		Transition(1);
	}
	else if ((QLength(1) - QLength(2)) > 1)
	{
		Transition(1);
	}
	//Инкремент счетчика времени всех заявок в системе
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
	//Каждую единицу реального времени пересчет средних значений
	if ((mp->total + 1) % mp->K == 0)
	{
		realTime = (mp->total + 1) / mp->K;
		j = Busy() + QLength(1) + QLength(2);
		mp->num_inbank << j << endl;
		mp->numAve = mp->numAve * (1 - 1.0 / realTime) + ((float)j) / realTime;
		mp->que1Ave = mp->que1Ave * (1 - 1.0 / realTime) + ((float)QLength(1)) / realTime;
		mp->que2Ave = mp->que2Ave * (1 - 1.0 / realTime) + ((float)QLength(2)) / realTime;
		mp->roAve = mp->roAve * (1 - 1.0 / realTime) + ((float)Busy()) / realTime;
	}



}

int Pharmacy::Choice()
{
	int k1, k2;
	k1 = QLength(1);
	k2 = QLength(2);
	if (k1 > k2 || serve2 == nullptr) // первая очередь больше второй или очередей нет, первый кассир занят, второй нет
	{
		return 2;
	}
	else if (k1 < k2 || serve1 == nullptr || k1 != buff) // вторая очередь больше первой или в системе заявок нет или очереди равны и не пусты
	{
		return 1;
	}
	else // мест для ожидания нет
	{
		return 0; 
	}
}

// Кто из аптекарей свободен
int Pharmacy::Busy()
{
	if (serve1 == nullptr && serve2 == nullptr)
	{
		return 0; // Если оба свободны
	}
	else if (serve1 == nullptr || serve2 == nullptr) // Один из них свободен
	{
		return 1;
	}
	else // Если оба заняты
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
