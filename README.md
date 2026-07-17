# LLVM GVN, DSE & DCE Passes

LLVM legacy pass plugin implementing Global Value Numbering, Dead Store Elimination and Dead Code Elimination. GVN removes redundant computations, DSE removes overwritten or unused local stores, and DCE removes unused computations without side effects. The combined pass repeatedly runs GVN, DSE and DCE until no pass changes the function.

## Requirements

- LLVM 17.0.0
- Clang 17.0.0
- CMake
- GNU Make

## Build

Place `OurGVNDCE` in `llvm/lib/Transforms` and add the following line to `llvm/lib/Transforms/CMakeLists.txt`:

```cmake
add_subdirectory(OurGVNDCE)
```

Build the plugin from the LLVM build directory:

```bash
cmake -S ../llvm -B .
cmake --build . --target LLVMOurGVNDCE
```

## Usage

Compile a C example to LLVM IR:

```bash
./bin/clang \
  -O0 \
  -fwrapv \
  -Xclang -disable-O0-optnone \
  -emit-llvm -S \
  example.c -o example.ll
```

Run the combined pass:

```bash
./bin/opt \
  -load lib/LLVMOurGVNDCE.so \
  -enable-new-pm=0 \
  -our-gvn-dse-dce \
  example.ll \
  -S -o example_out.ll
```

Individual passes can be executed with `-our-gvn`, `-our-dse` or `-our-dce`.

For GVN, promote stack variables to SSA form before running the pass:

```bash
./bin/opt \
  -enable-new-pm=0 \
  -mem2reg \
  example.ll \
  -S -o example_mem2reg.ll

./bin/opt \
  -load lib/LLVMOurGVNDCE.so \
  -enable-new-pm=0 \
  -our-gvn \
  example_mem2reg.ll \
  -S -o example_gvn_out.ll
```

The implementation is conservative around memory accesses, aliases and function calls.
