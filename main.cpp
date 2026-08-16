#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#define PI 3.14159265358979323846

// ============================================================================
// GLOBAL VARIABLES & STATE
// ============================================================================
int currentScene = 1;
int windowWidth = 1200;
int windowHeight = 700;

// ---------- SCENE 1 : VILLAGE AT SUNRISE ----------
float s1_sunY = 340.0f;
float s1_brightness = 0.0f;
float s1_birdPhase = 0.0f;

// ---------- SCENE 2 : SUBMARINE & PORT ----------
float s2_subX = 0.0f;
float s2_subY = 250.0f;
float s2_wavePhase = 0.0f;
float s2_propAngle = 0.0f;

// Container Crane Variables
float s2_containerY = 280.0f; // Height of the container
float s2_craneDir = 1.0f;     // Direction of crane movement (1 = up, -1 = down)

// Bomb Variables
int s2_bombState = 0; // 0: IDLE, 1: LAUNCHED, 2: EXPLODING, 3: HOUSE_BROKEN
float s2_bombX = 0.0f, s2_bombY = 0.0f;
float s2_explRadius = 0.0f;

struct Bubble { float x, y, alpha; bool active; };
Bubble s2_bubbles[20];

struct Debris { float x, y, vx, vy; };
Debris s2_debris[12];

// ---------- SCENE 3 : CAR ON ROAD ----------
float s3_playerCarX = 200.0f;
float s3_wheelAngle = 0.0f;
bool s3_headlightsOn = false;
float s3_bgOffset = 0.0f;

// Traffic Cars (NPC Cars)
float s3_npcCar1X = 1300.0f;
float s3_npcCar2X = 1800.0f;

// ---------- SCENE 4 : RAINY NIGHT IN CITY ----------
float s4_dayNight = 0.0f;
bool s4_rainOn = true;
float s4_carX = -200.0f;
float s4_carScale = 1.0f;

struct Star { float x, y, size, phase; };
Star s4_stars[40];

struct Raindrop { float x, y, speed, len; };
Raindrop s4_rain[200];

struct Building { float x, w, h; bool lit; };
Building s4_buildings[12];

// ============================================================================
// HELPER DRAWING FUNCTIONS (Simple Shapes)
// ============================================================================
void drawCircle(float cx, float cy, float r, int segments = 30) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * PI * float(i) / float(segments);
        glVertex2f(cx + r * cos(theta), cy + r * sin(theta));
    }
    glEnd();
}

void drawCircleOutline(float cx, float cy, float r, int segments = 30) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * PI * float(i) / float(segments);
        glVertex2f(cx + r * cos(theta), cy + r * sin(theta));
    }
    glEnd();
}

void drawEllipse(float cx, float cy, float rx, float ry, int segments = 30) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * PI * float(i) / float(segments);
        glVertex2f(cx + rx * cos(theta), cy + ry * sin(theta));
    }
    glEnd();
}

void drawRect(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

void drawGradientSky(float topR, float topG, float topB, float botR, float botG, float botB) {
    glBegin(GL_QUADS);
    glColor3f(botR, botG, botB);
    glVertex2f(0, 0);
    glVertex2f(1200, 0);
    glColor3f(topR, topG, topB);
    glVertex2f(1200, 700);
    glVertex2f(0, 700);
    glEnd();
}

// ============================================================================
// REUSABLE OBJECT DRAWERS (Detailed & Realistic)
// ============================================================================

// Draws a realistic tree
void drawTree(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1);

    // Tree Trunk
    glColor3f(0.35f, 0.20f, 0.10f);
    drawRect(-10, 0, 10, 60);

    // Leaves (Overlapping circles for a detailed bush look)
    glColor3f(0.1f, 0.4f, 0.1f);
    drawCircle(-20, 60, 25);
    glColor3f(0.15f, 0.45f, 0.15f);
    drawCircle(20, 60, 25);
    glColor3f(0.2f, 0.5f, 0.2f);
    drawCircle(0, 85, 30);
    glColor3f(0.25f, 0.55f, 0.25f);
    drawCircle(0, 50, 35); // Center fluff

    glPopMatrix();
}

// Draws a village hut
void drawHut(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1);

    // Main House Body (Mud color)
    glColor3f(0.7f, 0.5f, 0.3f);
    drawRect(-45, 0, 45, 50);

    // Door
    glColor3f(0.25f, 0.15f, 0.05f);
    drawRect(-12, 0, 12, 35);

    // Window
    glColor3f(0.6f, 0.8f, 0.9f); // Glass reflection
    drawRect(18, 20, 35, 38);

    // Roof (Thatch / Straw)
    glColor3f(0.5f, 0.3f, 0.1f);
    drawTriangle(-55, 50, 55, 50, 0, 95);

    glPopMatrix();
}

