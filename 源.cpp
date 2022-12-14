#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>

						位段
	跟结构体声明和结构基本相同，but
  1.其成员是int,signed int,unsigned int 或 char(整型家族)
  2.其空间是按需要以4字节(int)或1字节(char)的方式开辟
  3.涉及不确定因素过多，且不跨平台，可移植的程序避免使用
struct S1 
{
	int a : 2;	// a是int共2^32次方个数据，不过其只有4个数0，1，2，3，所以只需要2个bit=2^2就可以
	内存中分配int型共32比特位，a占2个，b占5个，c占10个，剩余15个，不够d的30个，舍弃，开辟下一个int
	int b : 5;	// 同理，b 内共 5 个比特位的数据
	  
	int c : 10;	// 同理，c 内共 10 个比特位的数据
	int d : 30; //   30
	     开辟的第二个int共32字节，存入30个，剩下2个舍弃
	 计算大小，占用的2+5+10+30+舍弃的15+2 = 64 bits = 8字节
};
	  注意，:后面的数不应超过类型的最大字节数，如a/b/c/d-int不超过32字节，char不超过8字节
struct S2
{
	char a : 2;
	char b : 5;
	char c : 7;
	char d : 8;
};
int main()
{
	struct S1 s1;
	struct S2 s2;
	printf("%d ", sizeof(s1));
	printf("%d ", sizeof(s2));
	return 0;
}

struct S2
{
	char a : 3; //  a 1字节8bit，占有8比特的3比特
	char b : 4; //	占有8比特的4比特，排在a后
	char c : 5;	//  占有第二个8比特的5比特
	char d : 4; //  占有第三个8比特的4字节
	0 0 0 0 0 0 0 0 |0 0 0 0 0 0 0 0 |0 0 0 0 0 0 0 0 
};
int main()
{
	struct S2 s = { 0 };
	s.a = 10;//1010	 占用第一个8比特a的3字节，1010的后面3个-010在a的3字节内
	s.b = 20;//10100 占用第一个8比特b的4字节，10100后面的4个，0100在a的剩余4字节内
	s.c = 3; //011   占用第二个8比特c的5字节，010前面补00010到5比特，存在c内，
	s.d = 4; //100   占用第三个8比特d的4字节，100前面补0100到4比特，存在d内
		0 0 1 0 0 0 1 0 | 0 0 0 0 0 0 1 0 | 0 0 0 0 0 1 0 0 
	  16进制存储，每4字节算
	  0010 | 0010 | 0000| 0010 | 0000 | 0100
	   2       2     0      3      0      4   - 22 03 04 - &s
	printf("%d ", sizeof(s));
	return 0;
}

		位段的跨平台问题
	1.int位段被当成有符号数还是无符号数是不确定的。
	2.位段中最大位的数目不能确定。(16位机器最大16，32位机器最大32，写成27，在16位机器会出问题。)
	3.位段中的成员在内存中从左向右分配，还是从右向左分配标准尚未定义
	4.当一个结构包含两个位段，第二个位段成员比较大，无法容纳于第一个位段剩余的位时，是舍弃剩余的位还是利用, 这是不确定的。
	总结 :
		跟结构相比，位段可以达到同样的效果，但是可以很好的节省空间，但是有跨平台的问题存在。

   应用：上网时，数据包可以分装，节省空间