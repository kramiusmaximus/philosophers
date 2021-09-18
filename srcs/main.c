#include "../includes/sauce.h"

t_params    params;

int main(int nargs, char **vargs) {
    if (nargs < 5)
        error_exit("Incorrect number of arguments entered. Enter 4 arguments please, 5 with optional argument.", 1);
    if (parse_params(vargs, &params))
        error_exit("An error occured whilst allocating heap memory with malloc... we're soorryyyyyy", 1);
    if (!valid_arguments(&params))
        error_exit("One or more arguments entered is invalid", 1);
    if (set_the_table(&params))
        error_exit("Malloc failed!", 1);
    begin_feast(&params);
}