// Note: Added RGB color parameters so we can draw many different colored cars easily!
void drawCar(float x, float y, float scale, float wheelAngle, bool headlightsOn, float bodyR, float bodyG, float bodyB) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1);

    // Tires
    glColor3f(0.1f, 0.1f, 0.1f);
    float wx1 = -35, wx2 = 40, wy = 15, wr = 15;
    drawCircle(wx1, wy, wr);
    drawCircle(wx2, wy, wr);

    // Rims & Spokes (Detailed)
    glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix(); glTranslatef(wx1, wy, 0); glRotatef(wheelAngle, 0, 0, 1); drawCircle(0, 0, 8);
    glBegin(GL_LINES); glVertex2f(-8, 0); glVertex2f(8, 0); glVertex2f(0, -8); glVertex2f(0, 8); glEnd(); glPopMatrix();

    glPushMatrix(); glTranslatef(wx2, wy, 0); glRotatef(wheelAngle, 0, 0, 1); drawCircle(0, 0, 8);
    glBegin(GL_LINES); glVertex2f(-8, 0); glVertex2f(8, 0); glVertex2f(0, -8); glVertex2f(0, 8); glEnd(); glPopMatrix();

    // Lower Body (Uses custom colors)
    glColor3f(bodyR, bodyG, bodyB);
    drawRect(-60, 15, 70, 35);

    // Upper Cabin
    glBegin(GL_QUADS);
    glVertex2f(-30, 35); glVertex2f(20, 35); glVertex2f(10, 55); glVertex2f(-20, 55);
    glEnd();

    // Windows
    glColor3f(0.6f, 0.9f, 0.9f);
    glBegin(GL_QUADS); // Back window
    glVertex2f(-25, 35); glVertex2f(-5, 35); glVertex2f(-5, 50); glVertex2f(-17, 50);
    glEnd();
    glBegin(GL_QUADS); // Front window
    glVertex2f(0, 35); glVertex2f(15, 35); glVertex2f(8, 50); glVertex2f(0, 50);
    glEnd();

    // Bumpers
    glColor3f(0.3f, 0.3f, 0.3f);
    drawRect(-65, 15, -60, 22);
    drawRect(70, 15, 75, 22);

    // Headlights
    if (headlightsOn) {
        // Front light beam (Semi-transparent glow)
        glEnable(GL_BLEND);
        glColor4f(1.0f, 1.0f, 0.6f, 0.4f);
        drawTriangle(70, 28, 250, -10, 250, 60);
        glColor4f(1.0f, 1.0f, 0.6f, 0.2f);
        drawTriangle(70, 28, 350, -30, 350, 90);
        glDisable(GL_BLEND);

        // Front Lamp Bulb
        glColor3f(1.0f, 1.0f, 0.8f);
        drawCircle(70, 28, 5);

        // Tail Lamp glowing red
        glColor3f(1.0f, 0.2f, 0.2f);
        drawRect(-63, 25, -60, 32);
    }
    else {
        // Headlights off
        glColor3f(0.9f, 0.9f, 0.9f);
        drawCircle(70, 28, 4);
        glColor3f(0.5f, 0.0f, 0.0f);
        drawRect(-62, 25, -60, 32);
    }
    glPopMatrix();
}

// ============================================================================
// SCENE 1 : VILLAGE AT SUNRISE
// ============================================================================
void scene1() {
    // Scene 1 - Sky Background 1
    float topR = 0.05f + s1_brightness * 0.40f;
    float topG = 0.05f + s1_brightness * 0.70f;
    float topB = 0.20f + s1_brightness * 0.80f;
    float botR = 0.05f + s1_brightness * 0.95f;
    float botG = 0.05f + s1_brightness * 0.55f;
    float botB = 0.20f + s1_brightness * 0.0f;
    drawGradientSky(topR, topG, topB, botR, botG, botB);

    // Scene 1 - Sun 1
    float sunR = 1.0f, sunG = 0.3f + s1_brightness * 0.6f, sunB = s1_brightness * 0.5f;
    glColor3f(sunR, sunG, sunB);
    drawCircle(600, s1_sunY, 40);

    // Scene 1 - Sun Glow 1
    glEnable(GL_BLEND);
    glColor4f(sunR, sunG, sunB, s1_brightness * 0.5f);
    drawCircle(600, s1_sunY, 65);
    glColor4f(sunR, sunG, sunB, s1_brightness * 0.2f);
    drawCircle(600, s1_sunY, 100);
    glDisable(GL_BLEND);

    // Scene 1 - Flying Birds 1 (Appears only when morning gets bright)
    if (s1_brightness > 0.5f) {
        glColor3f(0.1f, 0.1f, 0.1f);
        glLineWidth(2.0f);
        for (int i = 0; i < 4; i++) {
            float bx = 200 + s1_brightness * 400 + i * 50;
            float by = 500 + i * 15;
            float wingY = 12 * sin(s1_birdPhase + i); // Flapping wings
            glBegin(GL_LINE_STRIP);
            glVertex2f(bx - 12, by + wingY);
            glVertex2f(bx, by);
            glVertex2f(bx + 12, by + wingY);
            glEnd();
        }
        glLineWidth(1.0f);
    }

    // Scene 1 - Background Mountain 1 (Left)
    glColor3f(0.05f * s1_brightness, 0.3f * s1_brightness, 0.1f * s1_brightness);
    drawEllipse(300, 200, 450, 150);
    // Scene 1 - Background Mountain 2 (Right)
    drawEllipse(900, 200, 550, 180);

    // Scene 1 - Ground Field 1
    glColor3f(0.15f * s1_brightness, 0.45f * s1_brightness, 0.20f * s1_brightness);
    drawRect(0, 0, 1200, 250);

    // Scene 1 - Winding Dirt Path 1
    glColor3f(0.4f * s1_brightness, 0.3f * s1_brightness, 0.2f * s1_brightness);
    glBegin(GL_QUAD_STRIP);
    for (float y = 0; y <= 250; y += 20) {
        float xCenter = 600 + 120 * sin(y * 0.02f);
        float width = 25 + (250 - y) * 0.25f; // Gets wider closer to the screen
        glVertex2f(xCenter - width, y);
        glVertex2f(xCenter + width, y);
    }
    glEnd();

    // Scene 1 - Trees
    drawTree(150, 220, 1.3f); // Scene 1 - Tree 1
    drawTree(1050, 200, 1.6f); // Scene 1 - Tree 2
    drawTree(850, 230, 1.0f);  // Scene 1 - Tree 3
    drawTree(250, 180, 0.8f);  // Scene 1 - Tree 4

    // Scene 1 - Village Houses
    drawHut(380, 180, 1.5f); // Scene 1 - Home 1
    drawHut(520, 210, 1.1f); // Scene 1 - Home 2
    drawHut(780, 190, 1.3f); // Scene 1 - Home 3
}

