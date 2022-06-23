
#include "Mirror.hpp"

#include <algorithm>
#include <execution>
#include <chrono>
#include <thread>
#include <condition_variable>

void MirrorTransformation::run()
{
	while (get_work_flag())
	{
		std::condition_variable cv;
		
		std::for_each(std::execution::par_unseq, std::begin(incoming_data_), std::end(incoming_data_), [](const auto& q) {
			if (q.second.queue.size() > 0)
			{
				
			}
			});

	}

}