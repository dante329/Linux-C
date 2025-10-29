//命名规范：类名 大驼峰，函数/方法 小驼峰，成员 前缀m_大驼峰

//头文件注意事项：头文件只包含定义类接口必需的头文件，不包含可能产生副作用的头文件。实现细节需要的头文件放在 .cpp 文件中

#include "WorkerManageSystem.h"
#include <iostream>

using namespace std;

int main() 
{
    WorkerManageSystem wm;

    int choice = 0;

    while (true)
    {
        wm.showMenu();
        cout << "请选择操作(0-7): " << endl;
        cin >> choice;

        switch(choice)
        {
        case 0: //退出系统
            wm.exitSystem();
            break;  
        case 1: //添加职工
            wm.addWorker();
            wm.save();
            break;
        case 2: //删除职工
            wm.delWorker();
            break;
        case 3: //修改职工信息
            wm.modWorker();
            break;
        case 4: //查找职工
            wm.findWorker();
            break;
        case 5: //显示所有职工
            wm.showWorkers();
            break;
        case 6: //按编号排序
            wm.sortById();
            break;
        case 7: //清空所有数据
            wm.CleanFile();
            break;
        default:    
            system("cls");
            break;
        }
    }

    return 0;
}