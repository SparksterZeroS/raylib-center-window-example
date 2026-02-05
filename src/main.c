#include <raylib.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360
#define PLAYER_SPEED_MIN 180
#define PLAYER_SPEED_MAX 300
#define PLAYER_SIZE 32

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))


void CenterWindow(int monitor);
int GetMaxWindowScale(int width, int height, int monitor);
int HandleWindowResize(int monitor, int currentScale, int maxScale);



int main(void) {
    SetTraceLogLevel(LOG_DEBUG);
    SetConfigFlags(FLAG_WINDOW_HIDDEN );
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Test Window");
    CenterWindow(0);
    ClearWindowState(FLAG_WINDOW_HIDDEN);

    int currentWindowMonitor = GetCurrentMonitor();
    int maxWindowScale = GetMaxWindowScale(WINDOW_WIDTH, WINDOW_HEIGHT, currentWindowMonitor);

    int currentWindowScale = 1;

    RenderTexture2D frameBuffer = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    float frameBufferScale = 1.0f;

    struct {
        Rectangle rect;
        Color color;
        int speed;
        float angle;
        int directionX;
        int directionY;
    } player;

    player.rect = (Rectangle){
        .x = GetRandomValue(0, WINDOW_WIDTH),
        .y = GetRandomValue(0, WINDOW_HEIGHT),
        .width = PLAYER_SIZE,
        .height = PLAYER_SIZE
    };

    player.speed = 200.0f;
    player.angle = 0.0f;
    player.directionX = 1;
    player.directionY = 1;
    player.color = GREEN;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F4)) {
            currentWindowScale = HandleWindowResize(currentWindowMonitor, currentWindowScale, maxWindowScale);
        }

        frameBufferScale = MIN((float)GetScreenWidth()/WINDOW_WIDTH, (float)GetScreenHeight()/WINDOW_HEIGHT);
;
        if (currentWindowMonitor != GetCurrentMonitor()) {
            currentWindowMonitor = GetCurrentMonitor();
            maxWindowScale = GetMaxWindowScale(WINDOW_WIDTH, WINDOW_HEIGHT, currentWindowMonitor);
        }

        if (player.rect.x < 0) {
            player.directionX = 1;
            player.speed = GetRandomValue(PLAYER_SPEED_MIN, PLAYER_SPEED_MAX);
        }

        if ((player.rect.x + player.rect.width) > WINDOW_WIDTH) {
            player.directionX = -1;
            player.speed = GetRandomValue(PLAYER_SPEED_MIN, PLAYER_SPEED_MAX);
        }

        if (player.rect.y < 0) {
            player.directionY = 1;
            player.speed = GetRandomValue(PLAYER_SPEED_MIN, PLAYER_SPEED_MAX);
        }

        if ((player.rect.y + player.rect.height) > WINDOW_HEIGHT) {
            player.directionY = -1;
            player.speed = GetRandomValue(PLAYER_SPEED_MIN, PLAYER_SPEED_MAX);
        }

        player.rect.x += (float)(player.speed * player.directionX) * GetFrameTime();
        player.rect.y += (float)(player.speed * player.directionY) * GetFrameTime();
        player.angle += ((float)player.speed * 2.0f) * GetFrameTime();
        BeginTextureMode(frameBuffer);
        ClearBackground(ORANGE);
        DrawRectanglePro(player.rect, (Vector2){ .x = player.rect.width / 2.0f, .y = player.rect.height / 2.0f }, player.angle, player.color);
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(
            frameBuffer.texture,
            (Rectangle){
                .x = 0.0f,
                .y = 0.0f,
                .width = (float)frameBuffer.texture.width,
                .height = (float)-frameBuffer.texture.height
            },
            (Rectangle){
                .x = (GetScreenWidth() - ((float)WINDOW_WIDTH*frameBufferScale))*0.5f,
                .y = (GetScreenHeight() - ((float)WINDOW_HEIGHT*frameBufferScale))*0.5f,
                .width = (float)WINDOW_WIDTH * frameBufferScale,
                .height = (float)WINDOW_HEIGHT * frameBufferScale
            },
            (Vector2){ .x = 0.0f, .y = 0.0f },
            0.0f,
            WHITE
        );
        EndDrawing();
    }

    UnloadRenderTexture(frameBuffer);
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

int GetMaxWindowScale(int width, int height, int monitor) {
    int monitorWidth = GetMonitorWidth(monitor);
    int monitorHeight = GetMonitorHeight(monitor);
    int ratioX = monitorWidth / width;
    int ratioY = monitorHeight / height;

    return ((ratioX)<(ratioY)? (ratioX) : (ratioY)); //Basically the min function.
}

int HandleWindowResize(int monitor, int currentScale, int maxScale) {
    int newScale = currentScale + 1;

    if (newScale != maxScale) {
        int newWindowWidth = WINDOW_WIDTH * newScale;
        int newWindowHeight = WINDOW_HEIGHT * newScale;

        SetWindowState(FLAG_WINDOW_HIDDEN);
        SetWindowSize(newWindowWidth, newWindowHeight);        
        CenterWindow(monitor);
        ClearWindowState(FLAG_WINDOW_HIDDEN);
    }

    if (newScale == maxScale - 1) {
        ToggleFullscreen();
    }

    if (newScale == maxScale) {
        SetWindowState(FLAG_WINDOW_HIDDEN);

        ToggleFullscreen();
        SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

        CenterWindow(monitor);
        ClearWindowState(FLAG_WINDOW_HIDDEN);
        return 1;
    }

    return newScale;
}