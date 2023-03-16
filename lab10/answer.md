# Exercise 1 - Working with CAMERA

99 A2 E1 4F 4D 9A 7D D4 06 AE

## Given the way the address was broken down, how big are the pages in this model?

2^5 = 32bit

## How many TLB Hits and Misses did we have for the randomly generated set of ten addresses? What about for Page Hits and Page Faults?

2 8 3 7

## Did you have any Page Hits? Explain why. Can you think of a set of ten addresses that would result in a Page Hit?

0x20 0x21 0x22 0x23 0x24 0x25 0x26 0x27 0x28 0x29

## Explain the process by which we turn a virtual address into a physical address for the very first access (emphasizing on TLB Misses and Page Faults).

# Exercise 2 - Misses

## Demonstrate that your ten memory accesses results in ten TLB Misses and ten Page Faults. Explain why such behavior occurs.

0x20 0x40 0x60 0x80 0xA0 0xC0 0xE0 0x40 0x60 0x80

# Exercise 3 - Fixing our Faults

## Explain the single parameter change that would result in ten TLB misses, but fewer than ten page faults.

Memory size

# Exercise 4 - Bringing it All Together

## Explain why there is a much higher percentage of TLB misses in this simulation

Page table is larger.

