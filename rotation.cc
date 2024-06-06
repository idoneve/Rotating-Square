#include <iostream>
#include <SDL2/SDL.h>

struct Rect {
    int x, y;
    int w, h;
};

struct Rotate {
    bool is_running;
    double theta;

    SDL_Window *window;
    SDL_Renderer *renderer;

    Rect rect = { 400, 300, 30, 30 };
    SDL_FPoint *points;
    SDL_FPoint *trans_points;

    void get_points(const Rect rect) {
        points = new SDL_FPoint[rect.w * rect.h];
        trans_points = new SDL_FPoint[rect.w * rect.h];
        SDL_FPoint point;

        for (int i = -rect.w / 2; i < rect.w / 2; ++i) {
            for (int j = -rect.h / 2; j < rect.h / 2; ++j) {
                point.x = i + rect.x;
                point.y = j + rect.y;
                points[(j + rect.h / 2) + ((i + rect.w / 2) * rect.w)] = point;
            }
        }
    }

    void rotate(double theta, SDL_FPoint *points, int count) {
        for (int i = 0; i < count; ++i) {
            SDL_FPoint point = points[i];

            double x = point.x - 400;
            double y = point.y - 300;

            point.x = 400 + x * SDL_cos(theta) - y * SDL_sin(theta);
            point.y = 300 + x * SDL_sin(theta) + y * SDL_cos(theta);

            trans_points[i] = point;
        }
    }

    void init(const char *title, int x, int y, int w, int h, bool fullscreen) {
        SDL_Init(SDL_INIT_EVERYTHING);

        window = SDL_CreateWindow(title, x, y, w, h, (fullscreen) ? SDL_WINDOW_FULLSCREEN : 0);
        renderer = SDL_CreateRenderer(window, -1, 0);

        get_points(rect);

        is_running = true;
    }

    void handle_events() {
        SDL_Event e;
        SDL_PollEvent(&e);

        switch (e.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        default:
            break;
        }
    }

    void update() {
        for (int i = 0; i < rect.w * rect.h; ++i) {
            rotate(theta, points, rect.w * rect.h);
        }

        theta += 0.1;

        SDL_Delay(16);
    }

    void render() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawPointsF(renderer, trans_points, rect.w * rect.h);

        SDL_RenderPresent(renderer);
    }

    void clear() {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    }
};

int main(int argc, char const *argv[]) {
    Rotate *r = new Rotate();

    r->init("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
    while (r->is_running) {
        r->handle_events();
        r->update();
        r->render();
    }

    r->clear();

    return 0;
}
