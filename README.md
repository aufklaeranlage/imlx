The imlx library is not functional on it's own. It is a wrapper API for the
[MinilibX-linux](https://github.com/42paris/minilibx-linux). And mostly came to
be because I wanted to make it easier for myself ot work with the MinilibX.

The library only got tested on one specific system, no guarantee it will work on
yours, if it doesn't let me know.

## Contents

- Source code in C to create the library
- Documentation of the functions inside the files.
- Usage examples as C files
- Documentation of the examples

## Requirements

- The same requirements of the MinilibX apply to this library

## Compilation

Just call **make** on this folder. The library libimlx.a will be created.

To use the library add the folder **inc/** to the places your compiler looks
for header files and the library **imlx** to the libraries that it references.

Example compilation:

> cc -Iinc -I"$MLX_DIR" examples/multiple_windows.c -L. -limlx -L"$MLX_DIR" -lmlx -lm -lXext -lX11

If you want to compile the library for error checking:
- DEBUG=1
    - will compile the library with -g and -O0

# Examples

### Session

The session is the struct that keeps track of all the other stucts, it is
initialized with **session_init()** which is mostly a wrapper for **mlx_init()**.

>   t_session s;
>
>   if (session_init(&s) == false) {
>       return (1);
>   }

Once the 

