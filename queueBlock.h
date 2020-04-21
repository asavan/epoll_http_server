#ifndef __QUEUE_BLOCK_H__
#define __QUEUE_BLOCK_H__

#include "non_copyable.h"

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <iostream>
 
template <typename T>
class Queue : public Common::NonCopyable
{
 public:

	 Queue(int maxItemsCount) : maxItemsCount_(maxItemsCount)
	 {
	 }
	 ~Queue()
	 {

		 std::cout << "queue destory" << std::endl;
		 cond_.notify_all();
	 }
 
  T pop()
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
	auto item = std::move(queue_.front());
    queue_.pop();
    return item;
  }
 
  void pop(T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    item = queue_.front();
    queue_.pop();
  }
 
  void push(const T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(item);
    mlock.unlock();
    cond_.notify_one();
  }
 
  void push(T&& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(std::move(item));
    mlock.unlock();
    cond_.notify_one();
  }
 
 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
  int maxItemsCount_;
};
#endif // __QUEUE_BLOCK_H__
