//
// Created by Viets on 2021/7/2.
//
#include "iostream"
#include "vector"
#include "thread"
#include "list"
#include "mutex"

using namespace std;

class A{
public:
    //把收到的消息放到一个队列
    void inmsgRevQueue() {
        for(int i=0;i<100000;i++) {
            cout << "in Message Receive Queue, Insert element." << endl;

            std::lock(my_mutex_1, my_mutex_2);

            std::lock_guard<std::mutex> sbguard_1(my_mutex_1, std::adopt_lock);  //用adopt_lock参数可以不用unlock
            std::lock_guard<std::mutex> sbguard_2(my_mutex_2, std::adopt_lock);
//            my_mutex_1.lock();  //实际工程这两个锁头中间可能执行很多代码
            //......其他代码
//            my_mutex_2.lock();
            msgRevQueue.push_back(i);
//            my_mutex_2.unlock();
//            my_mutex_1.unlock();
        }
    }

    bool outMsgLULProc(int &command) {
        //使用lock_guard()类模板

        //lock_guard()析构函数执行mutex::unlock()


//        my_mutex_1.lock();  //判断是否为空是读操作，所以需要加锁
//        my_mutex_2.lock();
        std::lock(my_mutex_1, my_mutex_2);
        std::lock_guard<std::mutex> sbguard_1(my_mutex_1, std::adopt_lock);    //lock_guard()构造函数执行了mutex::lock()
        std::lock_guard<std::mutex> sbguard_2(my_mutex_2, std::adopt_lock);

        if (!msgRevQueue.empty()) {
            //消息不为空
            int command = msgRevQueue.front(); //返回第一个元素
            msgRevQueue.pop_front();  //移除元素，但是不返回
            //.....处理数据

//            my_mutex_2.unlock();
//            my_mutex_1.unlock();
            return true;
        }

//        my_mutex_2.unlock();
//        my_mutex_1.unlock();
        return false;
    }

    //把数据从消息队列中取出来
    void outMsgRevQueue() {
        int command = 0;

        for (int i = 0; i < 100000; ++i) {
            bool result = outMsgLULProc(command);

            if (result == true) {
                cout << "outMsgRevQueue() perform, take out a data" << command << endl;
                //可以考虑进行命令的处理
            } else {
                cout << "Message Queue is Empty!" << i <<endl;
            }
        }
        cout << "End" <<endl;
    }
private:
    std::list<int> msgRevQueue; //用于装玩家发过来的命令的容器
    std::mutex my_mutex_1;  //创建一个互斥量
    std::mutex my_mutex_2;
};

int main() {
    A myTaskObj;
    std::thread myOutMsgObj(&A::outMsgRevQueue, &myTaskObj);  //第二个参数是引用,保障线程用的同一个对象
    std::thread myInMsgObj(&A::inmsgRevQueue, &myTaskObj);
    myOutMsgObj.join();
    myInMsgObj.join();
    return 0;
}



