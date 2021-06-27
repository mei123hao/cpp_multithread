//
// Created by Viets on 2021/6/27.
//
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

