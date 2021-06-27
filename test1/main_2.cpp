//
// Created by Viets on 2021/6/27.
//
//
// Created by Viets on 2021/6/27.
//
#include "iostream"
#include "vector"
#include "map"
#include "string"
#include "thread"

using namespace std;

class TA{
public:
    int &m_i;
    TA(int &i): m_i(i) {
        cout << "construct function..." << endl;
    }
    TA(const TA &ta): m_i(ta.m_i) {
        cout << "TA copy construct function..." << endl;
    }
    ~TA() {
        cout << "~TA() deconstruct function..." << endl;
    }
    void operator() () {  //将类对象变为一个可调用对象,注意：不可以带参数
//        cout << "operator start ...." << endl;
//        // ...
//        // ...
//        cout << "operater end ...." << endl;
        cout << "m_i 1 value: "<< m_i << endl;
        cout << "m_i 2 value: "<< m_i << endl;
        cout << "m_i 3 value: "<< m_i << endl;
        cout << "m_i 4 value: "<< m_i << endl;
        cout << "m_i 5 value: "<< m_i << endl;
        cout << "m_i 6 value: "<< m_i << endl;
    }
};


int main(){

    int i = 6;
    TA ta(i);
    thread myThreadObj(ta);
//    myThreadObj.detach();  //使用detach，主线程结束后，变量i内存被清除，子线程的还在打印引用变量，会产生无法预料的结果
    myThreadObj.join();
    cout << "I Love Fuck!" << endl;

    return 0;
}


