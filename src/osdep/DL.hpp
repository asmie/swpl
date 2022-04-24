/**
 *  @file   DL.hpp
 *  @brief  Dynamic loader interface.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2021.03.04
 */

#ifndef SRC_OSDEP_DL_H_
#define SRC_OSDEP_DL_H_

/**
 * Structure with flags that can be used by dynamic linker.
 */
typedef struct {
	bool lazy;
} DL_FLAGS;

/**
 * Class representing dynamic loader general interface. Interface is os and compiler agnostic allowing other
 * modules to use it the same way.
 */
class DL
{
public:
	/**
	 * Loads library with given name.
	 * @param[in] name name of the library file
	 * @return Handle to loaded library or nullptr if library was not loaded due to an error.
	*/
	static void* loadLibrary(const char* name)
	{
		DL_FLAGS defFlags{false};
		return (DL::loadLibrary(name, defFlags));
	}

	/**
	 * Loads library with given name.
	 * @param[in] name name of the library file
	 * @param[in] flags flags used to open library
	 * @return Handle to loaded library or nullptr if library was not loaded due to an error.
	 */
	static void* loadLibrary(const char* name, DL_FLAGS flags);

	/**
	 * Gets function address from the library.
	 * @param[in] libraryHandle handle to the library returned by loadLibrary function
	 * @param[in] functionName name of the function
	 * @return Pointer to the function address or nullptr if function was not found or there was any other error.
	 */
	static void* getFuncAddr(void* libraryHandle, const char *functionName);

};

#endif /* SRC_OSDEP_DL_H_ */
