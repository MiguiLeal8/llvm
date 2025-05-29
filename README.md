This is the Intel staging area for llvm.org contributions and the home for
Intel LLVM-based projects:

- [oneAPI DPC++ compiler](#oneapi-dpc-compiler)
- [Late-outline OpenMP and OpenMP Offload](#late-outline-openmp-and-openmp-offload)
- [QPU simulation](#qpu-simulation)

For general contribution process see [CONTRIBUTING.md](./CONTRIBUTING.md)

## oneAPI DPC++ compiler

[![oneAPI logo](https://spec.oneapi.io/oneapi-logo-white-scaled.jpg)](https://www.oneapi.io/)

[![SYCL Post Commit](https://github.com/intel/llvm/actions/workflows/sycl-post-commit.yml/badge.svg?branch=sycl)](https://github.com/intel/llvm/actions/workflows/sycl-post-commit.yml)
[![Generate Doxygen documentation](https://github.com/intel/llvm/actions/workflows/sycl-docs.yml/badge.svg?branch=sycl)](https://github.com/intel/llvm/actions/workflows/sycl-docs.yml)

The DPC++ is a LLVM-based compiler project that implements compiler and runtime
support for the SYCL\* language. The project is hosted in the
[sycl](https://github.com/intel/llvm/tree/sycl) branch and is synced with the
tip of the LLVM upstream main branch on a regular basis (revisions delay is
usually not more than 1-2 weeks). DPC++ compiler takes everything from LLVM
upstream as is, however some modules of LLVM might be not included in the
default project build configuration. Additional modules can be enabled by
modifying build framework settings.

The DPC++ goal is to support the latest SYCL\* standard and work on that is in
progress. DPC++ also implements a number of extensions to the SYCL\* standard,
which can be found in the [sycl/doc/extensions](./sycl/doc/extensions)
directory.

The main purpose of this project is open source collaboration on the DPC++
compiler implementation in LLVM across a variety of architectures, prototyping
compiler and runtime library solutions, designing future extensions, and
conducting experiments. As the implementation becomes more mature, we try to
upstream as much DPC++ support to LLVM main branch as possible. See
[SYCL upstreaming working group notes](https://github.com/intel/llvm/wiki/SYCL-upstreaming-working-group-meeting-notes)
for more details.

Note that this project can be used as a technical foundation for some
proprietary compiler products, which may leverage implementations from this open
source project. One of the examples is
[Intel(R) oneAPI DPC++ Compiler](https://www.intel.com/content/www/us/en/developer/tools/oneapi/dpc-compiler.html)
Features parity between this project and downstream projects is not guaranteed.

Project documentation is available at:
[DPC++ Documentation](https://intel.github.io/llvm/).

### How to use DPC++

#### Dockerfiles

See available Dockerfiles to create containers with pre-built/pre-installed DPC++ compiler at:
[Containers](./sycl/doc/developer/DockerBKMs.md#sycl-dockerfiles-overview)

#### Releases

Daily builds of the sycl branch on Linux are available at
[releases](https://github.com/intel/llvm/releases).
A few times a year, we publish [Release Notes](./sycl/ReleaseNotes.md) to
highlight all important changes made in the project: features implemented and
issues addressed. The corresponding builds can be found using
[search](https://github.com/intel/llvm/releases?q=oneAPI+DPC%2B%2B+Compiler&expanded=true)
in daily releases. None of the branches in the project are stable or rigorously
tested for production quality control, so the quality of these releases is
expected to be similar to the daily releases.

#### Build from sources

See [Get Started Guide](./sycl/doc/GetStartedGuide.md).

### Report a problem

Submit an [issue](https://github.com/intel/llvm/issues) or initiate a
[discussion](https://github.com/intel/llvm/discussions).

### How to contribute to DPC++

This project welcomes contributions from the community. Please refer to [CONTRIBUTING](/CONTRIBUTING.md)
for general guidelines around contributing to this project. You can then see
[ContributeToDPCPP](./sycl/doc/developer/ContributeToDPCPP.md) for DPC++ specific
guidelines.

## Late-outline OpenMP\* and OpenMP\* Offload

See [openmp](/openmp) branch.

## QPU Simulation

Experimental support for Quantum Processing Unit (QPU) simulation within LLVM, including GPU-accelerated workflows.

### Key Features

- Hybrid quantum-classical workflows via SYCL
- Multi-backend support: Native CPU, NVIDIA CUDA
- HPC cluster integration (CiTIUS)
- Quiskit AER compatibility

### Environment Setup

```bash
mkdir ~/sycl_workspace
cd ~/sycl_workspace
export DPCPP_HOME=~/sycl_workspace
```

### Native CPU

#### Enviroment configuration

```bash
# Configure for native CPU
python $DPCPP_HOME/llvm/buildbot/configure.py --native_cpu

# Compilation
python $DPCPP_HOME/llvm/buildbot/compile.py -j$(nproc)

# Environment setup
export PATH=$DPCPP_HOME/llvm/build/bin:$PATH
export LD_LIBRARY_PATH=$DPCPP_HOME/llvm/build/lib:$LD_LIBRARY_PATH
```

#### Compile and Run

```bash
clang++ -fsycl -fsycl-targets=native_cpu -std=c++20  source.cpp -o source.exe
./source.exe
```

### CiTIUS HPC Cluster (Native CPU + NVIDIA GPU)

#### Enviroment Configuration

```bash
# Since the system-provided libzstd.a is not suitable for dynamic linking, you should build your own version of zstd that is compiled correctly with -fPIC (Position Independent Code).
cd ~/custom-zstd/zstd-1.5.0/build/cmake/build/
cmake .. -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_INSTALL_PREFIX=$HOME/custom-zstd-install -DZSTD_BUILD_STATIC=ON -DZSTD_BUILD_SHARED=OFF
make -j$(nproc)
make install
export ZSTD_ROOT=$HOME/custom-zstd-install
export CMAKE_PREFIX_PATH=$ZSTD_ROOT:$CMAKE_PREFIX_PATH
export LIBRARY_PATH=$ZSTD_ROOT/lib:$LIBRARY_PATH
export LD_LIBRARY_PATH=$ZSTD_ROOT/lib:$LD_LIBRARY_PATH
export CPATH=$ZSTD_ROOT/include:$CPATH

# Other modules
module load CMake/3.21.1-GCCcore-11.2.0
module load Python/3.9.6-GCCcore-11.2.0
module load Ninja/1.10.2-GCCcore-11.2.0
module load hwloc/2.5.0-GCCcore-11.2.0
```

#### Native CPU Backend

##### Enviroment Configuration

```bash
python $DPCPP_HOME/llvm/buildbot/configure.py --native_cpu
python $DPCPP_HOME/llvm/buildbot/compile.py -j$(nproc)

export PATH=$DPCPP_HOME/llvm/build/bin:$PATH
export LD_LIBRARY_PATH=$DPCPP_HOME/llvm/build/lib:$LD_LIBRARY_PATH
```

##### Compile and Run

```bash
clang++ -fsycl -fsycl-targets=native_cpu \
  -stdlib=libstdc++ \
  -L/opt/citius/modules/software/GCCcore/11.2.0/lib64 \
  -Wl,-rpath,/opt/citius/modules/software/GCCcore/11.2.0/lib64 \
  -std=c++20 \
  source.cpp -o source.exe
./source.exe
```

###### Key Compilation Flags Explained

###### `-fsycl`

Enables SYCL compilation support in the Intel LLVM-based compiler.

###### `-fsycl-targets=native_cpu`

Specifies the local CPU as the target backend, which is particularly useful for:

- Development and testing without GPU dependencies
- Debugging SYCL code in a simplified environment
- Environments without compatible accelerators

###### `-stdlib=libstdc++`

Forces the use of GCC's C++ standard library because:

- SYCL runtime libraries (like `libsycl.so`) are built against `libstdc++`
- Ensures ABI compatibility between user code and SYCL runtime
- Prevents standard library version conflicts

###### `-L` and `-Wl,-rpath`

These linker flags work together to:

1. Locate the correct version of `libstdc++.so` during:
   - Compilation (`-L` flag)
   - Runtime (`-Wl,-rpath` flag)
2. Prevent common symbol errors (e.g., `GLIBCXX_3.4.26` not found)
3. Handle cases where multiple standard library versions exist

##### Important Technical Notes

###### Template Function Requirements

The explicit use of `malloc_device<T>` without prior prototype declaration:

- Is **not** standard-compliant in C++17
- Requires C++20 or higher standard version
- Is commonly used in SYCL device memory allocation

#### CUDA Backend

##### GPU Node Allocation

```bash
salloc --nodelist=hpc-gpu[1-2] --time=08:00:00
```

##### Enviroment Configuration

```bash
module load CUDA/12.4

CC=gcc CXX=g++ python $DPCPP_HOME/llvm/buildbot/configure.py \
    --cuda -DCUDA_Toolkit_ROOT=/usr/local/cuda-12.4 --native_cpu

CC=gcc CXX=g++ python $DPCPP_HOME/llvm/buildbot/compile.py -j$(-nproc)

export CPLUS_INCLUDE_PATH=/opt/citius/modules/software/GCCcore/11.2.0/include/c++/11.2.0:/opt/citius/modules/software/GCCcore/11.2.0/include/c++/11.2.0/x86_64-pc-linux-gnu
export LIBRARY_PATH=/opt/citius/modules/software/GCCcore/11.2.0/lib64
```

##### Compile and Run

1. **Create a script**

```bash
touch example.sh
vim example.sh
```

2. **Add this content**

```sh
#!/bin/bash
#SBATCH --job-name=mytest            # Job name
#SBATCH --nodes=1                    # -N Run all processes on a single node
#SBATCH --ntasks=1                   # -n Run a single task
#SBATCH --cpus-per-task=4            # -c Run 1 processor per task
#SBATCH --mem=1gb                    # Job memory request
#SBATCH --time=05:00:00              # Time limit hrs:min:sec
#SBATCH --qos=regular                # Queue name
#SBATCH --output=mytest_%j.log       # Standard output and error log
#SBATCH --gres=gpu:1

cd sycl_workspace
sycl-ls

$DPCPP_HOME/llvm/build/bin/clang++ -std=c++20 -O3 -fsycl -fsycl-targets=nvptx64-nvidia-cuda \
  --cuda-path=/usr/local/cuda-12.4 \
  --gcc-toolchain=/opt/citius/modules/software/GCCcore/11.2.0 \
  source.cpp -o source.exe
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DPCPP_HOME/llvm/build/lib ./source.exe
```

3. **Run the script**

```bash
sbatch example.sh
```

4. **Check the results**

```bash
cat mytest_<test_id>.log
```

As a result of the sycl-ls command in the script, this must be shown:

```bash
[cuda:gpu][cuda:0] NVIDIA CUDA BACKEND, Tesla V100S-PCIE-32GB 7.0 [CUDA 12.6]
[cuda:qpu][cuda:1] NVIDIA CUDA BACKEND, Simulated QPU on NVIDIA 7.0 [CUDA 12.6]
[native_cpu:cpu][native_cpu:0] SYCL_NATIVE_CPU, Native CPU 0.1 [0.0.0]
[native_cpu:qpu][native_cpu:1] SYCL_NATIVE_CPU, Simulated QPU 0.1 [0.0.0]
```

### Aer Quantum Circuit

#### Enviroment Configuration

```bash
sudo apt-get install pybind11-dev

cd ~/sycl_workspace/openmp
mkdir build
cd build
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..
make
sudo make install

export DPCPP_HOME=~/sycl_workspace
export PATH=$DPCPP_HOME/llvm/build/bin:$PATH
export LD_LIBRARY_PATH=$DPCPP_HOME/llvm/build/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$DPCPP_HOME/llvm/openmp/build/runtime/src:$LD_LIBRARY_PATH
export PYTHONPATH=~/sycl_workspace/llvm:$PYTHONPATH

python $DPCPP_HOME/llvm/buildbot/configure.py --native_cpu  \
  --shared-libs \
  --cmake-opt="-DLLVM_ENABLE_PROJECTS=clang;sycl;llvm-spirv;opencl;xpti;xptifw;libdevice;sycl-jit;libclc;openmp" \
  --cmake-opt="-DLIBOMP_ENABLE_SHARED=ON" \
  --cmake-opt="-DOPENMP_ENABLE_LIBOMPTARGET=ON"
python $DPCPP_HOME/llvm/buildbot/compile.py -j$(-nproc)
```

#### Compile and Run

```bash
clang++ -fsycl -fsycl-targets=native_cpu -std=c++20 source.cpp -o source.exe \
  -I/llvm/openmp/build/runtime/src \
  -I<python include directory> \
  -L/llvm/openmp/build/runtime/src \
  -L<python library directory> \
  -lpython3.10 -lomp \
  -llapack -lblas -lm

./source.exe
```

Example with Python3.10:

```bash
clang++ -fsycl -fsycl-targets=native_cpu -std=c++20 source.cpp -o source.exe \
  -I/llvm/openmp/build/runtime/src \
  -I/usr/include/python3.10 \
  -L/llvm/openmp/build/runtime/src \
  -L/usr/lib/python3.10/config-3.10-x86_64-linux-gnu \
  -lpython3.10 -lomp \
  -llapack -lblas -lm

./source.exe
```

### Notes

#### Select the simulated QPU as device

```bash
sycl::queue q(sycl::qpu_selector_v);
```

### Example Implementations

Explore our collection of quantum simulation examples:

1. **Device Compatibility Demo**  
   [`devices.cpp`](./test-qpu/devices.cpp) - Demonstrates QPU-specific functionality that fails on conventional CPUs

2. **Basic Quantum Gate**  
   [`h_gate.cpp`](./test-qpu/h_gate.cpp) - Minimal implementation of a Hadamard gate operation

3. **Full Hybrid Simulation**  
   [`sycl_aer.cpp`](./test-qpu/sycl_aer.cpp) - Complete SYCL/Qiskit Aer integration with multi-gate circuit simulation

# License

See [LICENSE](./sycl/LICENSE.TXT) for details.

<sub>\*Other names and brands may be claimed as the property of others.</sub>
