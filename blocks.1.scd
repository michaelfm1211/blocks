BLOCKS(1)

# NAME

blocks - generate and recombine _blocks_(5) archives

# SYNOPYSIS

*blocks* {-*s* | -*r*} [*-z* _size_] [*-o* _filename_] _files_ _..._

# DESCRIPTION

The *blocks* utility can split up files and generate _blocks_(5) archives,
then later recombine those archives into the original file. By convention,
archives are named in the format #.file.ext.blk, where # is the index of the
archive staring at zero and file.ext is the original filename. Examples
include 0.plan.txt.blk, 7.mypresentation.pptx.blk, 27.hello_world.c.blk, and
8.myexecutable.blk.

# OPTIONS
*-s*
	Split up the input file and generate a _blocks_(5) archive. Only one 
	file may be given as input.
*-r*
	Recombine the input _blocks_(5) archives into the original file.
*-z* _size_
	Specify the maximum size of the resulting archives when using the *-s* 
	option including a 40 byte header. _size_ may include the suffixes K, 
	M, or G to denote kilobytes, megabytes, or gigabytes respectively. If 
	no suffix is added, bytes are used. The default value is 8 megabytes. 
	If the *-r* option is used instead of *-s*, then this option will be 
	ignored.
*-o* _filename_
	Specify the name of the output file(s). If the *-s* option is being
	used, _filename_ will be used to generate the names of output files
	instead of the original filename (_files_). Otherwise, if the *-r*
	option is being used, the recombined output will be written to the
	file _filename_. The default value is output.bin.

# SEE ALSO

_blocks_(5)

# AUTHORS

*blocks* was written by Michael M (https://github.com/michaelfm1211).
