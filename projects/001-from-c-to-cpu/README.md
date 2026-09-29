# 001 — From C Code to CPU

## Overview

This project investigates how a C program is transformed from source code into instructions executed by the CPU.

The goal is to understand the complete execution path:

```text
C Source
   ↓
Preprocessor
   ↓
Compiler
   ↓
Assembly
   ↓
Object File
   ↓
Linker
   ↓
Executable
   ↓
Process
   ↓
CPU
```

Instead of treating the compiler as a black box, this project uses small experiments to observe each stage and analyze how data moves between registers and memory.

---

## Experiments

### Experiment 1 — C → Executable

Source:

```c
int main() {
    int a = 1;
    int b = 2;
    int c = a + b;
    return c;
}
```

Compile:

```bash
gcc src/hello.c -o hello
```

Run:

```bash
./hello
echo $?
```

The program returns:

```text
3
```

---

### Experiment 2 — Preprocessing

Generate the preprocessed source:

```bash
gcc -E src/hello.c -o experiments/hello.i
```

Observe:

```text
hello.c
   ↓
Preprocessor
   ↓
hello.i
```

For this simple program, the source code changes very little because it does not contain complex preprocessing directives.

---

### Experiment 3 — C → ARM64 Assembly

Generate Assembly:

```bash
gcc -S src/hello.c -o assembly/hello.s
```

The development machine is Apple Silicon, so the generated Assembly uses ARM64 instructions.

For example:

```asm
mov w8, #1
str w8, [sp, #8]

mov w8, #2
str w8, [sp, #4]

ldr w8, [sp, #8]
ldr w9, [sp, #4]

add w8, w8, w9
```

The important observation is the data flow:

```text
Register
   ↓
Store
   ↓
Stack Memory
   ↓
Load
   ↓
Register
   ↓
CPU Operation
   ↓
Register
```

---

### Experiment 4 — Function Calls

Source:

```c
int add(int a, int b) {
    return a + b;
}

int main() {
    int x = 10;
    int y = 20;

    return add(x, y);
}
```

Generate Assembly with no optimization:

```bash
gcc -S -O0 src/hello.c -o assembly/add-O0.s
```

The experiment shows how function arguments move through registers and stack memory.

Simplified data flow:

```text
main()

x = 10
y = 20

   ↓

Stack
   ↓
w0 = 10
w1 = 20

   ↓
bl _add

   ↓

add()

w0 / w1
   ↓
Stack
   ↓
w8 / w9
   ↓
add
   ↓
w0 = 30

   ↓
ret
```

The key observation is that the actual arithmetic is performed using registers, while the stack is used as memory for storing function-related data.

---

## Experiment 5 — Compiler Optimization

Compare different optimization levels:

```bash
gcc -S -O0 src/hello.c -o assembly/add-O0.s
gcc -S -O2 src/hello.c -o assembly/add-O2.s
gcc -S -O3 src/hello.c -o assembly/hello-O3.s
```

At `-O0`, the generated Assembly contains more explicit memory operations.

At higher optimization levels, the compiler can eliminate unnecessary memory accesses and simplify computations.

For example, a simple calculation may eventually become:

```asm
mov w0, #30
ret
```

This demonstrates an important principle:

> The source-level variables do not necessarily correspond to physical memory locations in the final machine code.

---

## Key Concepts

Through these experiments, the project studies:

* C compilation
* Preprocessing
* ARM64 Assembly
* Registers
* Stack memory
* Stack Pointer (`sp`)
* Function calls
* Calling conventions
* `ldr` / `str`
* `add`
* `bl`
* `ret`
* Compiler optimization
* `-O0` / `-O2` / `-O3`

---

## Data Flow Model

The current mental model is:

```text
              ┌─────────────┐
              │   Register  │
              └──────┬──────┘
                     │
                  str│ldr
                     ↕
              ┌─────────────┐
              │ Stack Memory│
              └─────────────┘

Register
   ↓
CPU executes operation
   ↓
Register
   ↓
Function return
```

The central idea is:

> **CPU computation mainly happens in registers, while the stack is a region of memory used to store function-related data.**

---

## Current Status

* [x] C source → executable
* [x] C source → preprocessed source
* [x] C source → ARM64 Assembly
* [x] Object file generation
* [x] Function call analysis
* [x] Register / stack data flow analysis
* [x] `-O0` / `-O2` / `-O3` comparison

---

## Next Experiments

Planned experiments:

1. Stack Frame
2. ARM64 Calling Convention
3. Object File Structure
4. Symbol Table
5. Linker
6. Process Creation
7. Virtual Memory
8. CPU Cache
9. Performance Benchmarking

---

## Engineering Method

```text
Question
   ↓
Experiment
   ↓
Implement
   ↓
Observe
   ↓
Measure
   ↓
Analyze
   ↓
Explain
   ↓
Share
```

The goal of this project is not simply to learn compiler concepts, but to understand computer systems by building and measuring small experiments.
