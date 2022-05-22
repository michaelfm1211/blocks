# blocks
blocks is a CLI utility to split up and recombine files. Archives made by blocks are generated with special filenames, but these archives can be renamed 
without consequence. Usage can be found in the man page `blocks(1)`.

### Building
Just run `make`. The `blocks` binary will be compiled along with the man page `blocks.1`. Run `make install` to install them.

### Development and Debugging
Run `make debug` to compile without optimization and with debugging symbols and the address sanitizer. Make a pull request to suggest changes or fork this project.
