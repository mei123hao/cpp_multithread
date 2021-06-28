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
        cout << " A(int a) construct function" << endl;
    }
    A(const A &a): m_i(a.m_i) {
        cout << "A(const A) copy construct function" << endl;
    }
    ~A() {
        cout << "~A() deconstruct function" << endl;
    }
};



//void myPrint(const int &i, char *pmyBuf) {
//    cout << i << endl;        //i实际并不是mvar的引用，实际是值传递。所以主线程用detach执行这个函数是安全的
//    cout << pmyBuf << endl;   //指针在detach子线程时一定会有问题，所以在创建子线程不可以用指针(此时pmyBuf指向同一片内存)
//    return;
//}

//void myPrint_2(const int &i, const string &pmyString) {
//    cout << i << endl;
//    cout << pmyString.c_str() << endl;   //此时&pmyString和&myBuf指向的内存地址是不一样的，所以主线程用detach是安全的
//    return;
//}

void myPrint3(const int &i, const A &pmyBuf) {
    cout << &pmyBuf << endl;
    return;
}


int main() {
    //1.传递临时对象作为线程参数
//    int mvar = 1;
//    int $myVar = mvar;
//    char myBuf[] = "this is a test";
//    thread myThreadObj(myPrint, mvar, myBuf);
////    myThreadObj.join();
//    myThreadObj.detach();
//    cout << "I Fuck America!" << endl;

//    int mvar = 1;
//    int &myVar = mvar;
//    char myBuf[] = "this is a test";
//    thread myThreadObj(myPrint_2, mvar, string(myBuf)); //直接将myBuf转换成string对象，确保稳定
//    myThreadObj.join();
//    cout << "I Fuck America!" << endl;

    int mvar = 1;
    int mySecondpar = 12;
    thread myThreadObj(myPrint3, mvar, A(mySecondpar));
//    myThreadObj.join();
    myThreadObj.detach();

//    cout << "I Fuck America!" << endl;

    return 0;
}


