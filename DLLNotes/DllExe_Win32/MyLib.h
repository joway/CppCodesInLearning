#ifdef MYLIBAPI

#else

#define MYLIBAPI extern "C" __declspec(dllimport)

#endif

//���涨���������ݽṹ������
//...

//���涨�����е����ı���(��ò�Ҫ��������)
//...

//���涨�����е����ĺ���ԭ��
MYLIBAPI int add(int a, int b);

//EOF