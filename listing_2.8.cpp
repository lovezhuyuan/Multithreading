#include <thread>
#include <numeric>
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>

template<typename Iterator,typename T>
struct accumulate_block
{
    void operator()(Iterator first,Iterator last,T& result)
    {
        result=std::accumulate(first,last,result);
    }
};

template<typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init)
{
    unsigned long const length=std::distance(first,last);

    if(!length)
        return init;

    unsigned long const min_per_thread=25;//线程的最小任务数
    unsigned long const max_threads=(length+min_per_thread-1)/min_per_thread;//需要的最大线程数 应该大于1

    unsigned long const hardware_threads=std::thread::hardware_concurrency();//硬件线程数

    unsigned long const num_threads= std::min(hardware_threads!=0?hardware_threads:2,max_threads);

    unsigned long const block_size=length/num_threads;//块大小  每个线程需要计算的长度

    std::vector<T> results(num_threads); //存放 result 的vector
    std::vector<std::thread>  threads(num_threads-1); //如果只需一个线程  就不创建 线程了

    Iterator block_start=first;
    for(unsigned long i=0;i<(num_threads-1);++i)
    {
        Iterator block_end=block_start;
        std::advance(block_end,block_size);
        threads[i]=std::thread(
            accumulate_block<Iterator,T>(),
            block_start,block_end,std::ref(results[i]));
        block_start=block_end;
    }
    accumulate_block<Iterator,T>()(block_start,last,results[num_threads-1]); //计算了剩余的
    
    std::for_each(threads.begin(),threads.end(),
        std::mem_fn(&std::thread::join));
    //线程结束后  将result 的vector 计算一遍
    return std::accumulate(results.begin(),results.end(),0);
}

int main()
{
    std::vector<int> vi;
    int resultCheck = 0;
    for(int i=0;i<100;++i)
    {
        vi.push_back(i);
        resultCheck+=i;
    }
    int sum=parallel_accumulate(vi.begin(),vi.end(),0);
    std::cout<<"sum="<<sum<<std::endl;
    std::cout<<"resultCheck="<<resultCheck<<std::endl;
}
