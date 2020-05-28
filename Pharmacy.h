#pragma once
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include "Parameters.h"

using namespace std;


class Client
{
private:
	int id;
	int time;
public:
	friend class Pharmacy;
	Client(int i);
};

class Pharmacy
{
private:
	static const int buff = 3;
	static constexpr float inputRate = 2; // средняя интенсивность входного потока
	static constexpr float serveRate = 1; // средняя длительность обслуживания
	static constexpr float serveDisp = 0.3; // среднеквадратичное отклонение
	Client *serve1, *serve2; // Клиенты
	Client** qServe1, ** qServe2; // очереди к аптекарю
	ModelParamaters *mp; // Структура с параметрами моделирования и файлами вывода статистики
	int toArrival; // время до прибытия след. заявки
	int toServe1; // время до завершения обслуживания 1
	int toServe2; // время до завершения обсулживания 2
	int fromOut; // время прошедшее с момента последнего ухода
public:
	Pharmacy(ModelParamaters *m);
	~Pharmacy();
	//Моделирующие методы
	void Arrival(); // прибытие новой заявки
	void Complete(int i); //i-й аптекарь завершил обслуживание
	void Transition(int i);//переход из i-й очереди
	void Run(); //запуск
	// Служебные методы
	int Choice(); // выбор очереди при поступлении
	int Busy(); // число занятых кассиров
	int QLength(int k); // длина очереди к k-му кассиру
};

