#ifdef MYLIBAPI

#else

#define MYLIBAPI extern "C" __declspec(dllimport)

#endif

//下面定义所有数据结构，符号
//...

//下面定义所有导出的变量(最好不要导出变量)
//...

//下面定义所有导出的函数原型
MYLIBAPI int add(int a, int b);

//EOF