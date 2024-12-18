
# Readme



## How to build


### Clone the Repo and go to the directory for it
```bash
git clone https://github.com/nathanwbrei/phasm
cd phasm
```

### Complete all the install steps at once
```bash
./install.sh

# To Run a PHASM example: 
export LD_LIBRARY_PATH=$DEPS/libtorch/lib:$LD_LIBRARY_PATH

# Run the PDE solver example and dump captured data to CSV
PHASM_CALL_MODE=CaptureAndDump install/bin/phasm-example-pdesolver
```

### OR if you would prefer to do it manually, follow the steps below

```bash

# Install PyTorch, Intel Pin, and JANA2 dependencies
./download_deps.sh
export DEPS=`pwd`/deps

# Build 
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH="$DEPS/libtorch;$DEPS/JANA2/install" -DLIBDWARF_DIR="$DEPS/libdwarf-0.3.4/installdir" -DPIN_ROOT="$DEPS/pin" ..
make install

# To run one of the examples:
export LD_LIBRARY_PATH=$DEPS/libtorch/lib:$LD_LIBRARY_PATH

# Run the PDE solver example and dump captured data to CSV
PHASM_CALL_MODE=CaptureAndDump install/bin/phasm-example-pdesolver

# Run vacuum tool against the example target program
install/bin/phasm-memtrace-pin install/bin/phasm-example-memtrace
```

## Special directions for building on ifarm

You can't download pytorch from the ifarm nodes because the site is
blocked. Instead, download it from a different CUE machine or copy 
it over via scp. 

You need a compiler that supports C++14 for pytorch. The default gcc on
ifarm is 4.8.5, which isn't good enough. Obtain a newer gcc via:
```bash
module unload gcc
module load gcc/10.2.0
```
CMake chooses the gcc/g++ specified by $CC/$CXX, not by whatever is on the $PATH. 
The module files don't appear to be setting these environment variables correctly,
so we have to set them ourselves:
```bash
export CC=`which gcc`
export CXX=`which g++`
```

Farm machines need the pre-cxx11 ABI version of the precompiled PyTorch. 
Go into download_deps.sh and comment/uncomment the following lines:

```bash
#wget --no-check-certificate https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.11.0%2Bcpu.zip
wget --no-check-certificate https://download.pytorch.org/libtorch/cpu/libtorch-shared-with-deps-1.11.0%2Bcpu.zip
```

Finally, you might get an error when trying to run phasm saying that it 
can't find libtorch.so. In this case, you need to add it to your LD_LIBRARY_PATH
like so:
```bash
export LD_LIBRARY_PATH=$DEPS/libtorch/lib:$LD_LIBRARY_PATH
```

Now you can proceed with the generic directions for building and running phasm.

## My contributions
+ Building physics informed neural nets
+ Beginner surrogate modeling
+ scripts for easier installation
