#include<iostream>
using namespace std;

long long st[100000];//堆疊用陣列
int top = 0;//堆疊裡面有幾個東西
long long ack(long long m, long long n)
{
	top = 0;
	st[top] = m;//push m
	top++;
	while (top > 0)
	{
		top--;//pop
		m = st[top];
		if (m == 0)
		{
			n = n + 1;
		}
		else if (n == 0)
		{
			st[top] = m - 1;//push m-1
			top++;
			n = 1;
		}
		else
		{
			st[top] = m - 1;//push m-1
			top++;
			st[top] = m;//push m
			top++;
			n = n - 1;
		}
	}
	return n;
}
int main()
{
	long long m, n;
	while (cin >> m >> n)
	{
		clock_t start = clock();//開始計時
		long long result = ack(m, n);//呼叫非遞迴
		clock_t end = clock();//結束計時
		double sec = (double)(end - start) / CLOCKS_PER_SEC;
		cout << "A(" << m << "," <<n<< ")=" << result << endl;
		cout << "time: " << sec << " sec" << endl;
	}
	return 0;
}
/*
條件	        結果
m == 0	        A(m, n) = n + 1
m > 0 且 n == 0	A(m, n) = A(m - 1, 1)
m > 0 且 n > 0	A(m, n) = A(m - 1, A(m, n - 1))
*/