/**
 *  @file    Global.h
 *  @brief  Global include that should be used by every file in the project.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2019.04.12
 */

#ifndef SWPL_SRC_GLOBAL_H_
#define SWPL_SRC_GLOBAL_H_

// Include generated configuration file.
#include "swpl_config.h"

// Include debug facility.
#include "debug.h"



#if defined(_MSC_VER)
#include <basetsd.h>
typedef SSIZE_T ssize_t;
#endif

#endif /* SWPL_SRC_GLOBAL_H_ */
