# Code Counter

This is a simple C++ command-line tool to count lines of code in a given directory. It can distinguish between total lines, blank lines, comment lines, and code lines for various programming languages.

## Features

- Counts total lines, blank lines, comment lines, and code lines.
- Supports C++, Markdown, and Text files.
- Provides a per-file report or a summary report for the entire directory.
- Easily extendable to support more languages.

## Getting Started

### Prerequisites

- C++17 compiler
- CMake 3.10 or higher

### Installation

1. Clone the repository:
   ```sh
   git clone <repository-url>
   ```
2. Navigate to the project directory:
   ```sh
   cd code_counter
   ```

## Building

1. Create a build directory:
   ```sh
   mkdir build && cd build
   ```
2. Run CMake to configure the project:
   ```sh
   cmake ..
   ```
3. Build the project:
   ```sh
   cmake --build .
   ```

## Usage

To run the code counter, execute the following command from the `build` directory:

```sh
./code_counter <directory_path> [--per-file]
```

- `<directory_path>`: The path to the directory you want to analyze.
- `--per-file`: (Optional) A flag to display a detailed report for each file.

## Running Tests

The project includes a suite of unit tests to verify the functionality of the `FileParser` class. The tests are written using the Google Test framework and are located in the `tests` directory.

### Test Coverage

The current tests cover the following scenarios:

- Verifying that the `FileParser` correctly identifies C++ source and header files (`.cpp`, `.h`).
- Verifying that the `FileParser` correctly identifies Markdown files (`.md`).
- Verifying that the `FileParser` correctly identifies text files (`.txt`).

### Running the Tests

To run the unit tests, execute the following command from the `build` directory:

```sh
ctest
```

**Note for Windows Users (MinGW):**

If you are using MinGW on Windows, you might encounter an error where `ctest` cannot find the necessary runtime DLLs. If this happens, you can temporarily add the MinGW `bin` directory to your `PATH` for the current PowerShell session before running `ctest`:

```powershell
# Add MinGW bin to the PATH for this session (adjust the path if necessary)
$env:PATH = "C:\msys64\ucrt64\bin;" + $env:PATH

# Run the tests
ctest
```

Alternatively, you can run the tests by providing the full path to `ctest.exe`:

```powershell
& "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\ctest.exe"
```
