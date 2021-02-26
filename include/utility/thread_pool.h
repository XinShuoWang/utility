//
// Created by wangxinshuo on 2021/2/26.
//

#ifndef UTILITY_INCLUDE_UTILITY_THREAD_POOL_H_
#define UTILITY_INCLUDE_UTILITY_THREAD_POOL_H_

#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <thread>

/**
 *  Simple ThreadPool that creates `threadCount` threads upon its creation,
 *  and pulls from a queue to get new jobs.
 *
 *  This class requires a number of c++11 features be present in your compiler.
 */
class ThreadPool {
 private:
  std::vector<std::thread> threads_;
  std::queue<std::function<void()>> queue_;
  int jobs_left_;
  bool is_running_;
  std::condition_variable jobs_queued_condition_;
  std::condition_variable jobs_done_condition_;
  std::mutex jobs_left_mutex_;
  std::mutex queue_mutex_;

 public:
  inline explicit ThreadPool(int threadCount) : jobs_left_(0), is_running_(true) {
    threads_.reserve(threadCount);
    for (int index = 0; index < threadCount; ++index) {
      threads_.emplace_back([&] {
        /**
         *  Take the next job in the queue and run it.
         *  Notify the main thread that a job has completed.
         */
        do {
          std::function<void()> job;
          // scoped lock
          {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            // Wait for a job if we don't have any.
            jobs_queued_condition_.wait(lock, [&] { return !queue_.empty(); });
            // Get job from the queue
            job = queue_.front();
            queue_.pop();
          }
          job();
          // scoped lock
          {
            std::lock_guard<std::mutex> lock(jobs_left_mutex_);
            --jobs_left_;
          }
          jobs_done_condition_.notify_one();
        } while (is_running_);
      });
    }
  }

  /**
   *  JoinAll on deconstruction
   */
  inline ~ThreadPool() { JoinAll(); }

  /**
   *  Add a new job to the pool. If there are no jobs in the queue,
   *  a thread is woken up to take the job. If all threads are busy,
   *  the job is added to the end of the queue.
   */
  inline void AddJob(const std::function<void()> &job) {
    // scoped lock
    {
      std::lock_guard<std::mutex> lock(queue_mutex_);
      queue_.push(job);
    }
    // scoped lock
    {
      std::lock_guard<std::mutex> lock(jobs_left_mutex_);
      ++jobs_left_;
    }
    jobs_queued_condition_.notify_one();
  }

  /**
   *  Join with all threads. Block until all threads have completed.
   *  The queue may be filled after this call, but the threads will
   *  be done. After invoking `ThreadPool::JoinAll`, the pool can no
   *  longer be used.
   */
  inline void JoinAll() {
    if (is_running_) {
      is_running_ = false;
      // add empty jobs to wake up threads
      const int threadCount = threads_.size();
      for (int index = 0; index < threadCount; ++index) {
        AddJob([] {});
      }
      // note that we're done, and wake up any thread that's
      // waiting for a new job
      jobs_queued_condition_.notify_all();
      for (std::thread &thread : threads_) {
        if (thread.joinable()) {
          thread.join();
        }
      }
    }
  }

  /**
   *  Wait for the pool to empty before continuing.
   *  This does not call `std::thread::join`, it only waits until
   *  all jobs have finished executing.
   */
  inline void WaitAll() {
    std::unique_lock<std::mutex> lock(jobs_left_mutex_);
    jobs_done_condition_.wait(lock, [&] { return jobs_left_ == 0; });
  }

  /**
   *  Get the vector of threads themselves, in order to set the
   *  affinity, or anything else you might want to do
   */
  inline std::vector<std::thread> &GetThreads() { return threads_; }

  /**
   *  Process the next job in the queue to run it in the calling thread
   */
  inline bool ExecuteNextJob() {
    std::function<void()> job;
    // scoped lock
    {
      std::lock_guard<std::mutex> lock(queue_mutex_);
      if (queue_.empty()) {
        return false;
      }
      // Get job from the queue
      job = queue_.front();
      queue_.pop();
    }
    job();
    // scoped lock
    {
      std::lock_guard<std::mutex> lock(jobs_left_mutex_);
      --jobs_left_;
    }
    jobs_done_condition_.notify_one();
    return true;
  }
};

#endif //UTILITY_INCLUDE_UTILITY_THREAD_POOL_H_
