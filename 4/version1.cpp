/* 版本一: 将一个类的一个对象序列化/反序列化到文件 */
#include <iostream>
#include <fcntl.h>
#include <vector>
#include <string.h>
#include <unistd.h>

using namespace std;

class A                         // A类
{
    public:
    A()                         // 构造函数
    {
        alpha = 0;              // char初始化
    }

    explicit A(char alpha)      // 显性单参数构造函数
    {
        this->alpha = alpha; 
    }

    virtual ~A()                // 析构函数  
    {

    }

    public:
    void coutAlpha()            // 输出alpha数据函数
    {
        cout << "alpha in A:" << this->alpha << endl;
    }

    public:
    bool Serialize(const char *pFilePath) const                    // A类 内部 序列化函数 序列化成功-true 失败-false 
    {
        int fd = open(pFilePath, O_RDWR | O_CREAT | O_TRUNC, 0);   // O_RDWR 读写权限
                                                                   // O_CREAT 如指定文件不在，则创建
                                                                   // O_TRUNC 如果文件存在，则清空文件全部内容
        if(fd == -1)  
        {
            cout << "[error]读写权限不够，请sudo执行" << endl;         // 读写权限提示
            return false; 
        }                             

        if(write(fd, &alpha, sizeof(char)) == -1)                   // 写入alpha[char]对象数据
        {
            close(fd);
            return false;
        }

        if(close(fd) == -1)
        {
            return false;
        }else
        {
            return true;
        }
    }

    bool Deserialize(const char *pFilePath)                         // A类 内部 反序列化函数 成功-true 失败-false 
    {
        int fd = open(pFilePath, O_RDWR);                           // O_RDWR 读写权限
        if(fd == -1)
        {
            return false;
        }

        int r2 = read(fd, &alpha, sizeof(char));                    // 读取alpha[char]对象数据
        if(r2 == -1)
        {
            close(fd);
            return false;
        }

        if(close(fd) == -1)
        {
            return false;
        }else
        {
            return true;
        }
        
    }

    private:
    char alpha;              // alpha数据成员
};

int main()                   // main函数测试 序列化/反序列化一个类一个对象
{
    {                        // 序列化测试
        A a('X');
        a.Serialize("data1");
    }

    {                        // 反序列化测试
        A b;
        b.Deserialize("data1");
        b.coutAlpha();
    }

    return 0;
}
