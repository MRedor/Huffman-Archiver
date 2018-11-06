Options:
* -c archive
* -u unarchive
* -f, --file <path> input file
* -o, --output <path> output file


The result of program is output file and some statistics: the size of input, the size of output and the size of additional data in the output.

Example:
```
$ ./huffman -c -f myfile.txt -o result.bin
15678
6172
482
```

