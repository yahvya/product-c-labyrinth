#include "./window-manager.h"
#include "raylib.h"
#include <assert.h>
#include <stddef.h>

void centerWindow(){
    int currentMonitorId = GetCurrentMonitor();

    SetWindowPosition(
        (GetMonitorWidth(currentMonitorId) - GetScreenWidth()) / 2,
        (GetMonitorHeight(currentMonitorId) - GetScreenHeight()) / 2
    );
}

bool renderWindow(RenderingConfig* renderingConfig,bool (*customRenderingFunction)(RenderingConfig*)){
    assert(renderingConfig != NULL && "La configuration de rendu fournie est NULL");
    assert(customRenderingFunction != NULL && "La fonction de rendu fournie est NULL");

    BeginDrawing();
        ClearBackground(RAYWHITE);
        bool successState = customRenderingFunction(renderingConfig);
    EndDrawing();

    return successState;
}