// ============================================================================
// SCENE 2 : SUBMARINE AND PORT (Detailed with Ship and Crane)
// ============================================================================
void scene2() {
    // Scene 2 - Sky Background 1
    drawGradientSky(0.3f, 0.6f, 0.9f, 0.6f, 0.8f, 1.0f);

    // Scene 2 - Land Bank 1
    glColor3f(0.4f, 0.8f, 0.4f);
    drawRect(0, 300, 1200, 380);

    // Scene 2 - Port Dock / Jetty 1 (Where the crane sits)
    glColor3f(0.4f, 0.4f, 0.4f); // Concrete dock
    drawRect(0, 280, 400, 320);
    // Dock pillars
    glColor3f(0.3f, 0.3f, 0.3f);
    drawRect(50, 250, 80, 280);
    drawRect(200, 250, 230, 280);
    drawRect(350, 250, 380, 280);

    // Scene 2 - Cargo Ship 1
    glPushMatrix();
    glTranslatef(150, 280, 0); // Position of the ship on the water
    // Ship hull
    glColor3f(0.2f, 0.2f, 0.3f); // Dark blue hull
    glBegin(GL_POLYGON);
    glVertex2f(0, 0); glVertex2f(250, 0); glVertex2f(280, 40); glVertex2f(-30, 40);
    glEnd();
    // Ship cabin / Bridge
    glColor3f(0.8f, 0.8f, 0.8f);
    drawRect(200, 40, 260, 80);
    // Ship Smoke Stack
    glColor3f(0.6f, 0.1f, 0.1f);
    drawRect(160, 40, 180, 90);
    // Ship Containers on deck
    glColor3f(0.2f, 0.6f, 0.2f); drawRect(20, 40, 70, 70);
    glColor3f(0.7f, 0.2f, 0.2f); drawRect(80, 40, 130, 70);
    glColor3f(0.2f, 0.3f, 0.7f); drawRect(20, 70, 70, 100);
    glPopMatrix();

    // Scene 2 - Crane 1 (Loading/Unloading)
    // Crane Tower base
    glColor3f(0.8f, 0.7f, 0.1f); // Yellow industrial crane
    drawRect(50, 320, 90, 500);
    // Crane Arm
    drawRect(50, 480, 300, 510);
    // Crane Control Cabin
    glColor3f(0.3f, 0.3f, 0.3f);
    drawRect(90, 440, 130, 480);
    // Crane Wire
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(250, 480);
    glVertex2f(250, s2_containerY + 30);
    glEnd();
    glLineWidth(1.0f);

    // Scene 2 - Moving Container 1
    glColor3f(0.7f, 0.4f, 0.1f); // Orange container
    drawRect(210, s2_containerY, 290, s2_containerY + 30);
    glColor3f(0.1f, 0.1f, 0.1f); // Container details/lines
    glBegin(GL_LINES);
    glVertex2f(230, s2_containerY); glVertex2f(230, s2_containerY + 30);
    glVertex2f(250, s2_containerY); glVertex2f(250, s2_containerY + 30);
    glVertex2f(270, s2_containerY); glVertex2f(270, s2_containerY + 30);
    glEnd();

    // Scene 2 - Submarine 1
    glPushMatrix();
    glTranslatef(s2_subX, s2_subY, 0);
    // Hull
    glColor3f(0.9f, 0.8f, 0.2f); // Yellow submarine
    drawEllipse(0, 0, 70, 25);
    // Conning tower
    glColor3f(0.8f, 0.7f, 0.1f);
    drawRect(-15, 15, 15, 45);
    // Periscope
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_LINES); glVertex2f(5, 45); glVertex2f(5, 60); glVertex2f(15, 60); glEnd();
    // Portholes (Windows)
    glColor3f(0.1f, 0.5f, 0.8f);
    drawCircle(-25, 0, 8); drawCircle(0, 0, 8); drawCircle(25, 0, 8);
    // Propeller
    glColor3f(0.5f, 0.5f, 0.5f);
    drawRect(-80, -5, -70, 5);
    glPushMatrix();
    glTranslatef(-80, 0, 0);
    glRotatef(s2_propAngle, 1, 0, 0); // 3D spin illusion
    drawRect(-2, -20, 2, 20);
    glPopMatrix();
    glPopMatrix(); // End Submarine

    // Scene 2 - Bubbles 1 (Submarine trail)
    glEnable(GL_BLEND);
    glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
    for (int i = 0; i < 20; i++) {
        if (s2_bubbles[i].active) {
            drawCircleOutline(s2_bubbles[i].x, s2_bubbles[i].y, 5);
        }
    }
    glDisable(GL_BLEND);

    // Scene 2 - Target House 1 (Enemy Base)
    glPushMatrix();
    glTranslatef(1000, 330, 0);
    if (s2_bombState != 3) {
        // Intact House
        glColor3f(0.6f, 0.6f, 0.5f); drawRect(-50, 0, 50, 60); // Wall
        glColor3f(0.3f, 0.2f, 0.1f); drawRect(-20, 0, 0, 35); // Door
        glColor3f(0.3f, 0.3f, 0.3f); drawRect(15, 25, 40, 45); // Window
        glColor3f(0.6f, 0.1f, 0.1f); drawTriangle(-60, 60, 60, 60, 0, 100); // Roof
    }
    else {
        // Broken House
        glColor3f(0.6f, 0.6f, 0.5f);
        glPushMatrix(); glRotatef(-15, 0, 0, 1); drawRect(-50, 0, -10, 40); glPopMatrix();
        glPushMatrix(); glRotatef(20, 0, 0, 1); drawRect(10, 0, 50, 50); glPopMatrix();
        glColor3f(0.6f, 0.1f, 0.1f);
        glPushMatrix(); glTranslatef(20, -20, 0); glRotatef(30, 0, 0, 1); drawTriangle(-60, 60, 60, 60, 0, 100); glPopMatrix();
        // Smoke
        glEnable(GL_BLEND);
        glColor4f(0.3f, 0.3f, 0.3f, 0.5f);
        drawCircle(0 + 20 * sin(s2_wavePhase), 70 + s2_wavePhase * 10, 30);
        drawCircle(-20 + 15 * cos(s2_wavePhase), 100 + s2_wavePhase * 12, 40);
        glDisable(GL_BLEND);
    }
    glPopMatrix();

    // Scene 2 - Bomb / Torpedo 1
    if (s2_bombState == 1) { // LAUNCHED
        glColor3f(0.2f, 0.2f, 0.2f);
        drawEllipse(s2_bombX, s2_bombY, 18, 7);
        drawTriangle(s2_bombX - 18, s2_bombY, s2_bombX - 25, s2_bombY + 6, s2_bombX - 25, s2_bombY - 6);
    }
    else if (s2_bombState == 2) { // EXPLODING
        glEnable(GL_BLEND);
        float alpha = std::max(0.0f, 1.0f - (s2_explRadius / 150.0f));
        glColor4f(1.0f, 0.2f, 0.0f, alpha); drawCircle(s2_bombX, s2_bombY, s2_explRadius);
        glColor4f(1.0f, 0.6f, 0.0f, alpha); drawCircle(s2_bombX, s2_bombY, s2_explRadius * 0.7f);
        glColor4f(1.0f, 1.0f, 0.6f, alpha); drawCircle(s2_bombX, s2_bombY, s2_explRadius * 0.4f);
        glDisable(GL_BLEND);
        // Debris particles
        glColor3f(0.1f, 0.1f, 0.1f);
        for (int i = 0; i < 12; i++) drawRect(s2_debris[i].x, s2_debris[i].y, s2_debris[i].x + 5, s2_debris[i].y + 5);
    }

    // Scene 2 - Water Surface 1 (Wavy transparency)
    glEnable(GL_BLEND);
    glColor4f(0.1f, 0.55f, 0.75f, 0.85f); // Beautiful deep sea blue
    glBegin(GL_QUAD_STRIP);
    for (float x = 0; x <= 1200; x += 20) {
        float y = 300 + 15 * sin(x * 0.015f + s2_wavePhase);
        glVertex2f(x, y);
        glVertex2f(x, 0);
    }
    glEnd();
    glDisable(GL_BLEND);

    // Scene 2 - Swimming Fish 1
    glColor3f(0.9f, 0.5f, 0.1f); // Orange fish
    float fishX = fmod(1200 - (s2_wavePhase * 40), 1400) - 100;
    drawEllipse(fishX, 150, 18, 10); // Fish body
    drawTriangle(fishX + 15, 150, fishX + 30, 165, fishX + 30, 135); // Fish tail
}

