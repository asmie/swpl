# swpl

![CMake](https://github.com/asmie/swpl/actions/workflows/cmake.yml/badge.svg)
![CodeQL](https://github.com/asmie/swpl/actions/workflows/codeql.yml/badge.svg)

SWitchboard PipeLines (swpl) is application dedicated to create pipelanes with stages to connect various inputs and outputs. One can create different stages where data from input is transformed using some operations and then put into the output.

## General Description

swpl is a tool that allow to connect different input/output components (lets call them IOs) and read/write to it some data. User is able to put between those IOs other components that are able to make certain operations on the data (transformations). The whole pipe between the IOs along with all transformations is called pipeline. It looks like:

IO_1 <--> Transform_1 <--> Transform_2 <--> Transform_n ... <--> IO_2

Each element is called stage - stage can be either input/output or transformation. Example:

File <--> match (pass if "Name") <--> mirror (names.csv) <--> File

Above pipeline is making a copy of file along with extracting lines with Name property and writing it in names.csv file.

Pipeline does not have to be linear. It's possible to change the datapath if some actions are true or false. Like:

File <--> match (pass1 if "Name", pass2 if "Password") <--> File_1 (names.csv)
                                                       <--> File_2 (pass.csv)
                                                       
The above example 

Supported IO types:
- files (type file);
- devices (type device);
- pipes (unix only) (type pipe);
- sockets (TCP and UDP) (type udp, type tcp);
- unix sockets (unix only) (type usock).

Supported transformations:
- mirror (mirroring data to another destination: any possible IO type);
- match (pass only data that are matching the pattern);
- patch (change data based on the pattern);


## Usage
### Command line interface

Swpl usage is as follows:
```
swpl -c <config_file> [-h] [-v] [-d] 

-c <config_file>        path to the configuration file
-h                      display this help
-v                      be more verbose
-d                      don't deamonize application
```

### Configuration file

Configuration file is INI-based file that can be set through the appropriate option in the command line. 


Every IO should recognize the below settings:
```
[section_name]
name = "io_name"
type = "type_name"

# Optional:
direction = input/output/bidirectional		# def: bidirectional
binary = true/false							          # def: true
read_chunk_min = 0							          # read chunks - only for bi/input
read_chunk_max = 128							        # read chunks - only for bi/input
write_chunk_min = 0							          # write chunks - onlu for bi/output
write_chunk_max = 128							        # write chunks - onlu for bi/output
```

Recognized type names are: file, device, pipe, tcp, udp, unix. Under the section name of the specified IO type there can be following settings:
```
[section_name]
name = "io_name"
type = "file or device"

path = "path"                             # path to the file or device
```

```
[section_name]
name = "io_name"
type = "pipe"

path_rx = "rx pipe"                        # path to the rx pipe
path_tx = "tx pipe"                        # path to the tx pipe
```

```
[section_name]
name = "io_name"
type = "tcp"

mode = "server/client"
address = "0.0.0.0"                         # address to connect to or to bind to
port = 234
```

Sections that defines transformations are not so standarized as every transform can demand different parameters.


### Examples

Example of RS to TCP converter (anyone who connect to the 9991 TCP port will gain access to ttyS1 device):

```
[io1]
name = "rs_port"
type = "device"
path = "/dev/ttyS1"

[io2]
name = "tcp_conn"
type = "tcp"
mode = "server"
address = "0.0.0.0"
port = 9991

[pipeline]
stage1 = io1
stage2 = io2
```

## Compilation

### Prerequisites

Application has no external dependencies. swpl demands:
* compiler compatible with GCC, Clang or MSVC, supporting at least C++17 standard;
* standard C++ library;
* cmake 3.16 or newer.

To properly build and run tests there is a need to have:
* gtest that can be found by cmake buildsystem.

### Building

Simply go into the source directory and the type:
```
mkdir build
cd build
cmake ..
```

This should generate all the build files and check if the compiler is appropriate and contain all needed headers, functions and other stuff. Cross-compilation and other actions can be done according to the cmake manual.

Afterwards, still beeing in the build directory just type:
```
make
```
which should produce binary itself.


## Bug reporting

Bugs can be reported using [GitHub issue tracker](https://github.com/asmie/swpl/issues).

## Further development
stamp-suite handles only one session at the time so it can be used to associate exactly one sender with exactly one reflector. This can be improved in the future as one reflector can handle many senders. Of course, it's simple when we're talking about the stateless mode as the only thing that reflector needs to do is copy original fields and generate own timestamp (in unauthorized mode). The things is going worse when we're talking about the stateful mode and multiple sessions, when some of them can be started and some of them can be terminated. As STAMP does not handles session management itself it needs to be done outside (somehow). Using 4-tuple params is of course problematic as configuring session-reflector for each 4-tuple can drive everybody crazy, using optional session identifiers described in the RFC 8972 seems to be the best way to handle state. 

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## Versioning

I use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/asmie/swpl). 

## Authors

* **Piotr Olszewski** - *Original work* - [asmie](https://github.com/asmie)

See also the list of [contributors](https://github.com/asmie/swpl/contributors) who participated in this project.


## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
