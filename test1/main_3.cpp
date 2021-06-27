//
// Created by Viets on 2021/6/27.
//
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
//    myThreadObj.join();
    myThreadObj.detach();
    cout << "I fuck JJ" << endl;

    return 0;
}

