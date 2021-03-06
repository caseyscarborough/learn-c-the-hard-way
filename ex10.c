/**
 * Learn C The Hard Way
 *
 * Exercise 10: Arrays of Strings, Looping
 */

#include <stdio.h>

int main(int argc, char *argv[])
{
    int i = 0;

    // Go through each argument in argv,
    // skip argv[0] since it is the program name.
    for(i = 1; i < argc; i++) {
        printf("arg %d: %s\n", i, argv[i]);
    }

    // let's make our own array of strings
    char *states[] = {
        "California", "Oregon",
        "Washington", "Texas"
    };

    int num_states = 4;

    for(i = 0; i < num_states; i++) {
        printf("state %d: %s\n", i, states[i]);
    }

    return 0;
}