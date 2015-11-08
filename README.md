# Fixed Length and Run Length encoder of a PGM image
---------------------------------------------

## Author: Abhay Mittal

This program takes a P2 (ASCII) PGM or a P5 (Binary) PGM as input and generates a fixed length and a run length encoded output. The output after run length encoding is taken as the input for the fixed length encoder to generate a coding scheme. The coding scheme is displayed on the standard output.

### Output
* The output of the fixed length encoder is a file with a .out extension.
* The output of the run length encoder is a file with a .rle extension.

### Error codes
The program provides the following two error codes
* _10_ - File format invalid. File must be a P2 or a P5 PGM.
* _11_ - The maximum gray scale level is more than 255.


Note: Generating the rle output file slows down the program. If speed is a concern, please comment the following line "rle.encode(pixelMap,nPxlMap,packets);" in the main function to preclude the generation of the rle output file.