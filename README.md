# Huffman Coding

A compression program that uses huffman coding to assign codewords to characters supplied from stdin.

Upon typing **make**, two executables are created: **encode** and **decode**. Here is their usage:

```encode [-d] [dump_file]```
```decode```

where the *-d* option dumps the average length of a codeword and the achieved 
compression followed by a table displaying each letter's frequency and associated codeword.
The default dump file is created in the current directory and is called *table.txt*.

Below is an example of how to use the two executables, as well as the generated output.
