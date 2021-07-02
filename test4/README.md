# 互斥量概念，用法，死锁演示及解决详解
## （1）互斥量(mutex)的基本概念
保护共享数据操作：操作时，某个线程用代码把共享数据锁住，操作数据，解锁。其他想操作共享数据的线程必须解锁。
互斥量其实就是一个类对象，可以把互斥量理解成一把锁，多个线程尝试用lock()成员函数来加锁，只有一个线程能够锁定成功
如果没有锁成功，那么流程就卡在lock()里面不断尝试去锁这把锁  
互斥量的使用：保护的数据少了达不到保护效果，保护的多了，影响代码效率


## （2）互斥量的用法
### lock()和unlock()
操作：先lock()，操作共享数据，unlock()  
注意：lock()和unlock()要成对使用  
```c++
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

            my_mutex.lock();
            msgRevQueue.push_back(i);
            my_mutex.unlock();
        }
    }

    bool outMsgLULProc(int &command) {
        my_mutex.lock();  //判断是否为空是读操作，所以需要加锁
        if (!msgRevQueue.empty()) {
            //消息不为空
            int command = msgRevQueue.front(); //返回第一个元素
            msgRevQueue.pop_front();  //移除元素，但是不返回
            //.....处理数据
            my_mutex.unlock();
            return true;
        }
        my_mutex.unlock();
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
```


### std::lock_guard()
为了防止忘记unlock()，可以使用std::lock_guard()。类似于智能指针，忘记了释放内存不要紧
智能指针可以帮忙释放
注意：使用lock_guard()就再也不能使用lock()和unlock()
```c++
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
    return 0;
}
```

## （3）死锁
### 死锁演示
例子：张三站在北京等李四，不挪窝
     李四站在深圳等张三，不挪窝
C++中比如我有两把锁（死锁这个问题，是由至少两个锁头也就是两个互斥量才可以产生）
eg:两个所金锁和银锁，两个线程A和B。  
（1）线程A执行的时候，这个线程先去锁金锁，把金锁lock成功，然后去锁银锁
（2）线程B执行的时候，这个线程先去锁银锁，把银锁lock成功，然后去锁金锁
（3）线程A因为拿不到银锁头，所以流程走不下去（金锁头解不开）
（4）线程B因为拿不到金锁头，所以流程也走不下去（银锁头也解不开）
死锁程序演示：
```c++
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
//            std::lock_guard<std::mutex> sbguard(my_mutex);
            my_mutex_1.lock();  //实际工程这两个锁头中间可能执行很多代码
            //......其他代码
            my_mutex_2.lock();
            msgRevQueue.push_back(i);
            my_mutex_2.unlock();
            my_mutex_1.unlock();
        }
    }

    bool outMsgLULProc(int &command) {
        //使用lock_guard()类模板
        std::lock_guard<std::mutex> sbguard(my_mutex_1);    //lock_guard()构造函数执行了mutex::lock()
        //lock_guard()析构函数执行mutex::unlock()

        my_mutex_2.lock();  //错误顺序，要与my_mutex_1顺序交换
        my_mutex_1.lock();  //判断是否为空是读操作，所以需要加锁
        if (!msgRevQueue.empty()) {
            //消息不为空
            int command = msgRevQueue.front(); //返回第一个元素
            msgRevQueue.pop_front();  //移除元素，但是不返回
            //.....处理数据
            my_mutex_1.unlock();
            my_mutex_2.unlock();
            return true;
        }
        my_mutex_1.unlock();
        my_mutex_2.unlock();
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
```
### 死锁解决方法
只要保证两个互斥量上锁的顺序一致就不会造成死锁
```c++
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
            std::lock_guard<std::mutex> sbguard_1(my_mutex_1);
            std::lock_guard<std::mutex> sbguard_2(my_mutex_2);
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
        std::lock_guard<std::mutex> sbguard_1(my_mutex_1);    //lock_guard()构造函数执行了mutex::lock()
        std::lock_guard<std::mutex> sbguard_2(my_mutex_2);
        //lock_guard()析构函数执行mutex::unlock()


//        my_mutex_1.lock();  //判断是否为空是读操作，所以需要加锁
//        my_mutex_2.lock();
        if (!msgRevQueue.empty()) {
            //消息不为空
            int command = msgRevQueue.front(); //返回第一个元素
            msgRevQueue.pop_front();  //移除元素，但是不返回
            //.....处理数据

//            my_mutex_2.unlock();
//            my_mutex_1.unlock();
            return true;
        }

        my_mutex_2.unlock();
        my_mutex_1.unlock();
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
```


### std::lock()函数模板
一次锁住两个或者两个以上的互斥量(至少两个，多了不限，一个不行)，一般用在处理多个互斥量
std::lock() 如果互斥量中有一个没有锁住，他就在那里等着，等待所有互斥量都锁住，才
继续往下走（返回）  
存在的情况（以两个为例）：（1）两个互斥量都锁住 （2）两个互斥量都没有锁住
（3）两个互斥量如果一个锁成功，另外一个没有锁成功，那么它立即把已经锁住的解锁


### std::lock_guard()的std::adopt_lock参数
std::adopt是一个结构体对象，表示互斥量已经lock了，可以不用再调用进行结构体里面方法
进行lock了
```c++
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
```

