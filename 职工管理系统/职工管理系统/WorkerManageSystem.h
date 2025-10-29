#pragma once
#include "Worker.h"

class WorkerManageSystem
{
public:
    int m_WNum;
    Worker** m_WArray;
    bool m_FileIsEmpty;

    WorkerManageSystem();

    void showMenu();

    //添加职工
    void addWorker();

    //保存职工信息
    void save();

    //获取文件中职工的个数
    int get_Num();

    //将文件中的数据全部拷贝到堆区（m_WArray）
    void init_Worker();

    //显示所有职工信息
    void showWorkers();

    //判断编号是否存在 存在返回编号，不存在返回-1
    int isExist(int id);

    //删除职工
    void delWorker();

    //修改职工
    void modWorker();

    //查找职工
    void findWorker();

    //按编号排序
    void sortById();

    //清空文件
    void CleanFile();

    //退出系统
    void exitSystem();

    ~WorkerManageSystem();

private:
    // 暂停并清屏
    void pauseAndClear() 
    {
        system("pause");
        system("cls");
    }
};