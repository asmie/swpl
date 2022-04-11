# swpl

![CMake](https://github.com/asmie/swpl/actions/workflows/cmake.yml/badge.svg)
![CodeQL](https://github.com/asmie/swpl/actions/workflows/codeql.yml/badge.svg)

SWitchboard PipeLines (swpl) is application dedicated to create pipelanes with stages to connect various inputs and outputs. One can create different stages where data from input is transformed using some operations and then put into the output.

## General Description

## Compilation

### Prerequisites

Application has no external dependencies. swpl demands:
* compiler compatible with GCC, Clang or MSVC, supporting at least C++14 standard;
* standard C++ library;
* cmake 3.14 or newer.

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

## Usage
### Command line interface

Swpl usage is as follows:
```

```

### Configuration file

Configuration file is INI-based file that can be set through the appropriate option in the command line. 

Sample configuration file:
```
```

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
