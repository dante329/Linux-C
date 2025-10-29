#include "Employee.h"
#include <iostream>

using namespace std;

Employee::Employee(int id, string name, int department)
{
	this->m_Id = id;
	this->m_Name = name;
	this->m_DeptId = department;
}

void Employee::showInfo()
{
	cout << "编号：" << m_Id
		<< "\t姓名：" << m_Name
		<< "\t岗位：" << getDepartment()
		<< "\t岗位职责：" << "完成经理交给的任务" << endl;
}

string Employee::getDepartment()
{
	return string("员工");
}