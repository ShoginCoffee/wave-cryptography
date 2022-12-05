# wave-cryptography

## Resources

[final report](https://www.overleaf.com/read/fftzthyvxrjy)

[planning report](https://docs.google.com/document/d/1pkwuNpgYdHtaowHwnL-tc-H3kmdxdhS4SDrBNSFlGek/)

[GA weekly plan](https://docs.google.com/document/d/1hy2SNBlXtZeDcJ0FPpg0Yc0sUpfFxmW-pY26DHssZ3k/)

[GA overview](https://docs.google.com/spreadsheets/d/17XEnRngz-wUG5Av5rm6USD8Lc1B98sJ75snNtTbLGMA/)

## Conventions (for devs and code reading)

- When Printing
	- Whatever printing should leave on one newline in **most** cases

- Variables
	- camelCase
	- if it's a variable whose size would pose issues if it was changed, then it should have a fixed size (check stdint.h)
	- pointers are prefixed with the letter 'p'
	- longer, more explanatory variable names are prefered; shorter, more confusing variable names are not
	- unsigned char is used for char variables that should not be displayed as strings
	- variables that contain filesizes in bytes should be uint32 (uint32 max is about 4GB) (for now, since WAV files have a maximum of 4GB size)
	

- Functions
	- camelCase
	- if it’s a function that returns a pointer of a known size, it should instead take a pointer to the variable wanted and modify it (if it's an array: the size is also passed)
	- if it's a function that returns a pointer of an unknown size, it should take in a pointer to an integer that modifies its value to the size of the pointer
	- if it's a function that could give an error, it should return an integer that ONLY communicates error/error type
		- return 0 if function executed successfully
		- use errno where needed; print errno error in function; return 1 when error (function should leave on ONE newline)
		- it's the function that should print the error
		- if function needs to communicate what type of error it is: it should return a positive integer that refers to an error
		- if the the function needs to also return something, it should instead take in a pointer to a desired data type and write the "return" to said pointer
		- reset errno (errno = 0) BEFORE calling a function that will use errno

- Structs
	- PascalCase
	- the struct contents are considered variables therefore all the conventions for variables apply to them

- Files
	- camelCase
	
- Constants
	- SNAKE_CASE

- Encoding
	- the file that will be used to encode message onto: **Container**
	- the file that will be used as a message to be encoded onto the aforementioned file: **Message**
	- the file that the encoding will produce: **Result**
- Dedcoding
	- the file that contains the message and that will be used to decode the message: **Container**
	- the file that will be extracted from the aforementioned file: **Message**

## ToDo-List (for devs)

- ~~put conventions into effect~~
- ~~remove ID from messageHeader~~
- ~~remove createNewWave()~~
- ~~create a "constants.h" header file with CONSTANTS~~
- ~~create a function that prints the contents of a directory given a filepath~~
- ~~make: naming conventions~~
- ~~split createTargetHeaderStruct into createTargetHeaderStruct and printTargetHeaderStruct~~
- complete GUI
	- add encoding function
		- take in target file, message file, result filepath, and result filename.
		- take in crypt key, counter, and nonce
		- take in audio to message ratio
		- make message file header
		- read target file header
		- check if message fits in target file
		- if yes. Create a new file in result file path 
		- read a specific chunk size (what should we decide on???) from target file
		- read a specific chunk size from message file depending on audio to message ratio
		- encrypt message
		- encode encryption
		- write to the newly created result file
		- repeat until there is no message left
		- write rest of target file to the result file (without encryption)
	- add decoding function
	- add help section for users
- create a commandline-like UI
	- encoding function
	- decoding function
	- list commands and how to use them when inputting **--help** or **-h** or **help** or **?** or **commands**
- complete the github documentation
	- ~~add all needed links~~
	- other stuff?
	
- ~~change variables that tell filesize in bytes so they are stored in uint32 (uint32 if stored in bytes stores over 4GB of data but a wave file is max 4GB, however going for uint16 would give us a measly 65MB)~~
- ~~use definitive variable sizes for structs and other data types which don't allow another size~~
- ~~let chacha20() function modify the counter instead. allows for outside temporary storage of the counter if needed after operation and also checking how many steps the chacha block counter has taken so as to not use the same block twice.~~
- ~~make functions return int and add errno functionality~~
- ~~change util functions so they return fixed size ints and change WAV header so it has fixed size ints~~
- ~~stop using google drive and link to the github in “planeringsrapport”, add the links in the github documentation/.md file~~

## Other general information (for devs)

- [markup guide (for .md)](https://www.markdownguide.org/basic-syntax/)
- [overleaf LaTeX guide](https://www.overleaf.com/learn/latex/Learn_LaTeX_in_30_minutes)
- [another LaTeX resource](https://latex-tutorial.com/tutorials/)

