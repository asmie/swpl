/**
 *  @file    Pipeline.hpp
 *  @brief  Single pipeline data model.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2022.06.15
 */

#ifndef SRC_PIPELINE_HPP_
#define SRC_PIPELINE_HPP_

#include "Global.h"

#include "Stage.hpp"

#include <deque>

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

	bool add_stage(Stage& stage);
	bool remove_stage(Stage& stage);


	bool start();

	/**
	* Stop running pipeline (closes all the streams).
	*/
	bool stop();

	/**
	* Pause the running pipeline.
	*/
	bool pause();

	/**
	* Resume paused pipeline.
	*/
	bool resume();

private:
	std::deque<Stage> stages_;

};


#endif /* SRC_PIPELINE_HPP_ */
