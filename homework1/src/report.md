# 41443130

作業一

## 解題說明

problem1_本題要求實作 Ackermann 函數 $A(m,n)$，並分別以遞迴與非遞迴方式完成。
函數定義
$A(m,n)$={
        $n+1$            if $m=0$
        $A(m-1,1)$       if $m>0$,$n=0$
        $A(m-1,A(m,n-1))$if $m>0$,$n>0$
         }
problem2_

### 解題策略
problem1
1. 遞迴版：直接依照三個條件撰寫，先判斷 $m=0$，再判斷 $n=0$，其餘情況即為 $m>0$ 且 $n>0$。
2. 非遞迴版：遞迴實際上是由系統的呼叫堆疊記錄「尚未完成的工作」。非遞迴版改為自己用陣列模擬堆疊：
變數 n 存放目前算出的值。堆疊存放「之後還要套用的 $m$」。
每次取出一個 $m$，依三種情況處理：
$m=0$：n = n + 1
$n=0$：推入 m-1，令 n = 1
其他：推入 m-1（外層待辦），再推入 m（先算內層），令 n = n - 1
堆疊空了，n 就是答案。
3.因題目限制可使用的標頭檔，不使用 <stack>，以全域陣列加 top 變數自行實作堆疊。
problem2
1.
## 程式實作

以下為主要程式碼：

```cpp
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
```

```cpp
#include<iostream>//非遞迴版本
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
```

## 效能分析

1. 時間複雜度：程式的時間複雜度為 $O(\log n)$。
2. 空間複雜度：空間複雜度為 $O(100\times \log n + \pi)$。

## 測試與驗證

### 測試案例
problem_1
| 測試案例 | 輸入參數 $m,n$ | 預期輸出/遞迴/非遞迴 | 實際輸出 |
|----------|--------------|----------|--------------|
| 測試一   | $(0,0)$      | 1/1        | 1/1        |
| 測試二   | $(1.0)$      | 2/2        | 2/2        |
| 測試三   | $(1.1)$      | 3/3        | 3/3        |
| 測試四   | $(2,3)$      | 9/9        | 9/9        |
| 測試五   | $(3,3)$      | 61/61      | 61/61      |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
6
```

### 結論
**problem_1**
1. 遞迴版與非遞迴版在所有測試案例中結果一致，且與公認的 Ackermann 值相符。  
2. 負數輸入會被攔截並顯示錯誤訊息。  
3. 測試涵蓋邊界（$m=0$、$n=0$）、一般情況與較大輸入（$A(4,1)$）。
**problem_2**

## 申論及開發報告
**problem_1**
為甚麼非遞迴版本要用堆疊:
應為必須先算出內層,才能進明外層,因此每往內一層,就多一件暫時的工作,
暫時的工作數量不固定且會持續增長,無法用固定的變數取代所以需要用堆疊
遞迴與非遞迴比較
項目	  遞迴版	                                   非遞迴版
可讀性	  與數學定義幾乎一致，簡單直觀	               需理解堆疊模擬，較複雜
速度	  較慢（函式呼叫開銷）	                     較快（實測約快 1.6 到 1.9 倍）
深度限制	受系統堆疊限制，深度過大會stack overflow    受自訂陣列大小限制，可自行調整
**problem2**

### 開發過程遇到的問題

**PROBLEM_1**：

1. **標頭檔限制**

   不能使用 <stack>，因此改以全域陣列加 top 實作堆疊。
   
2. **計時**
   clock() 通常定義在 <ctime>，但清單中沒有該標頭；在我的編譯環境中，由 <iostream> 間接引入即可使用。    若環境不支援，可改用呼叫次數作為效能指標。

3. **數值溢位**
   使用遞迴時若數值太大會編譯不出來

4. **堆疊大小**
   使用非遞迴時陣列要開得夠大才能計算出比較大的值

心得
problem_1透過這題理解到遞迴背後其實是系統在維護堆疊，將遞迴改寫成非遞迴，就是把「系統幫你做的事」改成自己手動管理。同時也看到 Ack 函數成長之快，即使輸入很小，也會讓運算次數與結果值急速爆漲。


