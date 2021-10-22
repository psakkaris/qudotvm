REQUIRED PACKAGES
===
[GTest 1.8.1](https://github.com/google/googletest/releases)


Building the QuDotVM
===
Assuming your src directory is `~/qudotvm`
create a build directory:
```bash
mkdir build
cd build
cmake ../
make
```
You willnow have an executable in the build directory called `qudotvm` and a library file `libqudot.so`


Running the QuDotVM Tests
===
The QuDotVM is testes using GTest 1.8.1 library. In general we compare exepected probability distrubtions of known circuits with the sampled probability distribution of the qudotvm. Given that we get closer and closer to the actual distribution the larger the ensemble size, some test may fail from time to time. This is fixed by re-running the test or by increasing the ensemble size. Note that increasing the ensemble size for all test will make the test take longer but if a new test is failing more than passing it is a good sanity check to increase the ensemble size.


Using the QuDotVM
===

The `qudotvm` runs compiled qudot bytecode in `.qudotc` files. If you have a compiled bytecode file then you can just run the vm with the qudot bytecode compiler is a separate executable.
```bash
qudotvm quantum_program.qudotc
```

Examples
===

The example use the qudot bytecode language. The bytecode programs are in a `.qudot` file and need to be compiled to a `.qudotc` file by the qudot compiler. Once compiled they can be run on the `qudotvm`.


I The Quantum Fourier Transform
==

In this example we implement the QFT starting by first applying a Haddamard Gate on qubits 2 and 3 of the initial state `|0000>` 
```
.qudot qubits=4, ensemble=100000

.gate main: args=0, regs=2, qubit_regs=1
    qload_array q0, 2, 1, 4
    hon q0
    iload r1, 1
    iload r2, 4
    call qft(), r1
    paths
    halt

// two arguments:
//    r1: start qubit
//    r2: end qubit
.gate qft: args=2, regs=11, qubit_regs=3
    iload r3, 1
    // r4 -> q
    move r4, r1
    // r5 -> end qubit + 1
    iadd r5, r2, r3

    for1:
        breq r4, r5, donefor1
        qloadr q3, r4
        hon q3
        // i = q + 1
        iadd r6, r4, r3
        // r=2
        iload r7, 2
        while:
            breq r6, r5, donewhile
            qloadr q0, r4
            qloadr q1, r6
            semi_crot r7, q0, q1
            // i+=1
            iadd r6, r6, r3
            // r+=1
            iadd r7, r7, r3
            br while

        donewhile:
            // q += 1
            iadd r4, r4, r3
            br for1

    donefor1:
        iload r8, 2
        // swap point + 1
        idiv r9, r2, r8
        iadd r9, r9, r3
        // q
        iload r10, 1
    for2:
        breq r10, r9, donefor2
        qloadr q0, r10
        isub r11, r2, r10
        iadd r11, r11, r3
        qloadr q1, r11
        swap_ab q0, q1
        iadd r10, r10, r3
        br for2

    donefor2:
        ret
```

Output:

```
state,frequency,pct
0000,24881,0.24881
0010,21234,0.21234
0100,12539,0.12539
0110,3753,0.03753
1010,3636,0.03636
1100,12507,0.12507
1110,21450,0.2145

```

II The QFT QuDotVM Intrinsic
==

The QFT Is such an important algorithm and used as a components to other quantum algorithms (such as Shor's Algoritm), we have developed a `qft` intrinsic for ease of use.

The example below uses the `qft` intrinsic to do a qft on the qubits 1-4. First we do a Haddammard on qubits 3 and 4 in the initial state `|0000>`:

```
.qudot qubits=4, ensemble=500000

.gate main: args=0, regs=0, qubit_regs=3
    qload_array q0, 2, 3, 4
    hon q0
    qload q1, 1
    qload q2, 4
    qft q1, q2
    halt
```

Output:
```
state,frequency,pct
0000,125127,0.250254
0001,102658,0.205316
0010,53313,0.106626
0011,12736,0.025472
0101,5640,0.01128
0110,8993,0.017986
0111,3968,0.007936
1001,4011,0.008022
1010,8964,0.017928
1011,5615,0.01123
1101,12627,0.025254
1110,53428,0.106856
1111,102920,0.20584
```