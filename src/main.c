#include "console_application.h"
#include <stdlib.h>

int main(int argc, char** argv) {

    /*
    if (argc < 4) return 1;
    char player_count = *argv[1];
    char n_lines = *argv[2];
    char n_columns = *argv[3];
    */

    ConsoleApplication app = ConsoleApplication_init(3, 6, 9);
    ConsoleApplication_run(&app);
    ConsoleApplication_destroy(&app);
    return EXIT_SUCCESS;
}
