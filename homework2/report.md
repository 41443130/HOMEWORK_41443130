# 41443130

作業二

### 解題說明
想法<br>
用Trem(係數coef,指數exp)存在一個非零項,Polynomial用動態陣列termArray存所有非0項,
並維持指數由大到小,同指數只能出現一次<br>
1.capacity:陣列大小,terms:目前有幾個非零項<br>
2.newTerm:放入一項,係數為零就略過,陣列滿了就*2,開新陣列,複製,釋放舊陣列<br>
3.應為用動態陣列會把兩個物件在同一個陣列,所以會被刪掉造成重複釋放,所以要用複製建構子<br>
4.operator>>:讀入後做三件事1排序2合併指數,因為每個指數只能有一項,使用者輸入時可能不造規則寫<br>
例如3X^2+5X^2,正常來說是8X^2但程式會把它存成2項,3移除係數為零的項<br>
5.Add:兩個以排序的多項式,每次比較目前只到的兩項,把該先放的放進結果,指數項相同則係數相加<br>
6.Mult:A的每一項*B的所有項放進temp,再用Add加進結果<br>
7.Eval:每一項算coef*pow(x,exp)後加總<br>
##舉例說明
(x+1)(x+1):
|a的項 |乘所B所有項得到的temp|result(用Add累積)|
|---|---|---|
|x    | x^2,x              |x^2+x             |
|1    | x,1                |x^2+2x+1          |
### 2.Algorithm Design&Programming(40%)
## 輸入格式
項數 係數 指數 係數 指數...,例如 `3 2 3 2 1 -5 0` 代表 2x^3 + 2x - 5
## 選單功能<br>
1.輸入A<br>
2.輸入B<br>
3.顯示A和B<br>
4.A+B<br>
5.A*B<br>
6.代入x求A的值<br>
7.代入x求B的值<br>
0.離開<br>
## Source code+Comment<br>
```cpp
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
using namespace std;
class Polynomial;
class Term
{
    friend Polynomial;//可以直接存取term的private成員
    friend ostream& operator<<(ostream&, const Polynomial&);
    friend istream& operator>>(istream&, Polynomial&);
private:
    float coef;//係數
    int exp;//指數
};
class Polynomial
{
    friend ostream& operator<<(ostream&, const Polynomial&);
    //output stream輸出串流
    //ex(cout,p1)
    friend istream& operator>>(istream&, Polynomial&);
    //input stream輸入串流
private:
    Term* termArray;//存係數指數
    int capacity;//容量
    int terms;//有幾項
public:
    Polynomial() :capacity(2), terms(0)//預設建構子先把capacity設terms設0
    {
        termArray = new Term[capacity];//新陣列的大小
    }
    ~Polynomial() { delete[] termArray; }//解構子

    Polynomial(const Polynomial& o) :capacity(o.capacity), terms(o.terms)//複製建構子
    {
        termArray = new Term[capacity];
        copy(o.termArray, o.termArray + terms, termArray);
    }

    Polynomial& operator=(const Polynomial& o)//指定運算子
    {//開新陣列,複製p1的資料進去,釋放p3原本的舊陣列,改只項新的陣列
        if (this != &o)//檢查是不是自己指定給自己
        {
            Term* t = new Term[o.capacity];
            copy(o.termArray, o.termArray + o.terms, t);
            delete[] termArray;//釋放就陣列
            termArray = t;
            capacity = o.capacity;
            terms = o.terms;
        }
        return *this;
    }
    void newTerm(const float theCoef, const int theExp);
    Polynomial Add(const Polynomial& b);
    Polynomial Mult(const Polynomial& b);
    float Eval(float x);
};
void Polynomial::newTerm(const float theCoef, const int theExp)
{
    if (theCoef == 0)return;
    if (terms == capacity)
    {
        capacity *= 2;//容量*2
        Term* temp = new Term[capacity];//宣告temp的值是新的陣列大小
        copy(termArray, termArray + terms, temp);//(來源起點,來源終點,目的地起點)
        delete[]termArray;//刪掉舊的
        termArray = temp;//把temp的位置複製給termArray
    }
    termArray[terms].coef = theCoef;
    termArray[terms++].exp = theExp;//經過一個指數項加一項
}
Polynomial Polynomial::Add(const Polynomial& b)
{
    Polynomial c;
    int aPos = 0, bPos = 0;
    while ((aPos < terms) && (bPos < b.terms))//這樣寫的前提是有排序好<
    {
        if (termArray[aPos].exp == b.termArray[bPos].exp)//指數相同相加
        {
            float t = termArray[aPos].coef + b.termArray[bPos].coef;
            if (t)c.newTerm(t, termArray[aPos].exp);
            aPos++; bPos++;
        }
        else if (termArray[aPos].exp < b.termArray[bPos].exp)//b指數項比a大,b的項次加1
        {
            c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);//把B的係數和B的指數丟到C裡面
            bPos++;
        }
        else
        {
            c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
    }
    for (; aPos < terms; aPos++)//應為while那邊只要有a或b讀完就會結束,所以這邊要處理剩下的項
        c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
    for (; bPos < b.terms; bPos++)
        c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);

    return c;
}
Polynomial Polynomial::Mult(const Polynomial& b)
{
    Polynomial result;//一開始是0
    for (int i = 0; i < terms; i++)
    {
        Polynomial temp;
        for (int j = 0; j < b.terms; j++)
        {
            temp.newTerm(termArray[i].coef * b.termArray[j].coef, termArray[i].exp+b.termArray[j].exp);
        }
        result = result.Add(temp);//乘起來然後一列一列丟去加
    }
    return result;
}
float Polynomial::Eval(float x)//帶入x
{
    float result = 0;
    for (int i = 0; i < terms; i++)
    {
        result = result + termArray[i].coef * pow(x, termArray[i].exp);
    }
    return result;
}
istream& operator>>(istream& in, Polynomial& p)//>>運算子的重載//p=>要填入資料的那個多項式
{
    int n;//有幾組係數指數
    in >> n;
    p.terms = 0;// 先清空
    for (int i = 0; i < n; i++)//讀入係數和指數
    {
        float c; int e;
        in >> c >> e;
        p.newTerm(c, e);// 係數0會被略過
    }
    // 1. 插入排序:指數由大到小
    for (int i = 1; i < p.terms; i++)
    {
        Term key = p.termArray[i];
        int j = i - 1;
        while (j >= 0 && p.termArray[j].exp < key.exp)
        {
            p.termArray[j + 1] = p.termArray[j];
            j--;
        }
        p.termArray[j + 1] = key;
    }
    // 2. 合併相同指數
    int k = 0;
    for (int i = 0; i < p.terms; i++)
    {
        if (k > 0 && p.termArray[k - 1].exp == p.termArray[i].exp)
            p.termArray[k - 1].coef += p.termArray[i].coef;
        else
            p.termArray[k++] = p.termArray[i];
    }
    // 3. 移除係數變 0 的項
    int m = 0;
    for (int i = 0; i < k; i++)
        if (p.termArray[i].coef != 0) p.termArray[m++] = p.termArray[i];
    p.terms = m;
    return in;
}

ostream& operator<<(ostream& out, const Polynomial& p)//輸出符號
{
    if (p.terms == 0) { out << "0"; return out; }
    for (int i = 0; i < p.terms; i++)
    {
        float c = p.termArray[i].coef;
        int   e = p.termArray[i].exp;
        if (i == 0) { if (c < 0) out << "-"; }
        else out << (c < 0 ? " - " : " + ");
        float a = fabs(c);//取絕對值
        if (e == 0) out << a;//只印數字
        else
        {
            if (a != 1) out << a;
            out << "x";
            if (e != 1) out << "^" << e;
        }
    }
    return out;
}
int main()
{
    Polynomial p1, p2,r;
    int choice;
    float x;
    do
    {
        cout << "\n=====選單=====" << endl;
        cout << "1.輸入A:" << endl;
        cout << "2.輸入B:"<<endl; 
        cout << "3.顯示A和B" << endl;
        cout << "4.A+B"<< endl;
        cout << "5.A*B=" << endl;
        cout << "6.帶入x求A的值" << endl;
        cout << "7.帶入x求B的值" << endl;
        cout << "0.離開" << endl;
        cout << "請選擇:";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "1.輸入A" << endl;
            cin >> p1;
            break;
        case 2:
            cout << "2.輸入B" << endl;
            cin >> p2;
            break;
        case 3:
            cout << "A= " << p1 << endl;
            cout << "B=" << p2 << endl;
            break;
        case 4:
            r = p1.Add(p2);
            cout <<"A+B=" << r << endl;
            break;
        case 5:
            r = p1.Mult(p2);
            cout << "A*B=" << r << endl;
            break;
        case 6:
            cout << "x="; cin >> x;
            cout << "A(" << x << ")=" << p1.Eval(x) << endl;
            break;
        case 7:
            cout << "x="; cin >> x;
            cout << "B(" << x << ")=" << p2.Eval(x) << endl;
            break;
        case 0:
            cout << "結束程式" << endl;
            break;
        default:
            cout << "無效選項" << endl;
        }
    } while (choice!=0);
    return 0;
}
//1.解構子=>物件生命週期結束,系統自動呼叫,避免記憶體洩漏

//2.複製建構子=>應為我們這城市的termArry是動態配置如果只用預設複製,
//新的物件和舊的termArry會指向同一格記憶體,若其中一個被釋放,就會造成重複釋放
//先幫新物件配置一快全新且獨立的記憶體空間,在百舊物件裡面的實際內容複製
//這樣兩個物件各自擁有獨立的記憶體
```
## 3.效能分析(15%)<br>
設A有N項,B有M項<br>
|函式      |          時間複雜度        |額外空間    |
|---|---|---|
|newTerm   |均攤O(1)(擴充那次為O(terms))|O(1)均攤    |
|Add       |O(N+M)                     |O(N+M)     |
|Eval      |O(n)                       |O(1)       |
|operator<<|O(n)                       |O(1)       |
|operator>>|最佳$O(n)$,最差O$(n^2)$     |O(1)       |
|MulT      |最差$O(n^2*m)$|O(nm)|
## 說明<br>
newTerm:容量每次翻倍,放入k項總共約擴充 log₂k次,搬資料的總成本O(k),平均每項 O(1)<br>
Add:兩個多項式都已排序,雙指標各走一遍,每項只處理一次,結果最多n+m項<br>
operator>>:讀入O(n),插入排序在輸入完全反序時最差O(n^2),已排序時 O(n),合併與移除零項各O(n)<br>
Mult:外層n圈,每圈建立temp為O(m),再做一次 Add 與一次operator=,此時result最多有i·m項。總成本約m+ 2m + ... + nm,即 O(n^2*m)。若指數範圍小、乘積大量重複,結果項數受限,實際會比最差情況快<br>
### 4.測試與驗證(15%)<br>

