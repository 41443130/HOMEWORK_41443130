#include <iostream>
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