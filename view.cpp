#include "view.h"
#include "textures.h"

const double M_PI = 3.14159265358979323846;
const int SNOW_COUNT = 10000;

RECT view::rect = { 0, 0, 0, 0 };
float view::aspect;
glm::vec2 snowVec[SNOW_COUNT];
glm::vec2 snowSpeed[SNOW_COUNT];

glm::vec2 moonPos = glm::vec2(0.0, 0.0);
float moonAlpha = 0.0;

unsigned char* pTreeTexArr[4] = { tree1_raw, tree2_raw, tree3_raw, tree4_raw };

float square[12] = {-1.0, 1.0, 1.0, 1.0, 1.0, -1.0,
                    -1.0, 1.0, -1.0, -1.0, 1.0, -1.0};

float scaleToFit = 2.0f;

void view::InitScreen()
{
    view::rect.right = GetSystemMetrics(SM_CXSCREEN);
    view::rect.bottom = GetSystemMetrics(SM_CYSCREEN);
    view::aspect = (float)(view::rect.right - view::rect.left) / (view::rect.bottom - view::rect.top);
}

float randomFloat()
{
    int num = rand() % RAND_MAX;
    float fl = (float)num / RAND_MAX;
    float sig = fl * 2.0 - 1.0;
    return sig;
}

glm::vec2 randomInitSnow()
{
    return glm::vec2(randomFloat() * view::aspect * scaleToFit, randomFloat() * scaleToFit);
}

glm::vec2 randomSnow()
{
    glm::vec2 temp = randomInitSnow();
    temp.y = 1.0f * scaleToFit;
    return temp;
}

void view::InitSnow()
{
    for (int i = 0; i < SNOW_COUNT; i++)
    {
        snowVec[i] = randomInitSnow();
        snowSpeed[i] =glm::vec2(randomFloat() * 0.0001f, 1.0f + randomFloat() * 0.2f);
    }
}

void view::DrawSnow()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPointSize(1.0);
    glEnable(GL_POINT_SMOOTH);
    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, snowVec);
    
    glColor3f(1.0, 1.0, 1.0);
    glDrawArrays(GL_POINTS, 0, SNOW_COUNT);

    glDisableClientState(GL_VERTEX_ARRAY);
}

GLuint moonTex;
int width = 50, height = 400;
void view::InitMoonTex() // all textures
{
    glGenTextures(1, &moonTex);
    glBindTexture(GL_TEXTURE_2D, moonTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Moon_0_raw);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLuint treeTex[4];
int defaultTreeWidth = 256, defaultTreeHeight = 112;
int bigTreeWidth = 256, bigTreeHeight = 128;
void view::InitTreesTextures()
{
    glGenTextures(4, treeTex);
    for (int i = 0; i < 4; i++)
    {
        glBindTexture(GL_TEXTURE_2D, treeTex[i]);

        if (i == 2)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bigTreeWidth, bigTreeHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pTreeTexArr[i]);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, defaultTreeWidth, defaultTreeHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pTreeTexArr[i]);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

int curTex = 0;
const int texCount = 8;

void view::DrawMoon()
{
    float yStart = curTex * 0.125f;
    float yEnd = yStart + 0.125f;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glTranslatef(moonPos.x, moonPos.y, 0.0f);
    glScalef(0.1, 0.1, 0.1);
    glRotatef(moonAlpha-90.0f, 0.0, 0.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, moonTex);

    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, yStart); glVertex2f(-1.0, 1.0);
    glTexCoord2f(1.0, yStart); glVertex2f(1.0, 1.0);
    glTexCoord2f(1.0, yEnd); glVertex2f(1.0, -1.0);
    glTexCoord2f(0.0, yEnd); glVertex2f(-1.0, -1.0);
    glEnd();
    glFlush();

    glDisable(GL_TEXTURE_2D);
}

const int treeTexCount = 4;

void view::DrawTree(int treeId, int treeTexId, glm::vec2 treePos)
{
    float xStart = treeTexId * 0.25f;
    float xEnd = xStart + 0.25f;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(treePos.x, treePos.y, 0.0f);
    glScalef(0.2, 0.2, 0.2);
    if (treeId == 2)
    {
        glTranslatef(0.0, 0.25, 0.0);
    }

    glBindTexture(GL_TEXTURE_2D, treeTex[treeId]);

    glEnable(GL_TEXTURE_2D);

    float treeHeight = (treeId == 2 ? 2.0 : 1.75);

    glBegin(GL_QUADS);
    glTexCoord2f(xStart, 0.01); glVertex2f(-1.0, treeHeight);
    glTexCoord2f(xEnd, 0.01); glVertex2f(1.0, treeHeight);
    glTexCoord2f(xEnd, 1.0); glVertex2f(1.0, -treeHeight);
    glTexCoord2f(xStart, 1.0); glVertex2f(-1.0, -treeHeight);
    glEnd();
    glFlush();

    glDisable(GL_TEXTURE_2D);
}

glm::vec2 treePosArr[4] = { glm::vec2(-1.0, -0.65), glm::vec2(-0.5, -0.65), glm::vec2(0.5, -0.65), glm::vec2(1.0, -0.65) };

void view::DrawTrees()
{
    glColor3f(1.0, 1.0, 1.0);
    int texId = (gametime::ticks % 40000000) / 10000000;
    for (int i = 0; i < 4; i++)
    {
        view::DrawTree(i, texId, treePosArr[i]);
    }
}

