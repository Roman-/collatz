# Collatz Conjecture (aka 3x+1 problem) Counterexample Finder

This program generates random large binary numbers and performs a futile attempt to find a counterexample to the well-known [Collatz Conjecture](https://youtu.be/094y1Z2wpJg?si=LqJe-1eHn__2wooY). Spoiler: It probably won't.

## Build
```sh
mkidr build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Run
```sh
./collatz
```

## How it works

An N-bit binary number is generated, where N is defined at compile time (see `Config.h`). Starting from this initial number, the program searches upward, performing the 3x+1 operations in [base two](https://en.wikipedia.org/wiki/Collatz_conjecture#As_an_abstract_machine_that_computes_in_base_two) until it verifies that the number reduces below its initial value. Optimizations, such as skipping even numbers, are employed to enhance efficiency.

## Example output
```
Checked 2611M numbers, max steps = 321, now on number 100000001010001010110111110010001001001101011110001010111110010100000001100011110001110011111101001001111101110110000111000110010001110110001011111100010000011011101010011011100000000101011000111000100011 (~2^203)
Checked 2612M numbers, max steps = 321, now on number 100000001010001010110111110010001001001101011110001010111110010100000001100011110001110011111101001001111101110110000111000110010001110110001011111100010000011011101010011011100000010100101001011100100011 (~2^203)
Checked 2613M numbers, max steps = 321, now on number 100000001010001010110111110010001001001101011110001010111110010100000001100011110001110011111101001001111101110110000111000110010001110110001011111100010000011011101010011011100000100011111010000000100011 (~2^203)
Checked 2614M numbers, max steps = 321, now on number 100000001010001010110111110010001001001101011110001010111110010100000001100011110001110011111101001001111101110110000111000110010001110110001011111100010000011011101010011011100000110011001010100100100011 (~2^203)
Checked 2615M numbers, max steps = 321, now on number 100000001010001010110111110010001001001101011110001010111110010100000001100011110001110011111101001001111101110110000111000110010001110110001011111100010000011011101010011011100001000010011011001000100011 (~2^203)
Checked 2616M numbers, max steps = 321, now on number 100000001010001010110111110010001001001101011110001010111110010100000001100011110001110011111101001001111101110110000111000110010001110110001011111100010000011011101010011011100001010001101011101100100011 (~2^203)
Checked 2617M numbers, max steps = 321, now on number 100000001010001010110111110010001001001101011110001010111110010100000001100011110001110011111101001001111101110110000111000110010001110110001011111100010000011011101010011011100001100000111100010000100011 (~2^203)
...
```
