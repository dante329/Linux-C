#include "Manager.h"
#include <iostream>

using namespace std;

Manager::Manager(int id, string name, int department)
{
	this->m_Id = id;
	this->m_Name = name;
	this->m_DeptId = department;
}

void Manager::showInfo()
{
	cout << "编号：" << m_Id
		<< "\t姓名：" << m_Name
		<< "\t岗位：" << getDepartment()
		<< "\t岗位职责：" << "完成总裁交给的任务，并下发给员工" << endl;
}

string Manager::getDepartment()
{
	return string("经理");
}