/* 版本二: 将一个类的多个对象序列化/反序列化到文件 */
#include <iostream>
#include <fcntl.h>
#include <vector>
#include <unistd.h>

using namespace std;

class A
{
    public:
    A()                           // 无参构造函数
    {
        this->number = 0;         // 初始化
        this->alpha = 0;
    }

    A(int number, char alpha)          // 构造函数
    {
        this->number = number;         // 初始化
        this->alpha = alpha;
    }

    virtual ~A()                       // 析构函数
    {

    }

    public:
    void coutData()                    // 输出类的数据成员
    {
        cout << "number in A:" << number << endl;
        cout << "alpha in A:" << alpha << endl;
    }
    public:
    bool Serialize(const char *pFilePath) const                   // A类 内部 序列化函数 序列化成功-true 失败-false 
    {
        int fd = open(pFilePath, O_RDWR | O_CREAT | O_TRUNC,0);   // O_RDWR 读写权限
                                                                  // O_CREAT 如指定文件不在，则创建
                                                                  // O_TRUNC 如果文件存在，则清空文件全部内容
        if(fd == -1)
        {
            cout << "[error]读写权限不够，请sudo执行" << endl;       // 读写权限提示
            return false;
        }

        if(write(fd, &number, sizeof(int)) == -1)                 // 写入number[int]数据
        {
            close(fd);
            return false;
        }

        if(write(fd, &alpha, sizeof(char)) == -1)                 // 写入alpha[char]数据   
        {
            close(fd);
            return false;
        }

        if(close(fd) == -1)
        {
            return false;
        }
        return true;
    }

    bool Deserialize(const char *pFilePath)                        // A类 内部 反序列化函数 成功-true 失败-false
    {
        int fd = open(pFilePath,O_RDWR);                           // O_RDWR 读写权限
        if(fd == -1)
        {
            return false;
        }

        int r1 = read(fd, &number, sizeof(int));                   // 读取number[int]数据
        if(r1 == -1)
        {
            close(fd);
            return false;
        }

        int r2 = read(fd, &alpha, sizeof(char));                   // 读取alpha[char]数据
        if(r2 == -1)
        {
            close(fd);
            return false;
        }

        if(close(fd) == -1)
        {
            return false;
        }

        return true;
    }

    // 重载 类接口 (不修改已有代码，重载新接口函数)
    bool Serialize(int fd) const
    {
        if(fd == -1)
        {
            return false;
        }

        if(write(fd, &number, sizeof(int)) == -1)                 // 写入number[int]数据
        {
            return false;
        }

        if(write(fd, &alpha, sizeof(char)) == -1)                 // 写入alpha[char]数据   
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

        int r1 = read(fd, &number, sizeof(int));                   // 读取number[int]数据
        if((r1 == -1) || (r1 == 0))
        {
            return false;
        }

        int r2 = read(fd, &alpha, sizeof(char));                   // 读取alpha[char]数据
        if((r2 == -1) || (r2 == 0))
        {
            return false;
        }

        return true;
    }

    private:
    int number;                            // number数据成员
    char alpha;                            // alpha数据成员
};

class serializerTool                     // 多对象序列化/反序列化工具
{
    public:
    serializerTool()                     // 构造函数
    {

    }  

    virtual ~serializerTool()            // 析构函数
    {

    }

    public:
    bool Serialize(const char *pFilePath, const vector<A>& v)        // 多对象序列化
    {
        int fd = open(pFilePath, O_RDWR | O_CREAT | O_APPEND | O_TRUNC, 0);     // O_RDWR 读写权限
                                                                                // O_CREAT 如指定文件不在，则创建
                                                                                // O_APPEND 每次写入都在文件末尾
                                                                                // O_TRUNC 如果文件存在，则清空文件全部内容
        if(fd == -1)                                                            
        {
            cout << "[error]读写权限不够，请sudo执行" << endl;          // 读写权限提示
            return false; 
        }

        for(int i = 0;i < v.size(); i++)
        {
            v[i].Serialize(fd);   // 调用各自类内序列化函数
        }

        close(fd);
        return true;
    }

    bool Deserialize(const char *pFilePath, vector<A>& v)             // 多对象反序列化
    {
        int fd = open(pFilePath, O_RDWR);                             // O_RDWR 读写权限
        if(fd == -1)
        {
            return false;
        }

        for(;;)     
        {
            A a;
            if(a.Deserialize(fd) == true)   // 调用各自类内反序列化函数
            {
                v.push_back(a);
            }
            else
            {
                break;
            }
        }

        close(fd);
        return true;
    }

    
};

int main()     // main函数测试 序列化/反序列化一个类多个对象
{
    {          // 序列化测试
        A a1(1, 'a'), a2(2, 'b'), a3(3, 'c');
        vector<A> v;
        v.push_back(a1);
        v.push_back(a2);
        v.push_back(a3);

        serializerTool s;
        s.Serialize("data2", v);
    }

    {          // 反序列化测试
        serializerTool s;
        vector<A> v;

        s.Deserialize("data2", v);
        for(int i = 0;i < 3;i++)
        {
            v[i].coutData();
        }
    }

    return 0;
}