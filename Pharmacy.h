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
	static constexpr float inputRate = 2; // ������� ������������� �������� ������
	static constexpr float serveRate = 1; // ������� ������������ ������������
	static constexpr float serveDisp = 0.3; // ������������������ ����������
	Client *serve1, *serve2; // �������
	Client** qServe1, ** qServe2; // ������� � ��������
	ModelParamaters *mp; // ��������� � ����������� ������������� � ������� ������ ����������
	int toArrival; // ����� �� �������� ����. ������
	int toServe1; // ����� �� ���������� ������������ 1
	int toServe2; // ����� �� ���������� ������������ 2
	int fromOut; // ����� ��������� � ������� ���������� �����
public:
	Pharmacy(ModelParamaters *m);
	~Pharmacy();
	//������������ ������
	void Arrival(); // �������� ����� ������
	void Complete(int i); //i-� �������� �������� ������������
	void Transition(int i);//������� �� i-� �������
	void Run(); //������
	// ��������� ������
	int Choice(); // ����� ������� ��� �����������
	int Busy(); // ����� ������� ��������
	int QLength(int k); // ����� ������� � k-�� �������
};

