#include <thread>
#include <map>
#include <string>
#include <any>
#include <vector>
#include <iostream>
class poolObj
{
public:
    int threadNum;
    ObjectStates state = INITIALIZED;
    std::map<std::string, std::any> out;
    virtual std::map<std::string, std::any> get()
    {
    }

private:
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
    std::map<int, std::thread *> pool;
    std::vector<poolObj *> _queue;
    std::vector<int> available;
    int _threadCount;
    Pool(int threadCount)
    {
        _threadCount = threadCount;
        for (int i = 0; i < _threadCount; ++i)
        {
            pool.insert({i, (std::thread *)nullptr});
            available.push_back(i);
        }
    }
    void stop()
    {
    }
    template <typename... Params>
    void queue(poolObj *obj, Params &...parameters)
    {
        obj->state = QUEUED;
        _queue.push_back(obj);
        if (available.size() > 0)
        {
            if (pool[available[0]]->joinable())
                pool[available[0]]->join();
            pool[available[0]] = &std::thread(_queue[_queue.size()]->run(), parameters);
        }
    }
};
//TODO: Async to run the next in queue
enum ObjectStates
{
    INITIALIZED,
    QUEUED,
    STARTED,
    STOPPED
};