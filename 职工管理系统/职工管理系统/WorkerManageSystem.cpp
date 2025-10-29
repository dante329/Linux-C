#include "WorkerManageSystem.h"
#include <iostream>
#include "Employee.h"
#include "Manager.h"
#include "Boss.h"
#include <fstream>

#define FILENAME "WorkerFile.txt"

using namespace std;

//添加返回功能

WorkerManageSystem::WorkerManageSystem()
{
    ifstream ifs(FILENAME, ios::in);

    //文件不存在
    if (!ifs.is_open())
    {
        //cout << "文件不存在" << endl;
        this->m_WNum = 0;
        this->m_WArray = NULL;
        this->m_FileIsEmpty = true;
        ifs.close();
        return;
    }

    //文件为空
    char ch;
    ifs >> ch; // 读取空白字符

    if (ifs.eof())
    {
        //cout << "文件为空" << endl;
        this->m_WNum = 0;
        this->m_WArray = NULL;
        this->m_FileIsEmpty = true;
        ifs.close();
        return;
    }

    //文件不为空 
    int num = get_Num();
    this->m_WNum = num;
    //cout << "职工个数为" << num << endl;

    this->m_WArray = new Worker * [this->m_WNum]; //这里要为m_WArray先开辟空间，然后在init_Worker()中才能将文件中的数据拷贝到m_WArray
    init_Worker();

    ifs.close();
}

//bug:同一次批量输入的时候出现重复编号不报错
void WorkerManageSystem::addWorker()
{
    cout << "请输入添加职工数量：" << endl;

    int AddNum = 0;
    cin >> AddNum;

    int NewSize = this->m_WNum + AddNum;

    if (AddNum > 0)
    {
        Worker** NewArray = new Worker * [NewSize];

        //移动旧数据到NewArray
        if (this->m_WArray != NULL)
        {
            for (int i = 0; i < this->m_WNum; i++)
            {
                NewArray[i] = this->m_WArray[i];
            }
        }

        //批量添加新数据
        for (int i = 0; i < AddNum; i++)
        {
            int id = 0;
            string name;
            int did = 0;

            cout << "请输入第 " << i + 1 << " 个职工的编号：" << endl;
            cin >> id;

            // 检查编号是否重复（包括旧数据和本次已添加的新数据）
            while (true)
            {
                bool isDuplicate = false;

                // 检查与旧数据重复
                if (isExist(id) != -1)
                {
                    isDuplicate = true;
                }
                // 检查与本次已添加的新数据重复（当前循环已添加的i个新职工）
                else
                {
                    for (int j = 0; j < i; j++)
                    {
                        if (NewArray[m_WNum + j]->m_Id == id)
                        {
                            isDuplicate = true;
                            break;
                        }
                    }
                }

                if (isDuplicate)
                {
                    cout << "编号 " << id << " 已存在，请重新输入：" << endl;
                    cin >> id;
                }
                else
                {
                    break; // 编号有效，退出循环
                }
            }

            cout << "请输入第 " << i + 1 << " 个职工的姓名：" << endl;
            cin >> name;

            cout << "请选择第 " << i + 1 << " 个职工的岗位：" << endl;
            cout << "1.普通员工" << endl;
            cout << "2.经理" << endl;
            cout << "3.老板" << endl;

            // 检查岗位输入是否有效
            while (true)
            {
                cin >> did;
                if (did >= 1 && did <= 3)
                {
                    break;
                }
                cout << "岗位输入错误，请重新输入(1-3)：" << endl;
            }

            Worker *worker = NULL;

            switch (did)
            {
            case 1:
                worker = new Employee(id, name, did);
                break;
            case 2:
                worker = new Manager(id, name, did);
                break;
            case 3:
                worker = new Boss(id, name, did);
                break;
            default:
                break;
            }

            NewArray[m_WNum + i] = worker;
            cout << "成功添加职工：" << name << "（编号：" << id << "）" << endl;
        }

        //释放旧的指针数组，并指向新的指针数组，更新职工人数
        delete[] m_WArray;

        m_WArray = NewArray;

        m_WNum = NewSize;

        //提示添加成功
        cout << "成功添加" << AddNum << "名新员工信息!" << endl;

        m_FileIsEmpty = false;
    }
    else
    {
        cout << "数据输入有误，请重新输入" << endl;
    }
    pauseAndClear();    
}

