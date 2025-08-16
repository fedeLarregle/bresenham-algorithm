#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef union {
    struct {
        float x;
        float y;
        float z;
    };
    struct {
        float r;
        float g;
        float b;
    };
} v3;

typedef struct {
    v3* pixels;
    int width;
    int height;
} canvas;

float absolute(float a) {
    if (a < 0) {
        return -a;
    }
    return a;
}

float fminf(float a, float b) {
    if (a < b) {
        return a;
    }
    return b;
}

float fmaxf(float a, float b) {
    if (a > b) {
        return a;
    }
    return b;
}

v3 get_pixel(canvas* c, int x, int y) {
    if (x >= 0 && x < c->width && y >= 0 && y < c->height) {
        return c->pixels[y * c->width + x];
    }
    return (v3) {0, 0};
}


canvas* create_canvas(int width, int height) {
    int pixel_size = sizeof(v3) * width * height;
    canvas* c = (canvas*) malloc(sizeof(canvas));
    v3* pixels = (v3*) malloc(pixel_size);

    memset(pixels, 0, pixel_size);
    c->pixels = pixels;
    c->width = width;
    c->height = height;

    return c;
}

void write_pixel(canvas* c, int x, int y, v3 pixel) {
    c->pixels[y * c->width + x] = pixel;
}

void canvas_to_ppm(canvas* c) {
    int width = c->width;
    int height = c->height;
    int total = c->width * c->height;
    printf("P3\n%d %d\n255\n", width, height);
    char* separator = "";
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            v3 pixel = get_pixel(c, i, j);
            int r = (int) (fminf(fmaxf(pixel.r, 0.0f), 1.0f) * 255 + 0.5f);
            int g = (int) (fminf(fmaxf(pixel.g, 0.0f), 1.0f) * 255 + 0.5f);
            int b = (int) (fminf(fmaxf(pixel.b, 0.0f), 1.0f) * 255 + 0.5f);
            printf("%s%d %d %d", separator, r, g, b);
            separator = " ";
        }
        printf("\n");
    }
}

float f(int x, int y, int r) {
    float result = (x * x) + (y * y) - (r * r);
    return result;
}

void draw_circle(canvas* c, v3 color, int r) {
    write_pixel(c, c->width / 2, c->height / 2, color);
    write_pixel(c, (c->width / 2) + r, c->height / 2, color);
    int origin_x = (c->width / 2);
    int origin_y = c->height / 2;

    int x = r;
    int y = 0;

    while (absolute(x) >= absolute(y)) {
        float a = absolute(f(x,     y - 1, r));
        float b = absolute(f(x - 1, y - 1, r));

        if (a < b) {
            y--;
        } else {
            x--;
            y--;
        }

        write_pixel(c,   x + origin_x,    y + origin_y,  color);
        write_pixel(c,   y + origin_y,    x + origin_x,  color);
        write_pixel(c,   y + origin_y,   -x + origin_x,  color);
        write_pixel(c,  -y + origin_y,    x + origin_x,  color);
        write_pixel(c,  -y + origin_y,   -x + origin_x,  color);
        write_pixel(c, (-x + origin_x),   y + origin_y,  color);
        write_pixel(c, (-x + origin_x), (-y + origin_y), color);
        write_pixel(c,   x + origin_x,  (-y + origin_y), color);

    }
}

int main() {
    v3 white_color = (v3) {1, 1, 1};
    int radius = 160;
    int width = 600;
    int height = 600;
    int offset_radius = (width / 2) + radius;

    canvas* c = create_canvas(width, height);

    draw_circle(c, white_color, radius);
    canvas_to_ppm(c);

    return 0;
}
