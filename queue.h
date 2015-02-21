#ifndef __COMMON_QUEUE_H__
#define __COMMON_QUEUE_H__

#include "exceptions.h"
#include <mutex>

#include <queue>
#include <memory>
#include <iostream>

namespace Network
{

  DECLARE_RUNTIME_EXCEPTION(Queue)
  
  template <typename T, typename TSyn = std::mutex>
  class Queue : private Common::NonCopyable
  {
  public:
    typedef std::unique_ptr<T> ItemPtr;
    typedef std::queue<ItemPtr> ItemPool;
    
    Queue(typename ItemPool::size_type maxItemsCount)
      : MaxItemsCount(maxItemsCount)
    {
    }

	~Queue()
	 {
		 std::cout << "queue destory" << std::endl;
	 }
    void push(std::unique_ptr<T> item)
    {
      LockGuard Lock(Guard);
      if (Items.size() >= MaxItemsCount)
        throw QueueException("Queue is full");
      Items.push(std::move(item));
    }
    ItemPtr pop()
    {   
		LockGuard Lock(Guard);	
       
        if (!Items.empty())
        {
          ItemPtr Item = std::move(Items.front());
          Items.pop();
		  return Item;
        }
		return ItemPtr();
      
      // return std::move(Item);
    }
    
  private:
    typedef std::lock_guard<TSyn> LockGuard;
    mutable TSyn Guard;
    typename ItemPool::size_type MaxItemsCount;
    ItemPool Items;
  };
  
}

#endif  // !__COMMON_QUEUE_H__
