# unique_lock()详解
## (1)unique_lock()取代lock_guard()
unique_lock()是个类模板，一般用lock_guard()  
回顾：lock_guard()取代了mutex的lock()和unlock();  
unique_lock()比lock_guard()灵活很多，但是占用内存较大
## (2)unique_lock()的第二个参数
### std::adopt；
表示互斥量已经被lock了的标志位。效果就是调用方线程已经拥有了互斥的所有权（已经lock()成功了）
通知lock_guard不需要在构造函数中对互斥量进行lock()
### std::try_lock
尝试用mutex的lock去锁这个Mutex,如果没有lock成功也会立即返回
```c++
class A{
public:
    //把收到的消息放到一个队列
    void inmsgRevQueue() {
        for(int i=0;i<100000;i++) {
            cout << "in Message Receive Queue, Insert element." << endl;
            std::unique_lock<std::mutex> sbguard_1(my_mutex_1, std::try_to_lock);  //用adopt_lock参数可以不用unlock
            //......其他代码
            if (sbguard_1.owns_lock()) {
                //拿到了锁头
                msgRevQueue.push_back(i);
            } else {
                cout << "waiting" << endl;
            }
        }
    }

    bool outMsgLULProc(int &command) {
        //使用lock_guard()类模板
        std::unique_lock<std::mutex> sbguard_1(my_mutex_1);

        std::chrono::milliseconds dura(20000);
        std::this_thread::sleep_for(dura);  //休息一定的时长

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
```
### std::defer_lock
用defer_lock前提，mutex不可以先lock。意思就是初始一个没有加锁的mutex。目的是为了方便
调用unique_lock的成员函数

## （3）unique_lock的成员函数
### lock()
```c++
class A{
public:
    //把收到的消息放到一个队列
    void inmsgRevQueue() {
        for(int i=0;i<100000;i++) {
            cout << "in Message Receive Queue, Insert element." << endl;
            std::unique_lock<std::mutex> sbguard(my_mutex_1, std::defer_lock); //此时my_mutex_1没有加锁
            sbguard.lock();  //不用unlock()
            msgRevQueue.push_back(i);

        }
    }

    bool outMsgLULProc(int &command) {
        //使用lock_guard()类模板
        std::unique_lock<std::mutex> sbguard_1(my_mutex_1);
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
```
### unlock()
可以方便把锁暂时解开，处理一些非共享代码，然后再锁上
```c++
class A{
public:
    //把收到的消息放到一个队列
    void inmsgRevQueue() {
        for(int i=0;i<100000;i++) {
            cout << "in Message Receive Queue, Insert element." << endl;

            std::unique_lock<std::mutex> sbguard(my_mutex_1, std::defer_lock); //此时my_mutex_1没有加锁
            sbguard.lock();  //不用unlock()

            //...处理共享代码
            sbguard.unlock();
            //...处理非共享代码
            sbguard.lock();

            msgRevQueue.push_back(i);
            sbguard.unlock();  //没必要，但是不会出错
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
```
### try_lock()
尝试给互斥量加锁，如果拿不到锁，返回false。如果拿到了锁，返回true。这个函数是不阻塞的
```c++
class A{
public:
    //把收到的消息放到一个队列
    void inmsgRevQueue() {
        for(int i=0;i<100000;i++) {
            cout << "in Message Receive Queue, Insert element." << endl;

            std::unique_lock<std::mutex> sbguard(my_mutex_1, std::defer_lock); //此时my_mutex_1没有加锁
            if (sbguard.try_lock()) {
                msgRevQueue.push_back(i);
            } else {
                cout << "my_mutex_1 do not lock, do others." << endl;
            }
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
```
 
### release()
返回其所管理的mutex指针，并释放所有权，也就是此时unique_lock和release不再有关系  
严格区分unlock()和release()的区别  
如果原来mutex对象处于加锁状态，有必要接管过来并进行解锁
```c++
class A{
public:
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
```

## (4)unique_lock()所有权的传递
```c++
std::unique_lock<std::mutex> sbguard(my_mutex_1); 
//所有权概念
//sbguard拥有my_mutex_1的所有权
//sbguard可以把自己的所有权转移给其他的unique_lock对象
//所以 unique_lock对象这个mutex所有权属于可以转移但是不可复制
//所以要转移所有权要这样写,
// 方法1：用上std::move()
std::unique_lock<std::mutex> sbguard_copy(std::move(sbguard)); 
//现在sbguard_copy接管了my_mutex_1，此时sbguard指向空
//方法2：返回局部的unique_lock()
std::unique_lock<std::mutex> rtn_unique_lock() {
    std::unique_lock<std::mutex> tmpguard(my_mutex1);
    return tmpguard;
}
```

