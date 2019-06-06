/* 
    版本四: 将三个不同类的多个对象序列化/反序列化到文件 
    采用面向对象的方法，查询变化点，抽象出需要进行序列化的基类
    当有新的类的对象加入时，不能修改已有的代码
*/
#include <iostream>
#include <fcntl.h>
#include <vector>
#include <unistd.h>

using namespace std;

class FaSerializer   // 虚函数 基类
{
    public:
    FaSerializer()
    {

    }

    virtual ~FaSerializer()
    {

    }

    public:
    virtual bool Serialize(int fd) = 0;             // 序列化 
    virtual FaSerializer* Deserialize(int fd) = 0;  // 反序列化
    virtual bool GetType(int& type) = 0;            // 返回类type值

};

class A : public FaSerializer   // A类 继承 基类
{
    public:    
    A()                         // 无参构造函数
    {
        number = 0;
    }

    explicit A(int number)      // 显性单参数构造函数
    {
        this->number = number;
    }

    virtual ~A()                // 析构函数
    {

    }

    public:
    void coutData()
    {
        cout << "number in A: " << number << endl;
    }

    public:
    virtual bool GetType(int& type)   // 返回 A类 为 type=0
    {
        type = 0;
        return true;
    }

    virtual bool Serialize(int fd)    // A类 内部 序列化函数
    {
        if(fd == -1)
        {
            return false;
        }

        int wc = write(fd, &number, sizeof(int));  // 写入number[int]数据

        if(wc == -1)
        {
            return false;
        } 
        return true;
    }

    virtual FaSerializer* Deserialize(int fd)    // A类 内部 反序列化函数
    {
        if (fd == -1)
        {
            return NULL;
        }

        A *p = new A();             // 动态内存分配

        int rc = read(fd, &(p->number), sizeof(int));
        if((rc == 0) || (rc == -1))
        {
            delete p;               // 释放内存
            return NULL;
        }
        
        return p;
    }

    private:
    int number;         // A类 number数据成员
};

class B : public FaSerializer     // B类 继承 基类
{
    public:
    B()        // 无参构造函数
    {
        number = 0;
        alpha = 0;
    }

    B(int number, char alpha)      // 带参数构造函数
    {
        this->number = number;
        this->alpha = alpha;
    }

    virtual ~B()      // 析构函数
    {

    }

    public:
    void coutData()   // 输出数据函数
    {
        cout << "number in B: " << number << endl;
        cout << "alpha in B: " << alpha << endl;
    }

    public:
    virtual bool GetType(int& type)     // 返回 B类 为 type=1
    {
        type = 1;
        return type;
    }

    virtual bool Serialize(int fd)     // B类 内部 序列化函数
    {
        if(fd == -1)
        {
            return false;
        }

        int wc1 = write(fd, &number, sizeof(int));   // 写入number[int]数据
        int wc2 = write(fd, &alpha, sizeof(char));   // 写入alpha[char]数据

        if((wc1 == 0) || (wc1 == -1))
        {
            return false;
        }

        if((wc2 == -1) || (wc2 == 0))
        {
            return false;
        }

        return true;
    }

    virtual FaSerializer* Deserialize(int fd)    // B类 内部 反序列化函数
    {
        if(fd == -1)
        {
            return NULL;
        }

        B *q = new B();                      // 动态内存分配

        int rc1 = read(fd, &(q->number), sizeof(int));    // 读取number[int]数据
        int rc2 = read(fd, &(q->alpha), sizeof(char));    // 读取alpha[char]数据

        if((rc1 == 0) || (rc1 == -1))
        {
            delete q;                 // 释放空间
            return NULL;
        }

        if((rc2 == -1) || (rc2 == 0))
        {
            delete q;                 // 释放空间
            return NULL;
        }

        return q;
    }

    private:
    int number;             // B类 number数据成员
    char alpha;             // B类 alpha数据成员
};

class C : public FaSerializer     // C类 继承 基类
{
    public:
    C()        // 无参构造函数
    {
        number = 0;
        alpha = 0;
    }

    C(char number, char alpha)      // 带参数构造函数
    {
        this->number = number;
        this->alpha = alpha;
    }

    virtual ~C()      // 析构函数
    {

    }

    public:
    void coutData()   // 输出数据函数
    {
        cout << "number in C: " << number << endl;
        cout << "alpha in C: " << alpha << endl;
    }

    public:
    virtual bool GetType(int& type)     // 返回 C类 为 type=2
    {
        type = 2;
        return type;
    }

