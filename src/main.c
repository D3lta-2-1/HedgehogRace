#include <stdio.h>
#include <canvas.h>

int main(int argc, char **argv) {
    Canvas canvas = Canvas_empty(20, 10);
    Canvas_set(&canvas, 5, 5, '*');
    Canvas_set(&canvas, 5, 7, '*');
    Canvas_set(&canvas, 6, 6, '*');

    canvas_flush(&canvas, stdout);
    fflush(stdout);

    return 0;
}
