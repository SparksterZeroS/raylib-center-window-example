#include <raylib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

#define PRIMARY_MONITOR 0

/**
 * Positions the window in the center of the specified monitor.
 * 
 * @param monitor The monitor to center the window at.
 */
void CenterWindow(int monitor);
int HandleWindowResize(int monitor, int currentScale, int maxScale);

int main(void) {
    SetTraceLogLevel(LOG_DEBUG);
    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Test Window");
    CenterWindow(PRIMARY_MONITOR);
    ClearWindowState(FLAG_WINDOW_HIDDEN);


    const int maxScale = MIN(GetMonitorWidth(PRIMARY_MONITOR) / WINDOW_WIDTH, GetMonitorHeight(PRIMARY_MONITOR) / WINDOW_HEIGHT) + 1;
    int currentScale = 1;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F4)) {
            currentScale = HandleWindowResize(PRIMARY_MONITOR, currentScale, maxScale);
        }

        BeginDrawing();

        EndDrawing();
    }

    CloseWindow();
}

void CenterWindow(int monitor) {
    Vector2 windowSize = { GetScreenWidth(), GetScreenHeight() };
    Vector2 monitorPosition = GetMonitorPosition(monitor);
    Vector2 monitorSize = { GetMonitorWidth(monitor), GetMonitorHeight(0) };

    Vector2 windowPosition = {
        monitorPosition.x + (monitorSize.x / 2.0f) - (windowSize.x / 2.0f),
        monitorPosition.y + (monitorSize.y / 2.0f) - (windowSize.y / 2.0f),
    };

    SetWindowPosition((int)windowPosition.x, (int)windowPosition.y);
}

int HandleWindowResize(int monitor, int currentScale, int maxScale) {
    int newScale = currentScale + 1;
    
    if (currentScale != maxScale) {
        int newWindowWidth = WINDOW_WIDTH * newScale;
        int newWindowHeight = WINDOW_HEIGHT * newScale;

        SetWindowState(FLAG_WINDOW_HIDDEN);
        SetWindowSize(newWindowWidth, newWindowHeight);        
        CenterWindow(monitor);
        ClearWindowState(FLAG_WINDOW_HIDDEN);
    }

    if (currentScale == maxScale) {
        TraceLog(LOG_DEBUG, "Fullscreen!");
        ToggleFullscreen();
    }

    if (currentScale > maxScale) {
        TraceLog(LOG_DEBUG, "Windowed!");

        ToggleFullscreen();
        SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

        SetWindowState(FLAG_WINDOW_HIDDEN);
        CenterWindow(monitor);
        ClearWindowState(FLAG_WINDOW_HIDDEN);
        return 1;
    }

    return newScale;
}