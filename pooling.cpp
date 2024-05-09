#include <thread>
#include <map>
#include <string>
#include <any>
#include <vector>
#include <iostream>
#include <functional>
#include <utility>
#include <typeinfo>

enum ObjectStates
{
    INITIALIZED,
    QUEUED,
    STARTED,
    STOPPED
};

class PoolObj
{
public:
    int threadNum;
    ObjectStates state = ObjectStates::INITIALIZED;
    std::map<std::string, std::any> out;
    virtual std::map<std::string, std::any> get()
    {
    }

    template <typename... Params>
    void run(Params &...parameters)
    {
        std::cout << "Override me!" << std ::endl;
        return;
    }
};
class Pool
{
public:
    std::map<int, std::thread> pool;
    std::vector<std::pair<PoolObj, std::any>> _queue;
    std::vector<int> available;
    int _threadCount;
    Pool(int threadCount)
    {
        _threadCount = threadCount;
        for (int i = 0; i < _threadCount; ++i)
        {
            pool.insert({i, std::thread([](int n)
                                        { std::cout << "THREAD " << n << " INITIALIZED" << std::endl; },
                                        i)});
            available.push_back(i);
        }
    }
    void stop()
    {
        _queue.clear();
        std::cout << "QUITTING MAY TAKE TIME TO CLOSE ALL THREADS" << std::endl;
        for (int i = 0; i < _threadCount; ++i)
        {
            pool[i].join();
        }
    }
    template <typename... Params>
    void queue(PoolObj obj, Params &...parameters)
    {
        obj.state = QUEUED;
        _queue.push_back(std::pair < obj, std::forward<Params>(parameters));
        if (available.size() > 0)
        {
            if (pool[available[0]].joinable())
                pool[available[0]].join();
            std::function<void()> run = [this]()
            { _queue[0].first.run<decltype(_queue[_queue.size()].second)>(_queue[_queue.size()].second); };
            pool[available[0]] = std::thread([&]
                                             { wrap(run, available[0]) });
            available.erase(available.begin());
        }
    }
    void update(int thread)
    {
        if (pool[thread].joinable())
        {
            pool[thread].join();
            if (_queue.size() > 0)
            {
                std::function<void()> run = [this]()
                { _queue[0].first.run<decltype(_queue[0].second)>(_queue[0].second); };
                _queue.erase(_queue.begin());
                pool[thread] = std::thread([&]
                                           { wrap(run, thread); });
            }
            else
            {
                available.push_back(thread);
            }
        }
    }

private:
    void wrap(std::function<void()> func, int thread)
    {
        func();
        update(thread);
    }
};
// TODO: Async to run the next in queue
