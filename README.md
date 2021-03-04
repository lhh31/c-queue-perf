# Intrdouction
A set of benchmarks to compare the performance of various types of multithreading. The following types are currently tested
* ZMQ inproc
* Mutexes and conditional signalling
* Counting Semaphores

# Build
To build the binaries run
```bash
make all
```
This requires that ZMQ is installed on your system

# Test
To test how long each take to run
```bash
make time
```

# Format
A .clang-format is included to foramt the files. Requries clang-format > 9.0
```bash
make format
````