void WorkerManageSystem::save()
{
    ofstream ofs(FILENAME, ios::out);

    for (int i = 0; i < m_WNum; i++)
    {
        ofs << m_WArray[i]->m_Id << " "
            << m_WArray[i]->m_Name << " "
            << m_WArray[i]->m_DeptId << endl;
    }

    ofs.close();
}

int WorkerManageSystem::get_Num()
{
    ifstream ifs(FILENAME, ios::in);
    
    if (!ifs.is_open())
    {
        perror("get_Num():ifs.is_open()");
        return -1;
    }

    int id;
    string name;
    int did;
    int num = 0;

    while (ifs >> id && ifs >> name && ifs >> did)
    {
        num++;
    }
    ifs.close();

    return num;
}

void WorkerManageSystem::init_Worker()
{
    ifstream ifs(FILENAME, ios::in);

    if (!ifs.is_open())
    {
        return;
    }

    int id;
    string name;
    int did;

    int index = 0;
    while (ifs >> id && ifs >> name && ifs >> did)
    {
        Worker* worker = NULL;
        switch (did)
        {
        case 1:
            worker = new Employee(id, name, did);
            break;
        case 2:
            worker = new Manager(id, name, did);
            break;
        case 3:
            worker = new Boss(id, name, did);
            break;
        default:
            break;
        }
        m_WArray[index++] = worker;
    }
}

void WorkerManageSystem::showWorkers()
{
    if (m_FileIsEmpty)
    {
        cout << "文件不存在或数据为空!" << endl;
    }
    Worker* worker = NULL;
    for (int i = 0; i < m_WNum; i++)
    {
        worker = m_WArray[i];
        worker->showInfo();
    }
    pauseAndClear();
}

int WorkerManageSystem::isExist(int id)
{
    for (int i = 0; i < m_WNum; i++)
    {
        if (m_WArray[i]->m_Id == id)
        {
            return i;
        }
    }

    return -1;
}

void WorkerManageSystem::delWorker()
{
    if (m_FileIsEmpty)
    {
        cout << "文件为空，无法删除" << endl;
        pauseAndClear();
        return;
    }

    cout << "输入要删除的职工的编号：" << endl;
    int id;
    cin >> id;

    int index = isExist(id);
    if (index == -1)
    {
        cout << "职工编号不存在，删除失败！" << endl;
        pauseAndClear();
        return;
    }

    // 先删除目标位置的Worker对象
    delete m_WArray[index];      // 释放动态内存，m_WArray[index]是一个对象，这个对象是new出来的，需要delete
    m_WArray[index] = NULL;

    // m_WArray中[index,m_WNum-1]的数据全部前移一位
    for (int i = index; i < m_WNum; i++)
    {
        m_WArray[i] = m_WArray[i + 1];
    }

    // 数组空间是在析构函数中整体释放的，不需要在这里释放数组中最后一个元素的空间
    // delete[] m_WArray; 会释放整个指针数组
    m_WArray[m_WNum - 1] = NULL;

    m_WNum--;

    save(); //数据同步到文件中

    // 更新空文件状态
    if (m_WNum == 0)
    {
        m_FileIsEmpty = true;
    }

    cout << "删除成功！" << endl;

    pauseAndClear();
}

void WorkerManageSystem::modWorker()
{
    if (m_FileIsEmpty)
    {
        cout << "文件为空!" << endl;
        pauseAndClear();
        return;
    }

    cout << "请输入要修改的职工编号：" << endl;
    
    int id;
    cin >> id;

    int index = isExist(id);
    if (index == -1)
    {
        cout << "职工编号不存在!" << endl;
        pauseAndClear();
        return;
    }

    //把需要修改的对象删除，然后重新创建修改后的对象
    delete m_WArray[index];
    m_WArray[index] = NULL;
    
    int Id;
    string Name;
    int dId;

    cout << "查到" << id << "号职工信息，" << "请输入新的职工编号：" << endl;
    cin >> Id;
    
    cout << "请输入新的职工姓名：" << endl;
    cin >> Name;
    
    cout << "请输入新的职工岗位编号：" << endl;
    cout << "1.普通员工" << endl;
    cout << "2.经理" << endl;
    cout << "3.老板" << endl;
    cin >> dId;

    Worker* worker = NULL;
    switch (dId)
    {
    case 1:
        worker = new Employee(Id, Name, dId);
        break;
    case 2:
        worker = new Manager(Id, Name, dId);
        break;
    case 3:
        worker = new Boss(Id, Name, dId);
        break;
    default:
        break;
    }

    m_WArray[index] = worker;

    save();

    cout << "修改成功!" << endl;

    pauseAndClear();
}

