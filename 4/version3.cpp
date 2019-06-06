/* 版本三: 将两个不同类的多个对象序列化/反序列化到文件 */
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

using namespace std;

class A
{
    public:
    A()                 // 无参构造函数
    {
        number = 0;
    }

    explicit A(int number)     // 显性单参数构造函数
    {
        this->number = number;
    }

    virtual ~A()   // 析构函数
    {

    }

    public:
    void coutData()            // 输出数据函数
    {
        cout << "number in A:" << number << endl;
    }

    public:
    bool Serialize(int fd)       // A类 内部序列化函数
    {
        if(fd == -1)
        {
            return false;
        }

        if(write(fd, &number, sizeof(int)) == -1)   // 写入number[int]数据
        {
            return false;
        }

        return true;
    }

    bool Deserialize(int fd)   // A类 内部反序列化函数
    {
        if(fd == -1)
        {
            return false;
        }

        int rc = read(fd, &number, sizeof(int));    // 读取number[int]数据
        if((rc == -1) || (rc == 0))
        {
            return false;
        }

        return true;
    }
    
    private:
    int number;                 // A类 number数据成员
};

class B
{
    public:
    B()              // 无参构造函数
    {
        number = 0;
        alpha = 0;
    }

    B(int number, char alpha)    // 带参构造函数
    {
        this->number = number;
        this->alpha = alpha;
    }

    virtual ~B()   // 析构函数
    {
        
    } 

    public:
    void coutData()     // 输出数据函数
    {
        cout << "number in B:" << number << endl;
        cout << "alpha in B:" << alpha << endl;
    }

    public:
    bool Serialize(int fd)        // B类 内部 序列化函数
    {
        if(fd == -1)
        {
            return false;
        }

        if(write(fd, &number, sizeof(int)) == -1)      // 写入number[int]数据
        {
            return false;
        }

        if(write(fd, &alpha, sizeof(char)) == -1)           // 写入alpha[char]数据
        {
            return false;
        }

        return true;
    }

    bool Deserialize(int fd)
    {
        if(fd == -1)
        {
            return false;
        }

        int rc1 = read(fd, &number, sizeof(int));        // 读取number[int]数据

        int rc2 = read(fd, &alpha, sizeof(char));        // 读取alpha[char]数据

        if((rc1 == 0) || (rc1 == -1))
        {
            return false;
        }
        if((rc2 == 0) || (rc2 == -1))
        {
            return false;
        }

        return true;
    }

    private:
    int number;                               // B类 number数据成员
    char alpha;                               // B类 alpha数据成员
};

struct SerializeClass   // 不同类集合 结构体
{
    int nType;          // Class A for 0 , B for 1
    void *pObj;
};

class serializerTool    // 多类多对象序列化/反序列化工具
{
    public:
    serializerTool()
    {

    }

    virtual ~serializerTool()
    {

    } 

    public:
    bool Serialize(const char *pFP,vector<SerializeClass>& v)   // 多类多对象序列化函数
    {
        int fd = open(pFP, O_RDWR | O_CREAT | O_TRUNC , 0);     // O_RDWR 读写权限
                                                                // O_CREAT 如指定文件不在，则创建
                                                                // O_TRUNC 如果文件存在，则清空文件全部内容
        if(fd == -1)
        {
            cout << "[error]读写权限不够，请sudo执行" << endl;      // 读写权限提示
            return false;
        }

        for(int i = 0; i < v.size(); i++)
        {
            if(write(fd, &(v[i].nType), 4) == -1)        // write ClassType
            {
                close(fd);
                return false;
            }

            if(v[i].nType == 0)                          // A class
            {
                A *p = (A *)(v[i].pObj);
                if((p->Serialize(fd)) == -1)
                {
                    return false;
                }
            }
            else if(v[i].nType == 1)                      // B class
            {
                B *p = (B *)(v[i].pObj);
                if((p->Serialize(fd)) == -1)
                {
                    return false;
                }
            }
        }

        if(close(fd) == -1)
        {
            return false;
        }
        return true;
    }

    bool Deserialize(const char *pFP,vector<SerializeClass>& v)   // 多类多对象反序列化函数
    {
        int fd = open(pFP, O_RDWR);                      // O_RDWR 读写权限
        if(fd == -1)
        {
            return false;
        }

        for(;;)
        {
            int nType;
            int re = read(fd, &nType, 4);     // read ClassType
            if((re == -1) || (re == 0))
            {
                break;
            }

            if(nType == 0)                 // A class
            {
                A *p;
                p = new A();
                p->Deserialize(fd);

                SerializeClass s;
                s.nType = nType;
                s.pObj = p;

                v.push_back(s);
            }
            else if(nType == 1)             // B class
            {
                B *p;
                p = new B();
                p->Deserialize(fd);

                SerializeClass s;
                s.nType = nType;
                s.pObj = p;

                v.push_back(s);
            }
        }

        if(close(fd) == -1)
        {
            return false;
        }

        return true;
    }
};

int main()
{
    {   // Serialize two Class more Obj
        A a1(4);
        SerializeClass s1;
        s1.nType = 0;
        s1.pObj = &a1;

        A a2(6);
        SerializeClass s2;
        s2.nType = 0;
        s2.pObj = &a2;

        B b1(8, 'd');
        SerializeClass s3;
        s3.nType = 1;
        s3.pObj = &b1;

        B b2(10, 't');
        SerializeClass s4;
        s4.nType = 1;
        s4.pObj = &b2;

        vector<SerializeClass> v;
        v.push_back(s1);
        v.push_back(s2);
        v.push_back(s3);
        v.push_back(s4);

        serializerTool s;
        s.Serialize("data3", v);

    }

    {   // Deserialize two Class more Obj
        serializerTool s;
        vector<SerializeClass> v;
        s.Deserialize("data3", v);

        for(int i=0; i<v.size(); i++)
        {
            if(v[i].nType == 0)
            {
                A *p = (A *)(v[i].pObj);
                p->coutData();
            }
            else if(v[i].nType == 1)
            {
                B *p = (B *)(v[i].pObj);
                p->coutData();
            }
        }
    }

    return 0;
}