# 创建多个线程，数据共享问题分析，案例代码
## （1）创建和等待多个线程
* 通过代码实践可以发现多个线程的执行机制是乱的，跟操作系统内部对线程的运行调度机制有关
* 主线程用join等待所有子线程运行结束，最后主线程结束
```c++
#include "iostream"
#include "vector"
#include "thread"
using namespace std;

//线程入口函数
void myPrint(int num) {
    cout << "Tread Start, thread_id=" << num << endl;
    cout << "Thread End, thread id=" << num << endl;
    return;
}
int main() {
    vector<thread> myTreads;
    //创建10个线程
    for(int i=0;i<10;i++) {
        myTreads.push_back(thread(myPrint, i)); //创建并开始执行线
    }
    for(auto iter=myTreads.begin();iter!=myTreads.end();iter++) {
        iter->join();  //等待十个线程都返回
    }
    cout << "I Fuck America!" <<endl;
    return 0;
}
```

## （2）数据共享问题分析
最简单的不奔溃处理，读的时候不能写，写的时候不能读。  
只读数据案例：
```c++
//
// Created by Viets on 2021/6/29.
//
//
// Created by Viets on 2021/6/29.
//
#include "iostream"
#include "vector"
#include "thread"


using namespace std;


vector<int> g_v = {1,2,3};
//线程入口函数
void myPrint(int num) {
    cout << "id = " << std::this_thread::get_id() << "value is " << g_v[0]
    << g_v[1] << g_v[2] << endl;
    return;
}


int main() {
    vector<thread> myThreadVec;
    for(int i=0;i<10;i++) {
        myThreadVec.push_back(thread(myPrint, i));
    }
    for (auto iter = myThreadVec.begin(); iter !=myThreadVec.end(); ++iter) {
        iter->join();
    }
    cout << "I Fuck America" << endl;
    return 0;
}
```
其他案例：...

## (3)网络游戏服务器案例
网络游戏服务器，创建两个线程。一个线程收集玩家命令并写到一个队列中。另一个线程负责
从队列中取命令，解析，执行动作
```c++
#include "iostream"
#include "vector"
#include "thread"
#include "list"


using namespace std;

class A{
public:
    //把收到的消息放到一个队列
    void inmsgRevQueue() {
        for(int i=0;i<100000;i++) {
            cout << "in Message Receive Queue, Insert element." << endl;
            msgRevQueue.push_back(i);
        }
    }
    //把数据从消息队列中取出来
    void outMsgRevQueue() {
        for (int i = 0; i < 100000; ++i) {
            if (!msgRevQueue.empty()) {
                //消息不为空
                int command = msgRevQueue.front(); //返回第一个元素
                msgRevQueue.pop_front();  //移除元素，但是不返回
                //.....处理数据

            } else {
                cout << "Message Queue is Empty!" << i <<endl;
            }
        }
        cout << "End" <<endl;
    }
private:
    std::list<int> msgRevQueue; //用于装玩家发过来的命令的容器
};



int main() {
    A myTaskObj;
    std::thread myOutMsgObj(&A::outMsgRevQueue, &myTaskObj);  //第二个参数是引用,保障线程用的同一个对象
    std::thread myInMsgObj(&A::inmsgRevQueue, &myTaskObj);
    myOutMsgObj.join();
    myInMsgObj.join();

    cout << "I Fuck America" << endl;
    return 0;
}
```
上述程序存在问题：msgRevQueue属于共享数据，两个线程一个读一个写。当两个线程分别执行的时候
存在访问同一片内存的问题。此时在读写过程中需要加锁，否则会出现问题。比如：读写的过程加锁，
读写完之后解锁。共享数据不被破坏，程序就不会异常。  
解决方法：互斥量。





