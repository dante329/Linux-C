#pragma once
#include "Worker.h"

class Manager : public Worker
{
public:
	Manager(int id, string name, int department);

	//显示职工信息
	virtual void showInfo() override;

	//返回职工部门名称
	virtual string getDepartment() override;
};