|測試目的|A輸入|B輸入|預期結果|
|---|---|---|---|
|基本|2 1 1 1 0|2 1 1 1 0|A+B=2X+2:A*B=X^2+2+1|
|係數相消|2 1 1 1 0|2 -1 1 -1 0|A+B=0:A*B=-^2-2+1|
|亂序家重複的指數|4 1 0 3 2 2 1 5 2|1 1 0|A=8x^2+2x+1|
|零多項式|0|2 1 1 1 0|A=0:A+B=x+1:A*B=0|
|代入求值|3 3 2 2 1 1 0||x=2時,A(2)=17|
|小數|2 1.5 2 -0.5 0||x=2,A(2)=5.5:選3顯示1.5x^2-0.5|

### 5.效能量測(10%)<br>
一般輸入100項以內,輸出測量值都為0因為速度太快
### 6.心得討論(10%)<br>
其實一開始有很多是看不懂的,我是一個一個查然後加註解上去,包括複製建構子解構子這部份是我看不太懂得其中一個部分,複製建構子是因為動態陣列會把兩個物件就是A,B被複製出來的那兩個物件在同一個陣列,所以刪掉會造成重複釋放,所以要用複製建構子,friend是要可以讀那個勒別的保護資料,<br>
輸入和輸出串流我也看不太懂為甚麼要這樣寫,我查到的是
```cpp
ostream& operator<<(ostream& out, const Polynomial& p)
//                  ↑ 管子         ↑ 要印的多項式
```
cout << p1;,這裡的 out 就是 cout,p 就是 p1函式裡的 out << "x" 等同於 cout << "x",只是名字換成 out。operator<< 和 operator>> 需要知道「資料要往哪條管子送」,所以第一個參數是管子,最後再把管子還回去<br>
還有很多類別都是互相關聯的繞來繞去讓人看不懂,但我想最重要的地方還是資料怎麼儲存吧沒有這個前提其他的還是寫不出來,反而是主程式那邊變很簡潔
