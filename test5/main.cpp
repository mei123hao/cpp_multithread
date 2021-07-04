//
// Created by Viets on 2021/7/4.
//
#include <iostream>
#include "vector"
#include "thread"
#include "list"
#include "mutex"

using namespace std;

class A{
public:
    std::unique_lock<std::mutex> rtn_unique_lock() {
        std::unique_lock<std::mutex> tmpguard(my_mutex_1);
        return tmpguard;  //系统生成临时unique_lock对象，并调用unique_lock的移动构造函数
    }


    //把收到的消息放到一个队列
    void inmsgRevQueue() {
        for(int i=0;i<100000;i++) {
            cout << "in Message Receive Queue, Insert element." << endl;

            std::unique_lock<std::mutex> sbguard(my_mutex_1);
            std::mutex *ptr = sbguard.release();  //要解锁这个mutex，返回原始mutex指针，指针接管了mutex的地址，mutex本身地址被释放
            msgRevQueue.push_back(i);
            ptr->unlock();   //对mutex指针解锁
        }
    }

    bool outMsgLULProc(int &command) {
        //使用lock_guard()类模板
        std::unique_lock<std::mutex> sbguard_1(my_mutex_1);

//        std::chrono::milliseconds dura(20000);
//        std::this_thread::sleep_for(dura);  //休息一定的时长

        if (!msgRevQueue.empty()) {
            //消息不为空
            command = msgRevQueue.front(); //返回第一个元素
            msgRevQueue.pop_front();  //移除元素，但是不返回
            //.....处理数据
            return true;
        }
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
};

int main() {
    A myTaskObj;
    std::thread myOutMsgObj(&A::outMsgRevQueue, &myTaskObj);  //第二个参数是引用,保障线程用的同一个对象
    std::thread myInMsgObj(&A::inmsgRevQueue, &myTaskObj);
    myOutMsgObj.join();
    myInMsgObj.join();
    return 0;
}