// ============================================================================
// SCENE 3 : CAR ON ROAD (Traffic and Realism added)
// ============================================================================
void scene3() {
    // Scene 3 - Sky Background 1
    drawGradientSky(0.2f, 0.4f, 0.7f, 0.6f, 0.8f, 1.0f);

    // Scene 3 - Distant Parallax Mountains 1
    glPushMatrix();
    glTranslatef(-s3_bgOffset * 0.2f, 0, 0);
    glColor3f(0.2f, 0.5f, 0.3f);
    for (int i = -2; i < 6; i++) drawEllipse(i * 400 + 200, 250, 300, 180);
    glPopMatrix();

    // Scene 3 - Midground Parallax Trees 1
    glPushMatrix();
    glTranslatef(-s3_bgOffset * 0.5f, 0, 0);
    for (int i = -2; i < 12; i++) drawTree(i * 200 + 100, 230, 0.9f);
    glPopMatrix();

    // Scene 3 - Green Grass Ground 1
    glColor3f(0.25f, 0.55f, 0.20f);
    drawRect(0, 0, 1200, 250);

    // Scene 3 - Main Asphalt Road 1
    glColor3f(0.20f, 0.20f, 0.20f);
    drawRect(0, 10, 1200, 190);

    // Scene 3 - Road Center White Dashed Line 1 (Moves automatically with car)
    glColor3f(0.9f, 0.9f, 0.9f);
    glPushMatrix();
    glTranslatef(-fmod(s3_bgOffset * 1.5f, 100.0f), 0, 0);
    for (float x = -100; x < 1400; x += 100) {
        drawRect(x, 95, x + 50, 105);
    }
    glPopMatrix();

    // Scene 3 - Road Top/Bottom Yellow Solid Lines 1
    glColor3f(0.9f, 0.8f, 0.1f);
    drawRect(0, 15, 1200, 20); // Bottom lane edge
    drawRect(0, 180, 1200, 185); // Top lane edge

    // Scene 3 - Street Lamps 1
    glPushMatrix();
    glTranslatef(-s3_bgOffset * 1.0f, 0, 0);
    glColor3f(0.4f, 0.4f, 0.4f);
    for (int i = -2; i < 12; i++) {
        float lx = i * 350 + 150;
        drawRect(lx - 5, 185, lx + 5, 420); // Pole
        drawRect(lx - 25, 410, lx + 5, 425); // Arm
        glColor3f(1.0f, 1.0f, 0.8f);
        drawCircle(lx - 20, 405, 10); // Light Bulb
        glColor3f(0.4f, 0.4f, 0.4f); // Reset color
    }
    glPopMatrix();

    // Scene 3 - NPC Traffic Car 1 (Driving on the top lane)
    // Dark Blue color (0.1, 0.2, 0.6)
    drawCar(s3_npcCar1X, 140, 1.3f, -s3_wheelAngle * 1.5f, false, 0.1f, 0.2f, 0.6f);

    // Scene 3 - NPC Traffic Car 2 (Driving behind NPC 1)
    // Dark Green color (0.2, 0.5, 0.2)
    drawCar(s3_npcCar2X, 140, 1.3f, -s3_wheelAngle * 1.5f, false, 0.2f, 0.5f, 0.2f);

    // Scene 3 - Player Controlled Car 1 (Red Color)
    drawCar(s3_playerCarX, 50, 1.6f, s3_wheelAngle, s3_headlightsOn, 0.8f, 0.1f, 0.2f);

    // Scene 3 - Headlight Floor Glow 1 (Only if Player headlights are ON)
    if (s3_headlightsOn) {
        glEnable(GL_BLEND);
        glColor4f(1.0f, 1.0f, 0.6f, 0.2f);
        drawEllipse(s3_playerCarX + 220, 50, 150, 40);
        glDisable(GL_BLEND);
    }
}

