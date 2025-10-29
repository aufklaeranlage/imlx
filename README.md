# imlx
Simple interface wrapper for the minilibX

This mianly implements a bunch of helper functions for ease of use that manage
a bunch of structs that can be easily cleaned up with the proper commands.

A simple file to showcase the usage is provided in test/src.

## Documentation

### Session

The session is the struct that keeps track of all the other stucts, it is
initialized with **session_init()** which is mostly a wrapper for **mlx_init()**.

>   t_session s;
>
>   if (session_init(&s) == false) {
>       return (1);
>   }

Once the 

