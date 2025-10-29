#pragma once
#include "Worker.h"

class Employee : public Worker
{
public: 
	Employee(int id, string name, int department);

	virtual void showInfo() override;

	virtual string getDepartment() override;
};