# Huffman Coding

A compression program that uses huffman encoding to assign codewords to characters supplied from stdin and huffman decoding
to output the corresponding characters to stdout given codewords.

Upon typing **make**, two executables are created: **encode** and **decode**. Here is their usage:

```encode [-d] [dump_file]```

```decode```

where the *-d* option dumps the average length of a codeword and the achieved 
compression followed by a table displaying each letter's frequency and associated codeword.
The default dump file is created in the current directory and is called *table.txt*.

Below is an example of how to use the two executables, as well as the generated output.

Sender  :```./encode -d < sample.txt > compressed```

Receiver:```cat compressed | ./decode > original```

Here is a look at the dumped table:

