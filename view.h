#pragma once

#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "gametime.h"
#include "glm/glm.hpp"
#include <vector>

namespace view
{
    extern RECT rect;
    extern float aspect;

    void InitScreen();
    void InitSnow();
    void DrawSnow();
    void MoveSnow();

    void InitMoonTex();
    void DrawMoon();

    void InitTreesTextures();
    void DrawTree(int treeId, int treeTexId, glm::vec2 treePos);
    void DrawTrees();

    void InitFirework();
    void DrawAllFireworks();
    void MoveAllFireworks();
}
