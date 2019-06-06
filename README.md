# Programming_Techniques_Based_on_the_Operating_System
[Homework]Programming Techniques Based on the Operating System

>基于Linux API编程

~~~
  $ g++ -o [编译后的文件名] [需要编译的文件]
  eg：
  $ g++ -o test1 1.cpp
~~~

## Homework1
- 当设定O_APPEND标志后，能否使用lseek函数修改当前文件偏移量？若能修改，读写操作发生在文件何处？

## Homework2
- 给定一个目录文件，打印该目录下所有子目录和文件的名称，并不断递归访问子目录，并打印其属子目录的文件名称。

## Homework3
- 实现“ls -l”的基本功能，能输出文件类型、9个权限位信息、文件大小、修改时间以及文件名称。

## Homework4
- version1:将一个类的一个对象序列化到文件

- version2:将一个类的多个对象序列化到文件

- version3:将两个不同类的多个对象序列化到文件

- version4:将三个不同类的多个对象序列化到文件
	- 采用面向对象的方法，抽象出需要进行序列化的基类;
	- 当又有新的类的对象，需要序列化到文件时，不能修改已有的代码

## Homework5
- 插件框架 
	- 通过命令行方式：./a.out help，输出所有插件实现的功能ID，以及该功能ID对应的功能描述
	- 通过命令行方式：./a.out FuncID，调用具体打印功能（每个插件导出GetID接口）

~~~
	$ g++ -fpic -shared -o f1.so function.cpp IPrintPlugin.cpp  // 编译动态链接库
	$ g++ -o a.out main.cpp CPluginController.cpp CPluginEnumerator.cpp -ldl  // 编译主程序
~~~

