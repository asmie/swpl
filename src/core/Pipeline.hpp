/**
 *  @file    Pipeline.hpp
 *  @brief  Single pipeline data model.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2019.04.12
 */

#ifndef SRC_PIPELINE_HPP_
#define SRC_PIPELINE_HPP_

#include "Global.h"

namespace swpl {

class Pipeline
{
public:
	Pipeline();
	~Pipeline();

	int start();
	int stop();
	int pause();
	int resume();

private:


};


} /* namespace swpl */

#endif /* SRC_PIPELINE_HPP_ */
