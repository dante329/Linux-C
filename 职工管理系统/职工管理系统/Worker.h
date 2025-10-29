//作为Employee、Manager、Boss的抽象基类
#pragma once
#include<string>

using namespace std;

class Worker
{
public:
	int m_Id;		//编号
	string m_Name;	//姓名
	int m_DeptId;	//岗位编号

	//析构函数可以且应该写成虚函数，但是构造函数不能是虚函数，因为虚函数表在构造期间尚未建立。所以子类的构造函数虽然代码实现相同，但是必须各自实现

	//显示职工信息
	virtual void showInfo() = 0;

	//返回职工岗位名称
	virtual string getDepartment() = 0;
};