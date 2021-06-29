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







