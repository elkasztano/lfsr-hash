# lfsr-hash
experimental hash generator using a linear feedback shift register  
Do not use for cryptographical applications!  
lfsr-hash takes bytes from stdin and uses them to create an initial state for an 128bit-lfsr.    
The lfsr in turn is used to create the hash value.  
## compiling
Compiling is pretty straight forward, as no external libraries are used.  
Example using gcc: `gcc -o lfsr-hash lfsr-hash.c`  
## usage
Provided lfsr-hash is in your $PATH, you can use the following command:  
`cat yourfile | lfsr-hash -n 32`  
The above command will result in a 256 bit hash value in hexadecimal output format.
Adding the option `-c` will result in a color pattern in your terminal, (almost) unique to your file.
## further development
Any suggestions for improvement are highly appreciated!
