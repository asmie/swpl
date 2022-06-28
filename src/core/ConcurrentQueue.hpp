/**
 *  @file   ConcurrentQueue.hpp
 *  @brief  ConcurrentQueue implementation.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2022.06.23
 */

#include <thread>
#include <atomic>
#include <mutex>

/**
* Queue node.
*/
template<class T>
class QueueNode {
public:
	QueueNode(T data) : data(data), next(nullptr) {}
	T data;
	QueueNode<T>* next;
};

/**
* ConcurrentQueue with minimal blocking. There has been used non-blocking mechanisms where it was possible.
* This is the first implemenetation - further optimization is needed to avoid blocking in some places - as it's 
* possible to write it using atomics and counting references only.
* 
* Queue is trying to be compilant with standard STL queue. Due to be concurrent-safe front() and back() members does not
* return references (as in STL version) but pointers to data. This prevent races when user check if queue is empty and then
* calls front()/back() - between empty() and front()/back() some other threads can pop() queue and make it empty. It can lead
* to undefined behaviour - returning null reference. Returning pointer is always safe as there is possibility to return nullptr.
* 
* Currently ConcurrentQueue is intended to solve one consumer with multiple producers problem as this is the most common usecase
* for the current application.
* 
* TODO: remove to_remove_mutex_ to avoid blocking.
* TODO2: change memory order from seq_cst to more loose one.
* 
*/
template<class T, class Allocator = std::allocator<T> >
class ConcurrentQueue {
public:
	ConcurrentQueue() { }
	
	/**
	* Destructor of the ConcurrentQueue. Deletes all allocated nodes.
	*/
	~ConcurrentQueue() { 
		while (head_ != nullptr) {
			QueueNode<T>* to_remove = head_;
			head_ = head_->next;
			delete to_remove;
		}
	}

	/**
	* Check if queue is empty.
	* @return True if queue is empty.
	*/
	[[nodiscard]] bool empty() const noexcept {
		return size_.load(std::memory_order::seq_cst) == 0;
	}
	
	/**
	* Get size of the queue.
	* @return Size of the queue.
	*/
	size_t size() const noexcept {
		return size_.load(std::memory_order::seq_cst);
	}

	/**
	* Return front queue element. 
	* @return Pointer to front queue element.
	*/
	T* front() noexcept {
		return head_.load()->data;
	}

	/**
	* Return front queue element.
	* @return Constant pointer to front queue element.
	*/
	const T* front() const noexcept {
		return head_.load()->data;
	}

	/**
	* Return last queue element.
	* @return Pointer to last queue element.
	*/
	T* back() noexcept {
		return tail_.load()->data;
	}

	/**
	* Return last queue element.
	* @return Constant pointer to last queue element.
	*/
	const T* back() const noexcept {
		return tail_.load()->data;
	}

	/**
	* Push element to the queue.
	* @param value element to push.
	*/
	void push(const T& value) {
		std::scoped_lock lock(to_remove_mutex_);								// TODO: remove to avoid locking
		QueueNode<T>* new_node = new QueueNode<T>(value);
		auto old_tail = tail_.load(std::memory_order::seq_cst);
		if (old_tail != nullptr)
			old_tail->next = new_node;
		else
			head_.store(new_node, std::memory_order::seq_cst);
		tail_.store(new_node, std::memory_order::seq_cst);
		size_.fetch_add(1);
		
		//while (!tail_.compare_exchange_weak(new_node->next, new_node, std::memory_order_release, std::memory_order_relaxed)) {
		//	std::this_thread::yield();
		//}
	}
	
	/**
	* Push element to the queue.
	* @param value element to push.
	*/
	void push(T&& value) {
		std::scoped_lock lock(to_remove_mutex_);							// TODO: remove to avoid locking
		QueueNode<T>* new_node = new QueueNode<T>(std::move(value));
		auto old_tail = tail_.load(std::memory_order::seq_cst);
		if (old_tail != nullptr)
			old_tail->next = new_node;
		else
			head_.store(new_node, std::memory_order::seq_cst);
		tail_.store(new_node, std::memory_order::seq_cst);
		size_.fetch_add(1);
	}

	/**
	* Pop front element from the queue.
	*/
	void pop() {
		std::scoped_lock lock(to_remove_mutex_);							// TODO: remove to avoid locking
		auto old_head = head_.load(std::memory_order::seq_cst);
		auto old_tail = tail_.load(std::memory_order::seq_cst);
		
		if (old_head == old_tail && old_head != nullptr) {
			delete old_head;
			head_.store(nullptr, std::memory_order::seq_cst);
			tail_.store(nullptr, std::memory_order::seq_cst);
			size_.store(0);
		}
		else
		{
			head_.store(old_head->next, std::memory_order::seq_cst);
			delete old_head;
			size_.fetch_sub(1);
		}	
	}

	/**
	* Swap elements from the other queue with the current one.
	* @param other queue to swap with.
	*/
	void swap(ConcurrentQueue& other) noexcept {

	}


	ConcurrentQueue& operator=(const ConcurrentQueue& other) {
		
	}

	ConcurrentQueue& operator=(ConcurrentQueue&& other) noexcept {
		
	}

private:
	std::atomic<QueueNode<T>*> head_;					/*!< Queue head */
	std::atomic<QueueNode<T>*> tail_;					/*!< Queue tail */
	std::atomic<size_t> size_;							/*!< Queue size */

	std::mutex to_remove_mutex_;						/*!< Mutex to be removed when queue will be fully lock-free */
};