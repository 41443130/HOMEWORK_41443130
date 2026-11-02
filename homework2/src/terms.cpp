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
        {
            clock_t start = clock();
            r = p1.Add(p2);
            clock_t end = clock();
            cout << "A+B=" << r << endl;
            cout << "Add 花了 " << (double)(end - start) * 1000000.0 / CLOCKS_PER_SEC << " us" << endl;//應為執行速度太快
            break;
        }
        case 5:
        {
            clock_t start1 = clock();
            r = p1.Mult(p2);
            clock_t end1 = clock();

            cout << "A*B=" << r << endl;
            cout << "Mult 花了 " << (double)(end1 - start1) * 1000000.0 / CLOCKS_PER_SEC << " us" << endl;//應為執行速度太快
            break;
        }
        case 6:
        {
            clock_t start2 = clock();
            cout << "x="; cin >> x;
            clock_t end2 = clock();
            cout << "A(" << x << ")=" << p1.Eval(x) << endl;
            cout << "Eval 花了 " << (double)(end2 - start2) * 1000000.0 / CLOCKS_PER_SEC << " us" << endl;//應為執行速度太快
            break;
        }
        case 7:
        {
            clock_t start3 = clock();
            cout << "x="; cin >> x;
            clock_t end3 = clock();
            cout << "B(" << x << ")=" << p2.Eval(x) << endl;
            cout << "Eval 花了 " << (double)(end3 - start3) * 1000000.0 / CLOCKS_PER_SEC << " us" << endl;//應為執行速度太快
            break;
        } 
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