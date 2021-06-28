//
// Created by Viets on 2021/6/28.
//
#include "iostream"
#include "vector"
#include "thread"

using namespace std;

class A {
public:
    int m_i;
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


void myPrint(const A &pmyBuf) {
    cout << "sub_thread_var_addr:" << &pmyBuf << " threadId=" << std::this_thread::get_id() << endl;
    return;
}


int main() {
    cout << "main thread id = " << std::this_thread::get_id() << endl;
    int mvar = 1;
    std::thread myThreadObj(myPrint, A(mvar));
//    myThreadObj.detach();
    myThreadObj.join();

    return 0;
}


