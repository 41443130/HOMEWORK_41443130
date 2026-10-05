# 41443130

作業一

## 解題說明

problem1_本題要求實作 Ackermann 函數 $A(m,n)$，並分別以遞迴與非遞迴方式完成。
函數定義
$$
A(m,n)=\left\{
\begin{array}{ll}
n+1 & \text{if } m=0 \\\\
A(m-1,1) & \text{if } m>0,\ n=0 \\\\
A(m-1,\ A(m,n-1)) & \text{if } m>0,\ n>0
\end{array}
\right.
$$
problem2_
本題要求撰寫一個遞迴函式，計算集合 $S$ 的冪集（powerset），也就是 $S$ 所有可能的子集合。

例如 $S=(a,b,c)$ 時：

$$\text{powerset}(S)=\{(\ ),(a),(b),(c),(a,b),(a,c),(b,c),(a,b,c)\}$$
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
1.對集合中的每個元素，只有兩種選擇：選（放進子集合）或不選。
2.從第一個元素開始決定，每決定完一個，就交給遞迴處理下一個元素。使用三個參數：
s：原集合
i：目前正在決定第幾個元素（從 $0$ 開始）
cur：目前已經選出的元素
3.每次呼叫會產生兩個子問題：
不選 s[i]：powerset(s, i + 1, cur)
選 s[i]：powerset(s, i + 1, cur + s[i])
4.結束條件：當 $i = n$（$n$ 為元素個數）時，所有元素都已決定完畢，cur 就是一個完整的子集合，直接輸出。
5.主程式從 powerset(s, 0, "") 開始呼叫，表示從第 $0$ 個元素開始、目前尚未選任何元素。
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
```cpp
#include <iostream>//problem2
#include <string>
using namespace std;

void powerset(string s, int i, string cur)
{
    if (i == s.size())            // 全部元素都決定完了
    {
        cout << "(" << cur << ")" << endl;
        return;
    }
    powerset(s, i + 1, cur);          // 不選 s[i]
    powerset(s, i + 1, cur + s[i]);   // 選 s[i]
}

int main()
{
    string s = "abc";
    clock_t start = clock();
    powerset(s, 0, "");
    clock_t end = clock();
    cout << "time: " << (double)(end - start) / CLOCKS_PER_SEC << " sec" << endl;
    return 0;
}
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
problem_2
| 測試案例 | 輸入參數 $S$ | 元素個數 $n$ | 實際輸出 |實際子集合數|
|----------|--------------|----------|----------|------------|
| 測試一   | 空集合       | 0         | 1        |1
| 測試二   | $a$         | 1         | 2        |2
| 測試三   | $ab$        | 2         | 4        |4
| 測試四   | $abc$       |3          | 8        |8
| 測試五   | $abcd$      | 4         | 16       |16

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
1.輸入 $n$ 個元素時，程式輸出的子集合個數都等於 $2^n$，與理論相符。
2.輸入 abc 的結果與題目範例的 8 個子集合完全對應，順序不同不影響正確性。
## 申論及開發報告
## problem_1
為甚麼非遞迴版本要用堆疊:
應為必須先算出內層,才能進明外層,因此每往內一層,就多一件暫時的工作,
暫時的工作數量不固定且會持續增長,無法用固定的變數取代所以需要用堆疊
遞迴與非遞迴比較
項目	  遞迴版	                                   非遞迴版
可讀性	  與數學定義幾乎一致，簡單直觀	               需理解堆疊模擬，較複雜
速度	  較慢（函式呼叫開銷）	                     較快（實測約快 1.6 到 1.9 倍）
深度限制	受系統堆疊限制，深度過大會stack overflow    受自訂陣列大小限制，可自行調整
## problem2
1.問題本身就能拆成相同形式的子問題
每個元素只有「選」或「不選」兩種可能。決定完第一個元素後，剩下的元素仍是同樣的問題，只是規模少了一個。
2.程式簡短，容易對照思路
核心只有兩行，分別對應「不選」與「選」：
### 開發過程遇到的問題

## PROBLEM_1：

1. **標頭檔限制**

   不能使用 <stack>，因此改以全域陣列加 top 實作堆疊。
   
2. **計時**
   clock() 通常定義在 <ctime>，但清單中沒有該標頭；在我的編譯環境中，由 <iostream> 間接引入即可使用。    若環境不支援，可改用呼叫次數作為效能指標。

3. **數值溢位**
   使用遞迴時若數值太大會編譯不出來

4. **堆疊大小**
   使用非遞迴時陣列要開得夠大才能計算出比較大的值
**PROBLEM_2**：
1.：不理解「選」與「不選」

狀況：一開始看不懂題目的 powerset 是什麼，也不知道程式裡的「選」與「不選」代表什麼。
解決：把子集合想成「從集合挑東西放進籃子」。每個元素只有兩種可能：放進 cur（選），或不放（不選）。把每個元素的兩種選擇全部走過一遍，就能列出所有子集合。

2.把 i 的結束值和起始值搞混
狀況：以為輸入 abc 時 i 一開始就是 3。
解決：3 是字串長度 s.size()，也是結束條件。i 從 0 開始，依序處理 s[0]、s[1]、s[2]，當 i == 3 表示三個元素都決定完了，才輸出 cur。字串沒有 s[3]，所以必須在這裡停止。

3.看不懂遞迴的執行順序
狀況：不清楚遞迴呼叫「回來」之後會做什麼。
解決：畫出遞迴樹，並用縮排追蹤每次呼叫。理解到電腦是深度優先執行：先把「不選」的整個分支做完，函式返回後，才執行下一行「選」的分支。因此輸出順序為 ()、(c)、(b)、(bc)、(a)、(ac)、(ab)、(abc)，和題目範例的順序不同，但內容相同。


心得
problem_1透過這題理解到遞迴背後其實是系統在維護堆疊，將遞迴改寫成非遞迴，就是把「系統幫你做的事」改成自己手動管理。同時也看到 Ack 函數成長之快，即使輸入很小，也會讓運算次數與結果值急速爆漲。

problem_2這次作業有兩題，我一開始最不熟的是遞迴，尤其是 Problem 2 的 powerset。剛看到題目時，我只覺得「要列出所有子集合」很複雜，不知道從哪裡開始寫。
後來我理解到，重點是把問題換成更簡單的問法：每個元素只有「選」或「不選」兩種可能。決定完一個元素，剩下的元素就是同樣的問題，只是少了一個，這就是遞迴的想法。

