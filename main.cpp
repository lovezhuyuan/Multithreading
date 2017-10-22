//
//  main.cpp
//  Multithreading
//
//  Created by lovezhuyuan on 2017/10/21.
//  Copyright © 2017年 lovezhuyuan. All rights reserved.
//

#include <iostream>
#include <thread>
class thread_guard{
private:
    
    std::thread & t;
public:
    explicit thread_guard(std::thread &t_):t(t_){
        
    }
    ~thread_guard(){
        if(t.joinable()){
            t.join();
        }
    }
    thread_guard(thread_guard const&) = delete;
    thread_guard& operator = (thread_guard const&) = delete;
};
void hello(int &i){
    i++;
    std::cout<<i<<std::endl;
}
struct func{
    int & i;
    func(int & i_):i(i_){}
    void operator()(){
            hello(i);
    }
};
int main(int argc, const char * argv[]) {
    int a =1;
//    auto fuc = func(a);
//    std::thread t(fuc);
//    thread_guard g(t);
    while(true){
        std::thread t(hello,std::ref(a));
        
        std::thread t2 = std::move(t);
        throw std::logic_error("No thread");
        t2.detach();
    }
    
    
    return 0;
}
