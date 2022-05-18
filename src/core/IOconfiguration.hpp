/**
 *  @file   IOconfiguration.hpp
 *  @brief  Helper class for IO configuration.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2022.05.09
 *
 *  To follow single responsibility principle splitted configuration from the actual IO class hierarchy.
 *  Now, to create configuration helpers for any derived class there would be a need to derive from this class.
 *  The implementation and interface derivation here is trade-off for not splitting this simple class to any
 *  smaller elements to provide pure abstract interface and therefore, separate implmenetation.
 * 
 *  Separation allows also to change the configuration item names without touching the IO logic and therefore 
 *  it is a good move to keep it away the class engine.
 * 
 *  Moreover, thanks to splitting configuration from the IO class itself, there is possibility to use the 
 *  same configuration for different IOs like eg. file and device IOs can use the same configuration type as 
 *  the only difference between them can be "path" field (generally, obviously there can be some special devices 
 *  that would need something more like additional ioctl's etc etc.).
 */

#ifndef SRC_IOCONFIGURATION_HPP_
#define SRC_IOCONFIGURATION_HPP_

#include "Global.h"
#include "Configurable.hpp"

#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>

/**
* Stream direction - defines whether stream is only input stream (read), output (write) or
* bi-directional (read/write).
*/
enum class StreamDirection {
	INPUT = 0,
	OUTPUT,
	BIDIRECTIONAL
};

/**
* Abstract class with IO interface and some default implementation. Every input/output connector of the application must derive
* from this class.
*/
class IOconfiguration : public Configurable
{
public:
	/**
	* Default constructor of the class objects.
	*/
	IOconfiguration() = default;

	/**
	* Default destructor.
	*/
	virtual ~IOconfiguration() = default;

	/**
	* Method allowing module to configure itself using external configuration source.
	* Basic configuration for every module is done here.
	* Supported configuration:
	* [section_name]
	* name = "io_name"
	* type = "type_name"
	*
	* Optional configuration:
	* direction = input/output/bidirectional		# def: bidirectional
	* binary = true/false							# def: false
	* read_chunk_min = 0							# read chunks - only for bi/input
	* read_chunk_max = 0							# read chunks - only for bi/input
	* write_chunk_min = 0							# write chunks - onlu for bi/output
	* write_chunk_max = 0							# write chunks - onlu for bi/output
	* @param[in] config reference to the configuration manager facility
	* @param[in] section place where module configuration is stored
	* @return True if configuration is valid, otherwise false.
	*/
	virtual bool configure(ConfigurationManager& config, const std::string& section) override;

	/**
	* Check if two streams are the same.
	* @param[in] rhs IO object to be compared
	* @return True if streams have the same ID, otherwise false.
	*/
	virtual bool operator==(const IOconfiguration& rhs)
	{
		return (rhs.id_ == id_);
	}

	/**
	* Get IO object ID.
	* @return ID of the object.
	*/
	unsigned int getID() const
	{
		return id_;
	}

	/**
	* Get name of the IO object.
	* @return Name of the IO object.
	*/
	std::string getName() const
	{
		return name_;
	}

	/**
	* Get type of the IO object.
	* @return Type of the IO object.
	*/
	std::string getType() const
	{
		return type_;
	}

	/**
	* Get stream direction.
	* @return Stream direction.
	*/
	StreamDirection getDirection() const
	{
		return direction_;
	}


	/**
	* Get max reading chunk.
	* @return Maxiumum read chunk.
	*/
	size_t getReadChunkMax() const
	{
		return readChunkMax_;
	}

	/**
	* Get min reading chunk.
	* @return Minumum read chunk.
	*/
	size_t getReadChunkMin() const
	{
		return readChunkMin_;
	}

	/**
	* Get max writing chunk.
	* @return Maxiumum write chunk.
	*/
	size_t getWriteChunkMax() const
	{
		return writeChunkMax_;
	}

	/**
	* Get min writing chunk.
	* @return Minumum write chunk.
	*/
	size_t getWriteChunkMin() const
	{
		return writeChunkMin_;
	}

	/**
	* Get information if stream is binary or text based.
	* @return True if binary, otherwise false.
	*/
	bool getBinary() const
	{
		return binary_;
	}

private:
	static unsigned int last_id_;

	unsigned int id_{ 0 };											/*!< IO identification number */
	std::string type_{ "" };										/*!< Type */
	std::string name_{ "" };										/*!< Name of the IO object - used for user friendly printing */
	StreamDirection direction_{ StreamDirection::BIDIRECTIONAL };	/*!< Direction of the stream */
	bool binary_{ true };											/*!< Indicates if stream is binary */

	size_t readChunkMax_{ 0 };			/*!< Maximum chunk to read (unless specified explicit in function call) */
	size_t readChunkMin_{ 0 };			/*!< Minimum chunk to read (can block reading) */
	size_t writeChunkMax_{ 0 };			/*!< Maximum write to be done at one time */
	size_t writeChunkMin_{ 0 };			/*!< Minimum chunk to write (can delay writing till demanded amount of data) */
};

#endif /* SRC_IOCONFIGURATION_HPP_ */
