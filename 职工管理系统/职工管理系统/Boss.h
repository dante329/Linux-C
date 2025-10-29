#pragma once
#include "Worker.h"

class Boss : public Worker
{
public:
	Boss(int id, string name, int department);

	virtual void showInfo() override;

	virtual string getDepartment() override;
};