void WorkerManageSystem::findWorker()
{
    if (m_FileIsEmpty)
    {
        cout << "文件为空!" << endl;
        pauseAndClear();
        return;
    }

    cout << "1.按编号查找" << endl 
         << "2.按姓名查找" << endl;

    cout << "请输入查找方式：" << endl;
    int choice;
    cin >> choice;

    if (choice == 1)
    {
        cout << "请输入要查找的编号：" << endl;
        int id; 
        cin >> id;
        int index = isExist(id);
        if (index == -1)
        {
            cout << "该编号不存在!" << endl;
            pauseAndClear();
            return;
        }
        else
        {
            cout << "查找到编号为：" << id << "的职工!" << endl;
            m_WArray[id]->showInfo();
        }
    }
    else
    {
        cout << "请输入要查找的姓名：" << endl;

        string name; 
        cin >> name;

        int flag = false;

        for (int i = 0; i < m_WNum; i++)
        {
            if (m_WArray[i]->m_Name == name)
            {
                flag = true;
                cout << "查找到名为：" << name << "的职工!" << endl;
                m_WArray[i]->showInfo();
            }
        }

        if (!flag)
        {
            cout << "该姓名不存在!" << endl;
            pauseAndClear();
            return;
        }

        pauseAndClear();        
    }
}

void WorkerManageSystem::sortById()
{
    if (m_FileIsEmpty)
    {
        cout << "文件为空!" << endl;
        pauseAndClear();
        return;
    }

    cout << "1.按职工编号排升序" << endl;
    cout << "2.按职工编号排降序" << endl;
    cout << "请选择排序方式：" << endl;

    int choice;
    cin >> choice;

    //选择排序
    for (int i = 0; i < m_WNum; i++)
    {
        int MinOrMax = i;
        for (int j = i + 1; j < m_WNum; j++)
        {
            if (choice == 1) //升序
            {
                if (m_WArray[MinOrMax]->m_Id > m_WArray[j]->m_Id) MinOrMax = j; //拿到id更小的对象的下标
            }
            else //降序
            {
                if (m_WArray[MinOrMax]->m_Id < m_WArray[j]->m_Id) MinOrMax = j; //拿到id更大的对象的下标
            }
        }

        //交换m_WArray[i] 和 第i轮找出的最大or最小值
        if (i != MinOrMax)
        {
            Worker* tmp = m_WArray[i];
            m_WArray[i] = m_WArray[MinOrMax];
            m_WArray[MinOrMax] = tmp;
        }
    }

    cout << "排序成功!" << endl;

    save();

    showWorkers();
}

void WorkerManageSystem::CleanFile()
{
    cout << "确定要清空所有数据吗？" << endl;
    cout << "1.确认" << endl;
    cout << "2.取消" << endl;
    
    int choice;
    cin >> choice;

    if (choice == 1)
    {
        ifstream ifs(FILENAME, ios::trunc); //通过trunc的方式摧毁文件 

        //WorkerManageSystem在构造的时候会通过init_Worker()将文件中的数据存到堆区维护，在清空数据时，需要将堆区数据释放，注意：指针数组的空间 及其 每个指针的空间 都要释放
        for (int i = 0; i < m_WNum; i++)
        {
            if (m_WArray != NULL)
            {
                delete m_WArray[i];
                m_WArray[i] = NULL;
            }
        }

        m_WNum = 0;
        delete[] m_WArray;
        m_WArray = NULL;
        m_FileIsEmpty = true;
        cout << "已成功清空所有数据，准备跑路!" << endl;
    }
    else
    {
        cout << "已退出!" << endl;      
    }

    pauseAndClear();
}

void WorkerManageSystem::showMenu()
{
    cout << "==================================" << endl;
    cout << "       职工管理系统菜单" << endl;
    cout << "==================================" << endl;
    cout << "        1. 添加职工" << endl;
    cout << "        2. 删除职工" << endl;
    cout << "        3. 修改职工信息" << endl;
    cout << "        4. 查找职工" << endl;
    cout << "        5. 显示所有职工" << endl;
    cout << "        6. 按编号排序" << endl;
    cout << "        7. 清空所有数据" << endl;
    cout << "        0. 退出系统" << endl;
    cout << "==================================" << endl;
}

void WorkerManageSystem::exitSystem()
{
    cout << "感谢使用，已退出系统!" << endl;
    exit(0);
}

WorkerManageSystem::~WorkerManageSystem()
{
    if (m_WArray != NULL)
    {
        delete[] m_WArray;
        m_WArray = NULL;
    }
}