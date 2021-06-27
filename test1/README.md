# 1.并发，进程，线程基本概念
*************************
## 并发概念：
两个或者更多的任务（独立的任务）同时进行：一个程序同时执行多个独立的任务  
单核cpu计算机：某一个时刻只能处理一个任务，由操作系统调度，每秒钟进行多次任务切换  
多核cpu计算机：能够实现真正的并行处理

## 进程概念：
进程：运行起来的可执行程序会形成一个进程
## 线程概念：
线程：
* 每一个进程都会有一个主线程，这个线程是惟一的，也就是一个进程中只能有一个主线程
* 只要产生了进程，主线程就会随着线程默默启动起来
* 主线程运行完毕，进程结束
* 主线程由代码自己创建，我们可以通过代码创建其他线程
* 线程并不是越多越好，每一个线程都需要有一个独立的堆栈空间(1M)，线程之间切换需要保存很多中间状态，且切换需要耗费时间

# 2.并发的实现方法
*********************************
概念：两个或多个任务同时发生  
实现并发手段：
* 通过多个进程并发
* 在单独进程中，创建多个线程实现并发

多进程并发：  

多线程并发：  
* 一个进程中的所有线程共享同一个地址空间（共享内存）。
* 全局变量，指针，引用都可以在线程之间传递。使用多线程开销远远小于多进程
* 共享内存带来的问题：数据一致性问题。eg:线程A和线程B 往同一片内存写数据

# 3.创建多线程的方法
***************************
## 范例演示线程运行的开始和结束
### thread:  
标准库里面的类  
### join():  
加入/汇合，说白了就是阻塞，让主线程等待子线程执行完毕，然后主线程与子线程汇合。然后主线程继续往下走  
### detach():  
传统多线程：主线程等待子线程执行完毕，然后主线程再推出；detach可以打破这一传统规律。
一旦detach()之后，主线程与子线程失去关联，此时子线程就会驻留在后台运行。此时这个子线程被C++运行时库接管。
执行完毕后，由运行时库负责清理该线程资源。使用detach()之后不可以再使用join()。  
### joinable():  
判断是否可以成功使用join()或者detach()，返回true或者false  
```c++
#include "iostream"
#include "vector"
#include "map"
#include "string"
#include "thread"

using namespace std;

void myPrint() {
    cout << "Start My Thread.." <<endl;
    cout << "Pussy 1" << endl;
    cout << "Pussy 2" << endl;
    cout << "Pussy 3" << endl;
    cout << "Pussy 4" << endl;
    cout << "Pussy 5" << endl;
    cout << "Pussy 6" << endl;
    cout << "Pussy 7" << endl;
    cout << "Pussy 8" << endl;
    cout << "Pussy 9" << endl;
    cout << "Pussy 10" << endl;
    cout << "Pussy 11" << endl;
    cout << "Pussy 12" << endl;
    cout << "End My Thread.." << endl;
}


int main(){

    cout << "Main Thread Start" << endl;  //主线程

    cout << "Fuck 1" << endl;
    cout << "Fuck 2" << endl;
    cout << "Fuck 3" << endl;
    cout << "Fuck 4" << endl;

    //主线程执行完毕，子线程也会被强行终止
    thread myThreadObj(myPrint);
    if (myThreadObj.joinable()) {
        cout << "joinable == true " << endl;
        myThreadObj.join();
    } else {
        cout << "joinable == false " << endl;
    }

    if (myThreadObj.joinable()) {
        cout << "joinable == true " << endl;
    } else {
        cout << "joinable == false " << endl;
    }

    cout << "Main Thread Over" << endl;

    return 0;
}
```
## 其他创建多线程的方法
### (1)用类以及一个问题范例
问题：  
主线程调用detach()之后，当主线程执行结束时，主线程中创建的类对象还在吗？  
答：对象肯定不在，但是对象被复制到子线程中去了。所以执行完主线程后，类对象被销毁，但是
被复制的类对象依旧存在。所以，只要对象中没有引用，指针等，一般都不会产生问题
```c++
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
```
### (2)用lamda表达式创建线程
```c++
#include "iostream"
#include "vector"
#include "map"
#include "string"
#include "thread"

using namespace std;


int main(){

    auto lamdaThread = [] {
        cout << "Fuck Thread Start..." << endl;
        //..
        cout << "Fuck Thread 1" << endl;
        cout << "Fuck Thread 2" << endl;
        cout << "Fuck Thread End..." << endl;
    };

    thread myThreadObj(lamdaThread);
    myThreadObj.join();
//    myThreadObj.detach();
    cout << "I fuck JJ" << endl;

    return 0;
}
```


