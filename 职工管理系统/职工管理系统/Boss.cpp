#include "Boss.h"
#include <iostream>

using namespace std;

Boss::Boss(int id, string name, int department)
{
	this->m_Id = id;
	this->m_Name = name;
	this->m_DeptId = department;
}

void Boss::showInfo()
{
	cout << "编号：" << m_Id
		<< "\t姓名：" << m_Name
		<< "\t岗位：" << getDepartment()
		<< "\t岗位职责：" << "管理公司所有事物" << endl;
}

string Boss::getDepartment()
{
	return string("总裁");
}