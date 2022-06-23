/**
 *  @file   Stage.hpp
 *  @brief  Stage abstraction used to represent the single step of the pipeline.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2020.11.12
 */

#ifndef SRC_CORE_STAGE_HPP_
#define SRC_CORE_STAGE_HPP_

#include "IO.hpp"

#include <atomic>
#include <queue>
#include <map>

struct DataQueue
{
	std::queue<std::vector<uint8_t>> queue;
	std::unique_ptr<std::mutex> mutex;
};

/**
* Base class for all stages that can be derived in the system.
* Provides basic mechanisms for registering senders as well as 
* interface for adding new messages.
*/
class Stage
{
public:
	Stage() = default;
	virtual ~Stage() = default;
	
	/**
	* Add message to the incoming queue of the stage with the specified sender ID or
	* leave that ID alone for those stages that have only one sender or does not care
	* who the sender is (0 then is default queue that stage which does not matter should
	* create during construction).
	* @param[in] data data to be added to the queue
	* @param[in] id optional id of the sender
	* @return True if added was successful, otherwise false (queue is full).
	*/
	virtual bool add_to_queue(std::vector<uint8_t> data, unsigned int id = 0) {
		bool added = false;
		
		if (incoming_data_.contains(id))
		{
			std::scoped_lock lock{ *incoming_data_[id].mutex.get() };
			incoming_data_[id].queue.push(std::move(data));
			added = true;
		}
		return added;
	}

	/**
	* Register new data cooperative stage with the specified ID.
	* @param[in] id id of the new sender
	* @param[in] sender pointer to the sender.
	*/
	virtual void register_coop(unsigned int id, Stage *sender) {
		std::scoped_lock lock{ configuration_mutex_ };

		incoming_data_.insert_or_assign(id, std::move(DataQueue{}));
		incoming_data_[id].mutex = std::make_unique<std::mutex>();
		outgoing_data_.insert_or_assign(id, sender);
	}

	virtual void unregister_coop(unsigned int id) {
		std::scoped_lock lock{ configuration_mutex_ };

		if (incoming_data_.contains(id))
		{
			std::scoped_lock rlock{ *incoming_data_[id].mutex.get() };		// Make sure that nothing is doing any operation
			incoming_data_.erase(id);
		}

		if (outgoing_data_.contains(id))
			outgoing_data_.erase(id);
	}

	bool get_work_flag() const {
		return work_flag_.load();
	}

	void set_work_flag(bool work_flag) {
		work_flag_.store(work_flag);
	}

protected:
	// Queues are protected for performance reason to give direct access.
	std::map<unsigned int, DataQueue> incoming_data_;					/*!< vector of the queues containing incoming data */
	std::map<unsigned int, Stage*> outgoing_data_;					/*!< vector of pointer to put outgoing data */

private:	
	unsigned int id{0};
	std::atomic<bool> work_flag_;
	std::mutex configuration_mutex_;
};

/**
* Derived class for stage that is intended to be IO stage.
* IO stage can have single IO and single queues for input and 
* output data. This determines that IO stages can be only
* leaves in pipelines (terminates the pipeline).
*/
class IOStage : public Stage
{
public:
	IOStage() { }

	
protected:
	std::unique_ptr<IO> io;												/*!< Base IO used for input / output or both */ 
};

/**
* Transformation stage is derived from the Stage class and handles
* operations done directly on the data.
*/
class TransformStage : public Stage
{
public:

	/**
	* Runs the current stage.
	*/
	virtual void run() = 0;

protected:

};


#endif /* SRC_CORE_STAGE_HPP_ */
