//
// Created by Viets on 2021/6/29.
//
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