// ============================================================================
// SCENE 4 : RAINY NIGHT IN CITY
// ============================================================================
void scene4() {
    // Scene 4 - Dynamic Sky Day->Night 1
    float tR = 0.5f * (1 - s4_dayNight) + 0.03f * s4_dayNight;
    float tG = 0.7f * (1 - s4_dayNight) + 0.03f * s4_dayNight;
    float tB = 0.9f * (1 - s4_dayNight) + 0.12f * s4_dayNight;
    float bR = 0.8f * (1 - s4_dayNight) + 0.1f * s4_dayNight;
    float bG = 0.9f * (1 - s4_dayNight) + 0.1f * s4_dayNight;
    float bB = 1.0f * (1 - s4_dayNight) + 0.2f * s4_dayNight;
    drawGradientSky(tR, tG, tB, bR, bG, bB);

    // Scene 4 - Twinkling Stars 1
    if (s4_dayNight > 0.1f) {
        glEnable(GL_BLEND);
        for (int i = 0; i < 40; i++) {
            float twinkle = 0.5f + 0.5f * sin(s1_birdPhase * 2.0f + s4_stars[i].phase);
            glColor4f(1, 1, 1, s4_dayNight * twinkle);
            drawCircle(s4_stars[i].x, s4_stars[i].y, s4_stars[i].size);
        }
        glDisable(GL_BLEND);
    }

    // Scene 4 - Sun 1 (Sets down)
    float sunY = 500 - s4_dayNight * 400;
    glColor4f(1.0f, 0.9f, 0.4f, 1.0f - s4_dayNight);
    drawCircle(300, sunY, 45);

    // Scene 4 - Moon 1 (Rises up)
    float moonY = 100 + s4_dayNight * 400;
    glColor4f(0.9f, 0.9f, 0.9f, s4_dayNight);
    drawCircle(900, moonY, 40);
    glColor4f(0.7f, 0.7f, 0.7f, s4_dayNight); // Moon Craters
    drawCircle(890, moonY + 12, 7);
    drawCircle(915, moonY - 5, 9);
    drawCircle(885, moonY - 18, 6);

    // Scene 4 - City Skyline Buildings 1
    for (int i = 0; i < 12; i++) {
        // Blend building color to dark silhouette at night
        float col = 0.5f * (1.0f - s4_dayNight) + 0.1f * s4_dayNight;
        glColor3f(col, col, col + 0.05f);
        drawRect(s4_buildings[i].x, 200, s4_buildings[i].x + s4_buildings[i].w, 200 + s4_buildings[i].h);

        // Building Windows (Turn on lights at night)
        if (s4_buildings[i].lit && s4_dayNight > 0.5f) {
            glColor4f(1.0f, 0.9f, 0.5f, (s4_dayNight - 0.5f) * 2.0f);
        }
        else {
            glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
        }

        for (float wy = 220; wy < 200 + s4_buildings[i].h - 15; wy += 25) {
            for (float wx = s4_buildings[i].x + 10; wx < s4_buildings[i].x + s4_buildings[i].w - 10; wx += 20) {
                drawRect(wx, wy, wx + 10, wy + 15); // Window rectangle
            }
        }
    }

    // Scene 4 - Foreground Ground and Road 1
    glColor3f(0.15f, 0.15f, 0.15f);
    drawRect(0, 0, 1200, 200);

    // Scene 4 - Foreground House 1 (Light turns on automatically)
    // House body is built manually here or using helper. Let's use helper but position carefully.
    glPushMatrix();
    glTranslatef(150, 200, 0);
    glScalef(1.5f, 1.5f, 1.0f);
    // House Walls
    glColor3f(0.6f, 0.6f, 0.6f); drawRect(-50, 0, 50, 60);
    // House Roof
    glColor3f(0.5f, 0.1f, 0.1f); drawTriangle(-60, 60, 60, 60, 0, 100);
    // House Door
    glColor3f(0.3f, 0.2f, 0.1f); drawRect(-20, 0, 0, 35);
    // House Window
    if (s4_dayNight > 0.5f) {
        glColor3f(1.0f, 0.9f, 0.4f); // Warm lit
        drawRect(15, 25, 40, 45);
        glEnable(GL_BLEND);
        glColor4f(1.0f, 0.9f, 0.4f, 0.3f); drawRect(10, 20, 45, 50); // Glow
        glDisable(GL_BLEND);
    }
    else {
        glColor3f(0.2f, 0.3f, 0.4f); // Dark unlit
        drawRect(15, 25, 40, 45);
    }
    glPopMatrix();

    // Scene 4 - Clouds 1
    float cloudBright = s4_rainOn ? 0.4f : 0.7f;
    glPushMatrix(); glTranslatef(200, 600, 0); glScalef(2.0f, 2.0f, 1.0f);
    glColor4f(cloudBright, cloudBright, cloudBright, 0.8f); drawCircle(-20, 0, 20); drawCircle(0, 10, 25); drawCircle(20, 0, 20);
    glPopMatrix();
    glPushMatrix(); glTranslatef(500, 550, 0); glScalef(1.5f, 1.5f, 1.0f);
    glColor4f(cloudBright, cloudBright, cloudBright, 0.8f); drawCircle(-20, 0, 20); drawCircle(0, 10, 25); drawCircle(20, 0, 20);
    glPopMatrix();
    glPushMatrix(); glTranslatef(800, 620, 0); glScalef(2.5f, 2.5f, 1.0f);
    glColor4f(cloudBright, cloudBright, cloudBright, 0.8f); drawCircle(-20, 0, 20); drawCircle(0, 10, 25); drawCircle(20, 0, 20);
    glPopMatrix();

    // Scene 4 - Driving Car 1 (Silver Color)
    bool carLights = s4_dayNight > 0.5f;
    drawCar(s4_carX, 50 + 50 * (1 - s4_carScale), s4_carScale * 1.3f, -s4_carX * 2.0f, carLights, 0.8f, 0.8f, 0.8f);

    // Scene 4 - Rain Particles 1
    if (s4_rainOn) {
        glEnable(GL_BLEND);
        glColor4f(0.6f, 0.8f, 1.0f, 0.6f);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        for (int i = 0; i < 200; i++) {
            glVertex2f(s4_rain[i].x, s4_rain[i].y);
            // Slanted line for realistic falling effect
            glVertex2f(s4_rain[i].x - s4_rain[i].len * 0.3f, s4_rain[i].y - s4_rain[i].len);
        }
        glEnd();

        // Rain Splashes on ground
        for (int i = 0; i < 200; i++) {
            if (s4_rain[i].y < 200) {
                drawEllipse(s4_rain[i].x, s4_rain[i].y, 5, 2);
            }
        }
        glLineWidth(1.0f);
        glDisable(GL_BLEND);
    }
}

