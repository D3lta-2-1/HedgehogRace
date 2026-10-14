#include "console_application.h"
#include "game.h"
#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void display_help() { printf("execpted format: [player_count] [hedgehog_per_player] [ligne_number] [column_number]\n"); }

int main(int argc, char** argv) {
    size_t player_count = 2;
    size_t hedgehog_count = 4;
    size_t width = 9;
    size_t height = 6;

    if (argc > 5) {
        printf("too many arguments: \n");
        display_help();
        return EXIT_SUCCESS;
    }

    if (argc > 1) {
        size_t len = strlen(argv[1]);
        char* end;
        player_count = strtoull(argv[1], &end, 10);
        if (argv[1] + len != end) {
            printf("invalid value for [player_count], expected a integer\n");
            display_help();
            return EXIT_SUCCESS;
        }
        if (player_count < 2) {
            printf("[player_count] must be greater or equal to 2\n");
            return EXIT_SUCCESS;
        }
        if (player_count > MAX_PLAYER_COUNT) {
            printf("[player_count] is too big\n");
            return EXIT_SUCCESS;
        }
    }

    if (argc > 2) {
        size_t len = strlen(argv[2]);
        char* end;
        hedgehog_count = strtoimax(argv[2], &end, 10);
        if (argv[2] + len != end) {
            printf("invalid value for [ligne_number], expected an integer\n");
            return EXIT_SUCCESS;
        }
        if (hedgehog_count < 1) {
            printf("[hedgehog_per_player] must be positive\n");
            return EXIT_SUCCESS;
        }
        if (hedgehog_count > INT_MAX) {
            printf("[hedgehog_per_player] is too large\n");
            return EXIT_SUCCESS;
        }
    }

    if (argc > 3) {
        errno = 0;
        size_t len = strlen(argv[3]);
        char* end;
        height = strtoull(argv[3], &end, 10);
        if (argv[3] + len != end) {
            printf("invalid value for [ligne_number], expected a integer\n");
            return EXIT_SUCCESS;
        }
        if (errno == ERANGE) {
            printf("[ligne_number] is too big\n");
            return EXIT_SUCCESS;
        }
    }

    if (argc > 4) {
        errno = 0;
        size_t len = strlen(argv[3]);
        char* end;
        width = strtoull(argv[3], &end, 10);
        if (argv[3] + len != end) {
            printf("invalid value for [column_number], expected a integer\n");
            return EXIT_SUCCESS;
        }
        if (errno == ERANGE) {
            printf("[column_number] is too big\n");
            return EXIT_SUCCESS;
        }
    }

    ConsoleApplication app = ConsoleApplication_init((int)player_count, (int)hedgehog_count, width, height);
    printf("starting a party with %zu players and %zu hedgegogs per player on a %zux%zu board", player_count, hedgehog_count, height, width);
    ConsoleApplication_run(&app);
    ConsoleApplication_destroy(&app);
    return EXIT_SUCCESS;
}
