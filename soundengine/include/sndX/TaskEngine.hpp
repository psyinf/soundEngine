#pragma once
#include <chrono>
#include <condition_variable>
#include <deque>
#include <functional>
#include <map>
#include <mutex>
#include <thread>

namespace soundEngineX {
using namespace std::chrono_literals;

// a task to be executed by the tasked player, should be non-blocking and as fast as possible
struct Task
{
    using Duration = std::chrono::high_resolution_clock::duration;
    using TimePoint = std::chrono::high_resolution_clock::time_point;

    std::function<bool()> task; //< the task needs to return true if it was successful, false otherwise.
    bool                  reschedule_on_failure = false; //< if the task fails, should it be rescheduled?
    Duration              starting_time_offset{0ms};     //< delay before executing the task
    Duration              reschedule_delay{0ms};         //< delay before executing the task
};

class TaskEngine
{
public:
    struct Config
    {
        using Duration = std::chrono::high_resolution_clock::duration;

        // periodically check for timed tasks. If set to 0, no periodic check will be performed and timed tasks will
        // only be handled by manually calling checkTimedTasks()
        Duration periodic_check_duration{16ms};
    };

    TaskEngine(Config&& cfg = {.periodic_check_duration{16ms}});
    ~TaskEngine();

    // add a generic callable as a Task
    template <typename F>
    void addTask(F&& f)
    {
        // if lambda's return type is void, we wrap it in a lambda that returns bool for convenience
        constexpr bool is_void = std::is_same_v<decltype(f()), void>;
        if constexpr (is_void)
        {
            addTask({[f]() {
                f();
                return true;
            }});
        }
        // is convertible to bool
        else if constexpr (std::is_convertible_v<decltype(f()), bool>)
        {
            addTask({[f]() { return f(); }});
        }
        else { static_assert(is_void, "Task must return bool or void"); }
    }

    void addTask(Task&& task);
    // check for task's deadline and move them to the task queue
    void checkTimedTasks();
    // forced run of checkTimedTasks
    void forceCheckTimedTasks();

    // wait for all tasks to finish
    void wait();
    // stop the player
    void stop();

private:
    void checkTimedTasks(const Task::TimePoint& time);
    void run();

    std::mutex                      mutex;
    std::condition_variable         _cv;                     //< used to notify the player that a new task is available
    bool                            _task_available = false; //< flag to signal that a new task is available
    std::deque<Task>                _tasks;                  //< tasks to be executed
    std::map<Task::TimePoint, Task> _timed_tasks;            //< tasks to be executed at a specific time
    Config                          _config{};
    std::jthread                    _player_thread;
    std::jthread                    _check_thread;
};

} // namespace soundEngineX