#include<iostream>//遞迴版本
using namespace std;
long long ACK(long long m, long long n)
{
	if (m == 0)return n + 1;
	if (m > 0 && n == 0)return ACK(m - 1, 1);
	if (m > 0 && n > 0)return ACK(m - 1, ACK(m, n - 1));
}
int main()
{
	int m,n;
	cout << "enter two number: "<< endl;
	while (cin >> m >> n)
	{
		if (m < 0 || n < 0)
		{
			cout << "mistake" << endl;
			continue;
		}
		clock_t start = clock();
		long long result = ACK(m, n);
		clock_t end = clock();
		double sec = (double)(end - start) / CLOCKS_PER_SEC;
		cout << "A(" << m << "," << n << ") = " << result << endl;
		cout << "time: " << sec << " sec" << endl;
	}
	return 0;
}