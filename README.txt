/////////////////////
HUFFMAN CODING
/////////////////////

	In this file when a command is written between "" type it withous the "" (e.g "make" becomes make).


/////////////////////
INSTALLATION

	Requires the make function for compilation.

/////////////////////
COMPILATION

	Type in the console: "make".

/////////////////////
EXECUTION

	You can either use "make run" and then follow the instructions displayed on the screen, or in the folder containing huffman.exe you can also type the following commmands with options:
	"./huffman.exe" on Linux.
	".\huffman.exe" on Windows.

	If you have changed the name of your executable, replace huffman.exe with the name of this executable in the commands above.

	Type "huffman.exe -h" to display the help or read the section HELP at the end of this README to know all the options.


/////////////////////
MISCELLANEOUS

	The output file (where the result will be saved) must not have the same name as the executable file.
	An empty file will not be compressed.
	If there are few identical characters in the file to be compressed the compression will be inefficient.
	If there is only one character that is repeated several times then the compressed file will only contain an header, since the Huffman code will be useless here: we don't use a tree, we just need the character and the size of the file.
	To generate the doxygen documentation type: "make doc".
	To remove the .o files you can type "make cleanwin" on Windows or "make cleanlinux" on Linux.


/////////////////////
HELP:



	NAME
		huffman

	SYNOPSIS
		huffman
		huffman [OPTION] SOURCE DEST

	DESCRIPTION
		Compresses or Decompresses the file SOURCE by using Huffman coding and saves it in the file DEST.

		-h
			display the help and exit.
		-c
			compress SOURCE to DEST.
		-d
			decompress SOURCE to DEST.