    virtual bool Serialize(int fd)     // C类 内部 序列化函数
    {
        if(fd == -1)
        {
            return false;
        }

        int wc1 = write(fd, &number, sizeof(char));   // 写入number[char]数据
        int wc2 = write(fd, &alpha, sizeof(char));   // 写入alpha[char]数据

        if((wc1 == 0) || (wc1 == -1))
        {
            return false;
        }

        if((wc2 == -1) || (wc2 == 0))
        {
            return false;
        }

        return true;
    }

    virtual FaSerializer* Deserialize(int fd)    // C类 内部 反序列化函数
    {
        if(fd == -1)
        {
            return NULL;
        }

        C *t = new C();                      // 动态内存分配

        int rc1 = read(fd, &(t->number), sizeof(char));    // 读取number[char]数据
        int rc2 = read(fd, &(t->alpha), sizeof(char));    // 读取alpha[char]数据

        if((rc1 == 0) || (rc1 == -1))
        {
            delete t;                 // 释放空间
            return NULL;
        }

        if((rc2 == -1) || (rc2 == 0))
        {
            delete t;                 // 释放空间
            return NULL;
        }

        return t;
    }

    private:
    char number;             // C类 number数据成员
    char alpha;              // C类 alpha数据成员
};

class serializerTool        // 多类多对象序列化/反序列化工具
{
    private:
    vector<FaSerializer*> m_vSerialize;      // 类的类型集合

    public:
    bool Serialize(const char *pFP,vector<FaSerializer*>& v)  // 多类多对象序列化函数
    {
        int fd = open(pFP, O_RDWR | O_CREAT | O_TRUNC, 0);  // O_RDWR 读写权限
                                                            // O_CREAT 如指定文件不在，则创建
                                                            // O_TRUNC 如果文件存在，则清空文件全部内容
        if(fd == -1)
        {
            cout << "[error]读写权限不够，请sudo执行" << endl;      // 读写权限提示
            return false;
        } 

        for(int i = 0;i < v.size();i++)
        {
            int type;
            v[i]->GetType(type);

            if(write(fd, &type, 4) == -1)                   // 先写入类的type值
            {
                close(fd);
                return false;
            }

            v[i]->Serialize(fd);                            // 再调用类内部的序列化函数
        }

        if(close(fd) == -1)
        {
            return false;
        }
        return true;
    }

    bool Deserialize(const char *pFP, vector<FaSerializer*>& v)   // 多类多对象反序列化函数
    {
        int fd = open(pFP, O_RDWR);                               // O_RDWR 读写权限
        if(fd == -1)
        {
            return false;
        }

        for(;;)
        {
            int nType;
            int rn = read(fd, &nType, 4);                         // 先读取类的type值，识别类的类型
            if((rn == 0) || (rn == -1))
            {
                break;
            }

            int type;
            for(int i = 0;i < m_vSerialize.size(); i++)
            {
                m_vSerialize[i]->GetType(type);
                if(type == nType)                              // 判断type值是否已注册
                {
                    FaSerializer *p = m_vSerialize[i]->Deserialize(fd);   // 调用类内部反序列胡函数
                    if(p!=NULL)
                    {
                        v.push_back(p);
                    }
                }
            }
        }

        if(close(fd) == -1)
        {
            return false;
        }
        return true;
    }

    void Register(FaSerializer *p)      // 类的类型注册函数
    {
        m_vSerialize.push_back(p);
    }
};

int main()
{
    {   // Serialize three Class more Obj
        A a1(2);
        B b1(3,'T');
        C c1('4', 'Y');
        C c2('7', 'u');
        B b2(4, 'a');
        A a2(5);

        vector<FaSerializer*> v;

        v.push_back(&a1);
        v.push_back(&b1);
        v.push_back(&c1);
        v.push_back(&c2);
        v.push_back(&b2);
        v.push_back(&a2);

        serializerTool s;
        s.Serialize("data4", v);
    }

    {   // Deserialize three Class more Obj
        serializerTool s;
        A a;
        B b;
        C c;
        s.Register(&a);     // 注册各类的类型
        s.Register(&b);
        s.Register(&c);

        vector<FaSerializer*> v;
        s.Deserialize("data4", v);

        for(int i=0; i< v.size(); i++)
        {
            A *p =dynamic_cast<A *>(v[i]);
            if(p!= NULL)
            {
                p->coutData();
            }

            B *q =dynamic_cast<B *>(v[i]);
            if(q != NULL)
            {
                q->coutData();
            }

            C *t =dynamic_cast<C *>(v[i]);
            if(t != NULL)
            {
                t->coutData();
            }
        }
    }
    return 0;
}