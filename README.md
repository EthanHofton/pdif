# PDiF

A PDF Diff tool that compares two PDFs, and reports the differences as a human-readable edit script.

## Requirements

- `CMake` build system, 3.25.2 or later
- `C++17` compiler
- `qpdf/qpdf` - A low level PDF library for C++. [GitHub](https://github.com/qpdf/qpdf.git)

QPDF is a requirement for this project. It must be installed to the system before building the project. The project will not build without it. This can be done either using Homebrew on macOS, or by building the library from source.

### macOS

To install QPDF on macOS, use Homebrew:

```bash
brew install qpdf
```

### Linux

To install QPDF on Linux, either use the package manager for your distribution or build and install the library from source:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_STATIC_LIBS=OFF
cmake --build build --parallel --target libqpdf
cmake --install build --component lib
cmake --install build --component dev
```

## Dependencies

The project auto imports the following dependencies (using `FetchContent`):
 
 - `EthanHofton/util` - A collection of utility functions and classes for C++. [GitHub](https://github.com/EthanHofton/util.git)
 - `nlohmann/json` - A JSON library for C++. [GitHub](https://github.com/nlohmann/json.git)

## Building

To buld the project, use cmake to build. The Cmake system has the following options:

 - `PDIF_BUILD_ENGINE_TESTS` - Build the tests for the engine. Default: `ON`
 - `PDIF_BUILD_CLI_TESTS` - Build the tests for the command line interface. Default: `ON` (temporarily `OFF`)
 - `PDIF_BUILD_CLI` - Build the command line interface. Default: `ON` (temporarily `OFF`)
 - `PDIF_BUILD_DOCS` - Build the documentation. Default: `OFF`

To build the project, use the following commands:

```bash
mkdir build
cd build
cmake .. [-D<OPTION>=<VALUE> ...]
cmake --build . --config [Debug|Release]
```

## Usage

Building the project creates two projects, the PDIF library, and the pdif-cli (if enabled). The library can be imported into other projects, to compare PDFs on the backend. The pdif-cli is a CLI implementation of the library, and can be used to compare PDFs from the command line.

### Library Usage

To include the library in your project, add the following to your CMakeLists.txt:

```cmake
find_package(pdif REQUIRED)
target_link_libraries(<your_target> pdif::pdif_engine)
```

Or you can use cmake's `FetchContent` to include the library in your project:

```cmake
include(FetchContent)
FetchContent_Declare(
  pdif
  GIT_REPOSITORY https://www.github.com/EthanHofton/pdif.git
)

# set the options for the library
set(PDIF_BUILD_CLI OFF)
set(PDIF_BUILD_DOCS OFF)
set(PDIF_BUILD_ENGINE_TESTS OFF)
set(PDIF_BUILD_CLI_TESTS OFF)

FetchContent_MakeAvailable(pdif)
```

#### Libary Docs

To get the documentation for the library, rebuild the project with the `PDIF_BUILD_DOCS` option set to `ON`. The documentation will be built in the `docs` directory of the build directory.

### Command Line Interface Usage

TODO: once the CLI is implemented, add usage instructions here.