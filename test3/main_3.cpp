//
// Created by Viets on 2021/6/29.
//
//
// Created by Viets on 2021/6/29.
//
//
// Created by Viets on 2021/6/29.
//
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








