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
* Queue is trying to be compilant with standard STL queue.
* 
* Currently ConcurrentQueue is intended to solve one consumer with multiple producers problem as this is the most common usecase
* for the current application.
* 
*/
template<class T, class Allocator = std::allocator<T> >
class ConcurrentQueue {
public:
	ConcurrentQueue() { }
	~ConcurrentQueue() { }

	[[nodiscard]] bool empty() const noexcept {
		return size_.load(std::memory_order::seq_cst) == 0;
	}
	
	size_t size() const noexcept {
		return size_.load(std::memory_order::seq_cst);
	}

	T& front() {
		return head_.load()->data;		// As in STL queue - front is first element and caller is responsible not to call front() on empty queue.
	}
	
	const T& front() const {
		return head_.load()->data;		// As in STL queue - front is first element and caller is responsible not to call front() on empty queue.
	}

	T& back() {
		return tail_.load()->data;
	}

	const T& back() const {
		return tail_.load()->data;
	}

	void push(const T& value) {
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
	
	void push(T&& value) {
		QueueNode<T>* new_node = new QueueNode<T>(std::move(value));
		auto old_tail = tail_.load(std::memory_order::seq_cst);
		if (old_tail != nullptr)
			old_tail->next = new_node;
		else
			head_.store(new_node, std::memory_order::seq_cst);
		tail_.store(new_node, std::memory_order::seq_cst);
		size_.fetch_add(1);
	}

	void pop() {
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

	void swap(ConcurrentQueue& other) noexcept {

	}


	ConcurrentQueue& operator=(const ConcurrentQueue& other) {
		
	}

	ConcurrentQueue& operator=(ConcurrentQueue&& other) noexcept {
		
	}

private:
	std::atomic<QueueNode<T>*> head_;
	std::atomic<QueueNode<T>*> tail_;
	std::atomic<size_t> size_;
};