glm::vec2 accGrav = glm::vec2(0.0, -1.0);
glm::vec2 accWind = glm::vec2(1.0, 0.0);
float accGravCoeff = 0.001;
float accWindCoeff = 0.0005;

void view::MoveSnow()
{
    for (int i = 0; i < SNOW_COUNT; i++)
    {
        // respawn snow if not on the screen
        if (abs(snowVec[i].x >= aspect*scaleToFit) || abs(snowVec[i].y) >= 1.0f*scaleToFit)
        {
            snowVec[i] = randomSnow();
        }

        snowVec[i] += (accGrav * accGravCoeff + accWind * (accWindCoeff + snowSpeed[i].x)) * (float)gametime::deltaTicksF * 4.0f * snowSpeed[i].y;
    }
    
    float angle = gametime::ticksF / 100.0;
    float sinAngle, cosAngle = cos(angle);
    if (cosAngle > 0.0f)
    {
        sinAngle = sin(angle);
    }
    else
    {
        sinAngle = sin(angle + M_PI);
    }
   
    float x = sinAngle * aspect * 1.2;
    float r = 4.0f;
    float alpha = acos(x / r);
    moonAlpha = glm::degrees(alpha);
    float y = sin(alpha) * r;

    if (x < moonPos.x)
    {
        curTex = (curTex + 1) % texCount;
    }

    moonPos.x = x;
    moonPos.y = y - 3.1f;
}


const int FIREWORK_PART_COUNT = 500;
const int MAX_FIREWORK_COUNT = 50;
const int COLOR_CHANNELS_COUNT = 12;

int qLeft = 0, qRight = 10;

glm::vec2 firework[MAX_FIREWORK_COUNT][FIREWORK_PART_COUNT];
glm::vec2 fireworkSpeed[MAX_FIREWORK_COUNT][FIREWORK_PART_COUNT];
glm::vec2 fireworkPos[MAX_FIREWORK_COUNT];
float opacity[MAX_FIREWORK_COUNT];
double startTime[MAX_FIREWORK_COUNT];
float colors[MAX_FIREWORK_COUNT][COLOR_CHANNELS_COUNT];

double lastFirework;

glm::vec2 gAcc = glm::vec2(0.0, -1.0);

void MoveFirework(int fireworkId)
{
    opacity[fireworkId] = glm::max(1.0 - (gametime::ticksF - startTime[fireworkId]) / 50.0, 0.0);

    if (gametime::ticksF - 15.0 > lastFirework)
    {
        view::InitFirework();
    }

    for (int i = 0; i < FIREWORK_PART_COUNT; i++)
    {
        firework[fireworkId][i] += fireworkSpeed[fireworkId][i] * (float) gametime::deltaTicksF * 0.04f;
        
        fireworkSpeed[fireworkId][i] *= 0.99f;
        fireworkSpeed[fireworkId][i] += 0.012f * gAcc;
    }
}

void view::MoveAllFireworks()
{
    int iterCount = abs(qRight - qLeft);
    for (int i = 0; i < iterCount; i++)
    {
        MoveFirework((qLeft+i) % MAX_FIREWORK_COUNT);
    }
}

void view::InitFirework()
{
    opacity[qRight] = 1.0;
    startTime[qRight] = gametime::ticksF;
    lastFirework = gametime::ticksF;
    fireworkPos[qRight] = glm::vec2(randomFloat() * aspect, randomFloat() * 0.2 + 0.7);

    for (int i = 0; i < COLOR_CHANNELS_COUNT; i++)
    {
        colors[qRight][i] = abs(randomFloat());
    }

    for (int i = 0; i < FIREWORK_PART_COUNT; i++)
    {
        firework[qRight][i] = glm::vec2(0.0, 0.0);
        fireworkSpeed[qRight][i] = glm::normalize(glm::vec2(randomFloat(), randomFloat())) * abs(randomFloat());
    }

    qLeft = (qLeft + 1) % MAX_FIREWORK_COUNT;
    qRight = (qRight + 1) % MAX_FIREWORK_COUNT;
}

void DrawFirework(int i);

void view::DrawAllFireworks()
{
    glEnable(GL_BLEND);
    int iterCount = abs(qRight - qLeft);
    for (int i = 0; i < iterCount; i++)
    {
        DrawFirework((qLeft + i) % MAX_FIREWORK_COUNT);
    }
    glDisable(GL_BLEND);
}

void DrawFirework(int i)
{
    if (opacity[i] < 0.1)
    {
        return;
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(fireworkPos[i].x, fireworkPos[i].y, 0.0);

    glPointSize(2.0);
    glEnable(GL_POINT_SMOOTH);
    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(2, GL_FLOAT, 0, firework[i]);

    glColor4f(colors[i][0], colors[i][1], colors[i][2], opacity[i]);
    glDrawArrays(GL_POINTS, 0, FIREWORK_PART_COUNT / 4);

    glColor4f(colors[i][3], colors[i][4], colors[i][5], opacity[i]);
    glDrawArrays(GL_POINTS, FIREWORK_PART_COUNT / 4, FIREWORK_PART_COUNT / 4);

    glColor4f(colors[i][6], colors[i][7], colors[i][8], opacity[i]);
    glDrawArrays(GL_POINTS, FIREWORK_PART_COUNT / 2, FIREWORK_PART_COUNT / 4);

    glColor4f(colors[i][9], colors[i][10], colors[i][11], opacity[i]);
    glDrawArrays(GL_POINTS, 3 * FIREWORK_PART_COUNT / 4, FIREWORK_PART_COUNT / 4);

    glDisableClientState(GL_VERTEX_ARRAY);
}
