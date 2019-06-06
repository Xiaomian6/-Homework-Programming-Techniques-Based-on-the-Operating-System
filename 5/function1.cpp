#include <iostream>
#include "IPrintPlugin.h"

using namespace std;

class CPrintPlugin : public IPrintPlugin    // 继承基类
{
	public:
		CPrintPlugin()
		{
			FUNC_ID = 2;
		}

		virtual ~CPrintPlugin()
		{
		}

		virtual void Print()
		{
			cout << "Hello China!" << endl;
		}

		virtual void Help()
		{
			cout << "Function ID " << FUNC_ID << " : This function will print hello china." << endl;
		}

		virtual int GetID(void)
		{
			return FUNC_ID;
		}
	private:
		int FUNC_ID;    // 功能ID
};

extern "C" void CreateObj(IPrintPlugin **ppPlugin)  // 创建对象
{
	static CPrintPlugin plugin;
	*ppPlugin = &plugin;
}
