#include <sndX/TaskEngine.hpp>
#include <thread>

void soundEngineX::TaskEngine::run()
{
    while (!_player_thread.get_stop_token().stop_requested())
    {
        std::unique_lock lk(mutex);
        _cv.wait(lk, [this] { return _task_available || _player_thread.get_stop_token().stop_requested(); });

        while (!_tasks.empty())
        {
            auto task = std::move(_tasks.front());

            auto success = task.task();
            if (!success && task.reschedule_on_failure)
            {
                _timed_tasks[std::chrono::high_resolution_clock::now() + task.reschedule_delay] = std::move(task);
            }
            _tasks.pop_front();
            _task_available = false;
        }

        lk.unlock();
        _cv.notify_one();
    }
}

void soundEngineX::TaskEngine::stop()
{
    std::lock_guard lk(mutex);
    _tasks.clear();
    _timed_tasks.clear();
    _task_available = false;
}

void soundEngineX::TaskEngine::wait()
{
    // wait for all tasks to finish
    std::unique_lock lk(mutex);
    _cv.wait(lk, [this] { return _timed_tasks.empty() && _tasks.empty(); });
}

void soundEngineX::TaskEngine::checkTimedTasks()
{
    auto now = std::chrono::high_resolution_clock::now();
    checkTimedTasks(now);
}

void soundEngineX::TaskEngine::checkTimedTasks(const Task::TimePoint& time)
{
    // get all delayed task with deadline passed
    const std::lock_guard lk(mutex);
    auto                  iter = _timed_tasks.lower_bound(time);
    // emplace all tasks from the map to the queue
    for (auto it = _timed_tasks.begin(); it != iter; it++)
    {
        _tasks.emplace_back(std::move(it->second));
    }

    _timed_tasks.erase(_timed_tasks.begin(), iter);
    _task_available = true;
    _cv.notify_one();
}

soundEngineX::TaskEngine::TaskEngine(Config&& config)
  : _config(config)
{
    _player_thread = std::jthread{&TaskEngine::run, this};
    if (_config.periodic_check_duration != std::chrono::high_resolution_clock::duration::zero())
    {
        _check_thread = std::jthread([this](std::stop_token stoken) {
            while (!stoken.stop_requested())
            {
                checkTimedTasks();
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
            }
        });
    }
}

soundEngineX::TaskEngine::~TaskEngine()
{
    stop();
    _player_thread.request_stop();
    _cv.notify_all();
    _check_thread.request_stop();
    _cv.notify_all();
    _player_thread.join();
    _check_thread.join();
}

void soundEngineX::TaskEngine::addTask(Task&& task)
{
    auto            now = std::chrono::high_resolution_clock::now();
    std::lock_guard lk(mutex);
    if (task.starting_time_offset == std::chrono::high_resolution_clock::duration::zero())
    {
        _tasks.emplace_back(std::move(task));
        _task_available = true;
        _cv.notify_one();
    }
    else
    {
        //
        _timed_tasks[task.starting_time_offset + now] = std::move(task);
    }
}

void soundEngineX::TaskEngine::forceCheckTimedTasks()
{
    // get all delayed task with deadline passed
    const std::lock_guard lk(mutex);
    auto                  iter = _timed_tasks.lower_bound(Task::TimePoint::max());
    // emplace all tasks from the map to the queue
    for (auto it = _timed_tasks.begin(); it != iter; it++)
    {
        _tasks.emplace_back(std::move(it->second));
    }

    _timed_tasks.erase(_timed_tasks.begin(), iter);
    _task_available = true;
    _cv.notify_one();
}