// ============================================================================
// HUD & INITIALIZATION
// ============================================================================
void drawHUD() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1200, 0, 700);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Background semi-transparent panel for text readability
    glEnable(GL_BLEND);
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    drawRect(10, 10, 1190, 40);
    glDisable(GL_BLEND);

    glColor3f(1.0f, 1.0f, 1.0f);
    char hudText[256] = "";

    switch (currentScene) {
    case 1:
        sprintf(hudText, "Scene 1/4 - Village at Sunrise  |  Auto-Animation  |  N: Next Scene  P: Prev Scene");
        break;
    case 2:
        sprintf(hudText, "Scene 2/4 - Port & Submarine  |  L-Click or 'B': Launch Torpedo  'R': Reset House  |  N: Next  P: Prev");
        break;
    case 3:
        sprintf(hudText, "Scene 3/4 - Busy Highway  |  W: Forward  S: Backward  H: Headlights ON  O: Headlights OFF (%s)  |  N: Next  P: Prev", s3_headlightsOn ? "ON" : "OFF");
        break;
    case 4:
        sprintf(hudText, "Scene 4/4 - Rainy Night City  |  T: Toggle Rain  D: Reset to Day  |  N: Next  P: Prev");
        break;
    }

    drawText(20, 20, hudText);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void initData() {
    // Scene 2 bubbles init
    for (int i = 0; i < 20; i++) {
        s2_bubbles[i] = { 0, 0, 1.0f, false };
    }
    // Scene 4 stars init
    for (int i = 0; i < 40; i++) {
        s4_stars[i] = { float(rand() % 1200), float(400 + rand() % 300), float(1 + rand() % 3), float(rand() % 10) };
    }
    // Scene 4 rain init
    for (int i = 0; i < 200; i++) {
        s4_rain[i] = { float(rand() % 1300), float(rand() % 700), float(10 + rand() % 15), float(10 + rand() % 20) };
    }
    // Scene 4 buildings init (Generate city blocks)
    float bx = 300;
    for (int i = 0; i < 12; i++) {
        s4_buildings[i] = { bx, float(60 + rand() % 50), float(200 + rand() % 200), (rand() % 2 == 0) };
        bx += s4_buildings[i].w + 10;
    }
}

