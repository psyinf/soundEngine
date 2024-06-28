#include <sndX/BackgroundPlayer.hpp>
#include <sndX/Loader.hpp>
#include <sndX/SoundEngine.hpp>
#include <sndX/Source.hpp>

#include <chrono>
#include <exception>
#include <iostream>
#include <thread>
#include <queue>
#include <map>

// a task to be executed by the tasked player, should be non-blocking and as fast as possible
struct Task
{
    using Duration = std::chrono::high_resolution_clock::duration;
    using TimePoint = std::chrono::high_resolution_clock::time_point;

    std::function<bool()> task; // the task needs to return true if it was successful, false otherwise.
    bool                  reschedule_on_failure = false; // if the task fails, should it be rescheduled?
    TimePoint             starting_time{};               // delay before executing the task
    Duration              reschedule_delay = std::chrono::milliseconds(0); // delay before executing the task
};

class TaskedPlayer
{
public:
    TaskedPlayer()
    {
        std::jthread t(&TaskedPlayer::run, this);
        t.detach();
        // regularly check for timed tasks
        std::jthread t2([this] {
            while (_running)
            {
                checkTimedTasks();
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
            }
        });
        t2.detach();
    }

    ~TaskedPlayer() {}

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

    void addTask(Task&& task)
    {
        auto            now = std::chrono::high_resolution_clock::now();
        std::lock_guard lk(mutex);
        if (task.starting_time < now)
        {
            _tasks.emplace_back(std::move(task));
            _task_available = true;
            cv.notify_one();
        }
        else { _timed_tasks[task.starting_time] = std::move(task); }
    }

    void checkTimedTasks()
    {
        auto now = std::chrono::high_resolution_clock::now();
        // get all delayed task with deadline passed
        const std::lock_guard lk(mutex);
        auto                  iter = _timed_tasks.lower_bound(now);
        // emplace all tasks from the map to the queue
        for (auto it = _timed_tasks.begin(); it != iter; it++)
        {
            _tasks.emplace_back(std::move(it->second));
        }

        _timed_tasks.erase(_timed_tasks.begin(), iter);
        _task_available = true;
        cv.notify_one();
    }

    void wait()
    {
        std::unique_lock lk(mutex);
        cv.wait(lk, [this] { return _tasks.empty() && _timed_tasks.empty(); });
    }

    void stop()
    {
        _running = false;
        std::lock_guard lk(mutex);
        _tasks.clear();
        _timed_tasks.clear();
        _task_available = false;
    }

private:
    void run()
    {
        while (_running)
        {
            std::unique_lock lk(mutex);
            cv.wait(lk, [this] { return _task_available; });

            while (!_tasks.empty())
            {
                auto task = std::move(_tasks.front());
                _tasks.pop_front();
                auto success = task.task();
                if (!success && task.reschedule_on_failure)
                {
                    _timed_tasks[std::chrono::high_resolution_clock::now() + task.reschedule_delay] = std::move(task);
                }

                _task_available = false;
            }

            lk.unlock();
            cv.notify_one();
        }
    }

    std::mutex mutex;
    // condition variable to signal the player that a new task has been added
    std::condition_variable         cv;
    bool                            _task_available = false;
    std::atomic_bool                _running = true;
    std::deque<Task>                _tasks;
    std::map<Task::TimePoint, Task> _timed_tasks;
};

void taskedPlayerTest()
{
    TaskedPlayer player;
    using namespace std::chrono_literals;
    auto now = std::chrono::high_resolution_clock::now();
    int  state = 0;
    std::cout << "Starting tasks\n";
    // current time in seconds
    auto t =
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    std::cout << t << ": starting\n";
    player.addTask({.task =
                        [&state]() {
                            state++;
                            // current time in seconds
                            auto t = std::chrono::duration_cast<std::chrono::milliseconds>(
                                         std::chrono::high_resolution_clock::now().time_since_epoch())
                                         .count();
                            std::cout << t << ": delayed run#" << state << "\n ";
                            return state > 3;
                        },
                    .reschedule_on_failure = true,
                    .starting_time = now + 2s,
                    .reschedule_delay = 1s}

    );
    player.wait();
    player.stop();
}

void taskedPlayerSoundTest()
{
    TaskedPlayer player;
    auto         src1 = std::make_shared<soundEngineX::Source>(soundEngineX::loader::load("data/demo/click.wav"));
    auto         src2 = std::make_shared<soundEngineX::Source>(soundEngineX::loader::load("data/demo/test.wav"));

    player.addTask({[&src1]() {
        src1->start();
        return "";
    }});
    player.addTask([]() { std::this_thread::sleep_for(std::chrono::seconds(3)); });

    player.addTask([&src2]() { src2->start(); });
    player.addTask([&src1]() { src1->start(); });
    player.addTask([]() { std::this_thread::sleep_for(std::chrono::seconds(3)); });
    player.wait();
    player.stop();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
try
{
    // initialize a SoundEngine instance
    soundEngineX::SoundEngine engine;
    taskedPlayerSoundTest();
    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
}
catch (...)
{
    std::cerr << "Unknown exception" << std::endl;
}
