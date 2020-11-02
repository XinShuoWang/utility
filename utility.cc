#ifndef TAIR_CONTEST_KV_CONTEST_NVM_UTILITY_H_
#define TAIR_CONTEST_KV_CONTEST_NVM_UTILITY_H_

#define XXH_INLINE_ALL
#include "include/xxhash.h"

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>
#include <queue>
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cassert>
#include <atomic>

#include <emmintrin.h>
#include <pthread.h>

/********************************* FOR LOCK FUNCTION * ****************************************/

class Mutex {
  private:
    std::mutex m;
  public:
    inline void Lock(){m.lock();}
    inline void Unlock() {m.unlock();}
};

class SpinLock {
private:
  pthread_spinlock_t lock_;

public:
  inline SpinLock() { pthread_spin_init(&lock_, PTHREAD_PROCESS_PRIVATE); }

  inline ~SpinLock() { pthread_spin_destroy(&lock_); }

  inline void Lock() { pthread_spin_lock(&lock_); }

  inline void Unlock() { pthread_spin_unlock(&lock_); }
};

class SpinLockGuard {
private:
  SpinLock *lock_;

public:
  inline SpinLockGuard(SpinLock *lock) {
    lock_ = lock;
    lock_->Lock();
  }

  inline ~SpinLockGuard() { lock_->Unlock(); }
};

class ReadWriteLock {
private:
  pthread_rwlock_t lock_;

public:
  inline ReadWriteLock() { pthread_rwlock_init(&lock_, NULL); }
  inline ~ReadWriteLock() { pthread_rwlock_destroy(&lock_); }
  inline void ReaderLock() { pthread_rwlock_rdlock(&lock_); }
  inline void ReaderUnLock() { pthread_rwlock_unlock(&lock_); }
  inline void WriterLock() { pthread_rwlock_wrlock(&lock_); }
  inline void WriterUnLock() { pthread_rwlock_unlock(&lock_); }
  ReadWriteLock(const ReadWriteLock &src) = delete;
  ReadWriteLock &operator=(const ReadWriteLock &rhs) = delete;
};

class ReadLockGuard {
private:
  ReadWriteLock *lock_;

public:
  inline ReadLockGuard(ReadWriteLock *lock) {
    lock_ = lock;
    lock_->ReaderLock();
  }

  inline ~ReadLockGuard() { lock_->ReaderUnLock(); }
};

class WriteLockGuard {
private:
  ReadWriteLock *lock_;

public:
  inline WriteLockGuard(ReadWriteLock *lock) {
    lock_ = lock;
    lock_->WriterLock();
  }

  inline ~WriteLockGuard() { lock_->WriterUnLock(); }
};

/********************************* FOR MEM ALLOCATE * ****************************************/

class Arena{
 private:
  char *p_;
  char *start_;
  char *end_;
  uint64_t block_size_;
  SpinLock lock_;
  
 public:
  Arena(uint64_t block_size){
    block_size_ = block_size;
    start_ = new char[block_size_];
    memset(start_, 0, block_size_);
    end_ = start_ + block_size_;
    p_ = start_;
  }

  ~Arena() = default;
  
  char* Allocate(uint64_t size){
    SpinLockGuard lg(&lock_);
    if(uint64_t(end_ - p_) < size){
      start_ = new char[block_size_];
      memset(start_, 0, block_size_);
      end_ = start_ + block_size_;
      p_ = start_;
    }
    char* res = p_;
    p_ += size;
    return res;
  }
};

/********************************* FOR HASH FUNCTION * ****************************************/

class Hash {
private:
  inline static int DecodeFixed32(const char *ptr, uint32_t &result){
		const uint8_t *const buffer = reinterpret_cast<const uint8_t *>(ptr);
		// Recent clang and gcc optimize this to a single mov / ldr instruction.
		result = (static_cast<uint32_t>(buffer[0])) |
						(static_cast<uint32_t>(buffer[1]) << 8) |
						(static_cast<uint32_t>(buffer[2]) << 16) |
						(static_cast<uint32_t>(buffer[3]) << 24);
		return 0;
	}

public:
  inline static uint32_t Murmur(const char *data, size_t n, uint32_t seed){
		// Similar to murmur hash
		const uint32_t m = 0xc6a4a793;
		const uint32_t r = 24;
		const char *limit = data + n;
		uint32_t h = seed ^ (n * m);
		// Pick up four bytes at a time
		while (data + 4 <= limit) {
			uint32_t w;
			DecodeFixed32(data, w);
			data += 4;
			h += w;
			h *= m;
			h ^= (h >> 16);
		}
		// Pick up remaining bytes
		if (limit - data == 3) {
			h += static_cast<uint8_t>(data[2]) << 16;
			h += static_cast<uint8_t>(data[1]) << 8;
			h += static_cast<uint8_t>(data[0]);
			h *= m;
			h ^= (h >> r);
		} else if (limit - data == 2) {
			h += static_cast<uint8_t>(data[1]) << 8;
			h += static_cast<uint8_t>(data[0]);
			h *= m;
			h ^= (h >> r);
		} else if (limit - data == 1) {
			h += static_cast<uint8_t>(data[0]);
			h *= m;
			h ^= (h >> r);
		}
		return h;
	}
  inline static uint32_t XXHash(const char *data, size_t n, uint32_t seed){
		return XXH32(data, n, seed);
	}

  inline static uint32_t XXHash(const char *data, size_t n){
		return XXH32(data, n, 9);
	}

  inline static uint32_t FastHash(const char* data, uint32_t n){
    return (*reinterpret_cast<uint32_t*>(const_cast<char*>(data)));
  }
};

/********************************* FOR LOG FUNCTION * ****************************************/

enum LogLevel { kNONE, kINFO, kDEBUG };

class Logger {
private:
  FILE *log_file_;
  time_t time_start_;
  LogLevel log_level_;

  inline void Add(LogLevel log_level, std::string log){
		if (log_file_ != nullptr && log_level <= log_level_) {
			time_t time_now = time(NULL);
			fprintf(log_file_, "Time: %ld, Message: %s\n", time_now - time_start_,
							log.c_str());
			fflush(log_file_);
		}
	}

public:
  inline explicit Logger(LogLevel log_level, FILE *file){
		log_level_ = log_level;
		log_file_ = file;
		time_start_ = time(NULL);
	}
  inline ~Logger(){
		if (log_file_ != nullptr) {
			fflush(log_file_);
			fclose(log_file_);
		}
	}
  // template log
  template <typename... Args>
  inline void Add(LogLevel log_level, const std::string &format, Args... args){
		size_t size = 1 + snprintf(nullptr, 0, format.c_str(), args...);
		char bytes[size];
		snprintf(bytes, size, format.c_str(), args...);
		this->Add(log_level, std::string(bytes));
	}
};

/********************************* FOR THREAD POOL FUNCTION * ****************************************/

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

#endif
