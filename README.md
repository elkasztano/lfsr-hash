# lfsr-hash
experimental hash generator based on an alternating step generator (ASG)  
Do not use for cryptographical applications!  
lfsr-hash takes bytes from stdin and uses them to create an initial state for three galois lfsrs.    
The lfsrs in turn are used to create the hash value.  
## compiling
Compiling is pretty straight forward, as no external libraries are used.  
Example using gcc: `gcc -o lfsr-hash lfsr-hash.c`  
## usage
Provided lfsr-hash is in your $PATH, you can use one of the following commands:

`cat yourfile | lfsr-hash -n 32` or `lfsr-hash -n 32 < yourfile`  

The above commands will each result in a 256 bit hash value in hexadecimal output format.

Adding the option `-c` will result in a color pattern in your terminal, (almost) unique to your file.  

typing `lfsr-hash -h` will show a very basic help text

## further development
Any suggestions for improvement are highly appreciated!  
## reference
https://en.wikipedia.org/wiki/Alternating_step_generator
