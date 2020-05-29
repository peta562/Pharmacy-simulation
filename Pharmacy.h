#pragma once
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <ctime>
#include "Parameters.h"
#include "RNG.h"



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
	static constexpr double inputRate = 122.625; // average input stream intensity
	static constexpr double serveRate = 201.68; // average service time
	static constexpr double serveDisp = 100.57; // standard deviation
	Client *serve1, *serve2; // Clients
	Client** qServe1, ** qServe2; // queues to the pharmacist
	ModelParamaters *mp; // Structure with modeling parameters and statistics output files
	int toArrival; // time to arrival next applications
	int toServe1; // time until completion of service 1
	int toServe2; // time to complete servicing 2
	int fromOut; // time elapsed since the last departure
public:
	Pharmacy(ModelParamaters *m);
	~Pharmacy();
	// Modeling methods
	void Arrival(); // arrival of a new application
	void Complete(int i); // i-th pharmacist completed the service
	void Transition(int i);// transition from the i-th queue
	void Run(); //launch
	// Utility methods
	int Choice(); // select the queue upon receipt
	int Busy(); // number of cashiers employed
	int QLength(int k); // queue length to the k-th cashier
};

