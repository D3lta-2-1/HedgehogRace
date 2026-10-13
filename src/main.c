#include "console_application.h"
#include <stddef.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    int player_count = 2;
    int hedgehog_count = 4;
    size_t width = 9;
    size_t height = 6;

    /*
    if (argc < 4) return 1;
    char player_count = *argv[1];
    char n_lines = *argv[2];
    char n_columns = *argv[3];
    */

    ConsoleApplication app = ConsoleApplication_init(player_count, hedgehog_count, width, height);
    ConsoleApplication_run(&app);
    ConsoleApplication_destroy(&app);
    return EXIT_SUCCESS;
}