void resetScene(int scene) {
    if (scene == 1) {
        s1_sunY = 340.0f; s1_brightness = 0.0f;
    }
    else if (scene == 2) {
        s2_subX = -100.0f; s2_subY = 250.0f; s2_bombState = 0; s2_explRadius = 0;
        for (int i = 0; i < 20; i++) s2_bubbles[i].active = false;
    }
    else if (scene == 3) {
        s3_playerCarX = 200.0f; s3_wheelAngle = 0.0f; s3_bgOffset = 0.0f;
        s3_npcCar1X = 1300.0f; s3_npcCar2X = 1800.0f;
    }
    else if (scene == 4) {
        s4_dayNight = 0.0f; s4_carX = -200.0f; s4_carScale = 1.0f;
    }
}

// ============================================================================
// LOGIC UPDATE & CALLBACKS
// ============================================================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    switch (currentScene) {
    case 1: scene1(); break;
    case 2: scene2(); break;
    case 3: scene3(); break;
    case 4: scene4(); break;
    }

    drawHUD();
    glutSwapBuffers();
}

void update(int value) {
    if (currentScene == 1) {
        // Sunrise animation
        if (s1_brightness < 1.0f) {
            s1_brightness += 0.005f;
            s1_sunY += 1.3f; // Sun goes up
        }
        s1_birdPhase += 0.1f; // Wing flap animation
    }
    else if (currentScene == 2) {
        // Continuous wave & propeller animation
        s2_wavePhase += 0.05f;
        s2_propAngle += 20.0f;

        // I am adding the crane container animation here. Moves up and down automatically.
        s2_containerY += s2_craneDir * 0.8f;
        if (s2_containerY > 440.0f) s2_craneDir = -1.0f; // Drop down
        if (s2_containerY < 320.0f) s2_craneDir = 1.0f;  // Pull up

        // Submarine automatic move
        s2_subX += 2.0f;
        if (s2_subX > 1300) s2_subX = -100;
        if (s2_subY > 150) s2_subY -= 1.0f; // Dives slightly

        // Generate Bubbles behind submarine
        if (rand() % 5 == 0) {
            for (int i = 0; i < 20; i++) {
                if (!s2_bubbles[i].active) {
                    s2_bubbles[i] = { s2_subX - 80, s2_subY, 1.0f, true };
                    break;
                }
            }
        }
        // Move Bubbles up
        for (int i = 0; i < 20; i++) {
            if (s2_bubbles[i].active) {
                s2_bubbles[i].y += 3.0f;
                if (s2_bubbles[i].y > 300) s2_bubbles[i].active = false;
            }
        }

        // Bomb / Torpedo logic
        if (s2_bombState == 1) { // 1 = Launched state
            float dx = 1000 - s2_bombX;
            float dy = 330 - s2_bombY;
            float dist = sqrt(dx * dx + dy * dy);
            s2_bombX += (dx / dist) * 10.0f;
            s2_bombY += (dy / dist) * 10.0f;

            if (dist < 15.0f) { // Collision detected
                s2_bombState = 2; // Transition to Exploding
                // Setup debris pieces for explosion
                for (int i = 0; i < 12; i++) {
                    s2_debris[i] = { s2_bombX, s2_bombY, -6.0f + (rand() % 12), 3.0f + (rand() % 10) };
                }
            }
        }
        else if (s2_bombState == 2) { // 2 = Exploding state
            s2_explRadius += 4.0f; // Expand explosion circle
            for (int i = 0; i < 12; i++) {
                s2_debris[i].x += s2_debris[i].vx;
                s2_debris[i].y += s2_debris[i].vy;
                s2_debris[i].vy -= 0.4f; // Fake gravity pulling debris down
            }
            if (s2_explRadius > 150.0f) s2_bombState = 3; // 3 = Broken house (Finished)
        }
    }
    else if (currentScene == 3) {
        // Animate NPC traffic cars moving towards the left side (opposite lane)
        s3_npcCar1X -= 8.0f;
        s3_npcCar2X -= 8.0f;

        // Loop NPC cars back around
        if (s3_npcCar1X < -200) s3_npcCar1X = 1300.0f + (rand() % 300);
        if (s3_npcCar2X < -200) s3_npcCar2X = 1500.0f + (rand() % 400);
    }
    else if (currentScene == 4) {
        // Slow Day to Night transition
        if (s4_dayNight < 1.0f) s4_dayNight += 0.002f;

        s1_birdPhase += 0.05f; // Reuse variable for star twinkle speed

        // Rain movement
        if (s4_rainOn) {
            for (int i = 0; i < 200; i++) {
                s4_rain[i].x -= s4_rain[i].speed * 0.4f; // Wind effect
                s4_rain[i].y -= s4_rain[i].speed;
                if (s4_rain[i].y < 0) {
                    s4_rain[i].y = 700; // Reset to top
                    s4_rain[i].x = float(rand() % 1300);
                }
            }
        }

        // Car driving out of the city (scaled to look like it's driving away/approaching)
        s4_carX += 3.5f;
        s4_carScale -= 0.0015f;
        if (s4_carX > 1300) {
            s4_carX = -200;
            s4_carScale = 1.0f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // 60 Frames Per Second approx
}

void keyboard(unsigned char key, int x, int y) {
    // Global App controls
    if (key == 27) exit(0); // ESC to quit
    if (key == 'n' || key == 'N') {
        currentScene++;
        if (currentScene > 4) currentScene = 1;
        resetScene(currentScene);
    }
    if (key == 'p' || key == 'P') {
        currentScene--;
        if (currentScene < 1) currentScene = 4;
        resetScene(currentScene);
    }
    if (key >= '1' && key <= '4') {
        currentScene = key - '0';
        resetScene(currentScene);
    }

    // Controls for Scene 2
    if (currentScene == 2) {
        if ((key == 'b' || key == 'B') && s2_bombState == 0) {
            s2_bombState = 1; // Launch bomb
            s2_bombX = s2_subX;
            s2_bombY = s2_subY;
        }
        if (key == 'r' || key == 'R') {
            s2_bombState = 0; // Repair house
            s2_explRadius = 0;
        }
    }
    // Controls for Scene 3
    else if (currentScene == 3) {
        float speed = 12.0f; // Make car slightly faster and responsive
        if (key == 'w' || key == 'W') {
            s3_playerCarX += speed;
            s3_wheelAngle -= speed * 2.5f;
            s3_bgOffset += speed;
            if (s3_playerCarX > 1100) s3_playerCarX = 1100; // clamp to screen
        }
        if (key == 's' || key == 'S') {
            s3_playerCarX -= speed;
            s3_wheelAngle += speed * 2.5f;
            s3_bgOffset -= speed;
            if (s3_playerCarX < 100) s3_playerCarX = 100;
        }
        if (key == 'h' || key == 'H') s3_headlightsOn = true;
        if (key == 'o' || key == 'O') s3_headlightsOn = false;
    }
    // Controls for Scene 4
    else if (currentScene == 4) {
        if (key == 't' || key == 'T') s4_rainOn = !s4_rainOn;
        if (key == 'd' || key == 'D') s4_dayNight = 0.0f; // Reset to daylight
    }
}

void mouse(int button, int state, int x, int y) {
    if (currentScene == 2) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && s2_bombState == 0) {
            s2_bombState = 1; // Left click acts same as 'B' key
            s2_bombX = s2_subX;
            s2_bombY = s2_subY;
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Computer Graphics Project - Advanced 4 Scene Animation");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Set 2D orthographic projection system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);

    // Enable Transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initData();
    resetScene(1);

    // Register Callbacks
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(16, update, 0); // Start the animation loop

    glutMainLoop();
    return 0;
}