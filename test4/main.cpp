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
            std::lock_guard<std::mutex> sbguard(my_mutex);
//            my_mutex.lock();
            msgRevQueue.push_back(i);
//            my_mutex.unlock();
        }
    }

    bool outMsgLULProc(int &command) {

        //使用lock_guard()类模板
        std::lock_guard<std::mutex> sbguard(my_mutex);    //lock_guard()构造函数执行了mutex::lock()
                                                             //lock_guard()析构函数执行mutex::unlock()

//        my_mutex.lock();  //判断是否为空是读操作，所以需要加锁
        if (!msgRevQueue.empty()) {
            //消息不为空
            int command = msgRevQueue.front(); //返回第一个元素
            msgRevQueue.pop_front();  //移除元素，但是不返回
            //.....处理数据
//            my_mutex.unlock();
            return true;
        }
//        my_mutex.unlock();
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
    std::mutex my_mutex;  //创建一个互斥量
};

int main() {
    A myTaskObj;
    std::thread myOutMsgObj(&A::outMsgRevQueue, &myTaskObj);  //第二个参数是引用,保障线程用的同一个对象
    std::thread myInMsgObj(&A::inmsgRevQueue, &myTaskObj);
    myOutMsgObj.join();
    myInMsgObj.join();

    //保护共享数据


    return 0;
}



