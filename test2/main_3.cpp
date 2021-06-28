//
// Created by Viets on 2021/6/28.
//
#include "iostream"
#include "vector"
#include "thread"

using namespace std;

class A {
public:
    mutable int m_i;
    A(int i): m_i(i) {
        cout << " A(int a) construct function " << this <<" thread_id = " << std::this_thread::get_id() << endl;
    }
    A(const A &a): m_i(a.m_i) {
        cout << "A(const A) copy construct function " << this <<" thread_id = " << std::this_thread::get_id() << endl;
    }
    ~A() {
        cout << "~A() deconstruct function " << this <<" thread_id = " << std::this_thread::get_id() << endl;
    }
};


void myPrint2(const A &pmyBuf) {
    pmyBuf.m_i = 199;  //我们修改这个值不会影响main函数
    cout << "sub_thread_var_addr:" << &pmyBuf << " threadId=" << std::this_thread::get_id() << endl;
    return;
}


int main() {
    cout << "main thread id = " << std::this_thread::get_id() << endl;
    A myAobj(10);
    std::thread myThreadObj(myPrint2, std::ref(myAobj)); //将类对象作为线程参数
    myThreadObj.join();

    return 0;
}


