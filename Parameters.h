#pragma once
#include <iostream>
#include <fstream>

using namespace std;

struct ModelParamaters
{
	int K = 100; // Time Mapping coefficient
	int Dump = 10;
	ofstream dump; // file to trace
	ofstream num_inbank; // file for collecting statistics on the number of applications in the system
	ofstream time_outbank; // file for collecting statistics of the output stream
	ofstream sojourn; // file for collecting statistics on the time spent in the system


	int entered = 0; // receipt counter
	int completed = 0; // counter of served orders
	int transition = 0; // jump counter
	int reject = 0; // counter of lost orders
	float que1Ave = 0; // variable to recalculate the average length of the queue to the first window
	float que2Ave = 0; // variable to recalculate the average length of the queue to the second window
	float numAve = 0; // variable to recalculate the average number of applications in the system
	float sojAve = 0; // variable to recalculate the average time spent by applications in the system
	float outAve = 0; // variable for recalculating the average duration of the interval between departures
	float roAve = 0; // variable for recalculating the system load factor
	long int total; // model time counter

};
