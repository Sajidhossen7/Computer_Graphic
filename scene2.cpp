#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>

#define PI 3.14159265358979323846

int currentScene = 2; // Starting with Scene 1 as per testing, can press 1,2,3,4
int windowWidth = 1200;
int windowHeight = 700;

float global_sunY = 400.0f;
float global_brightness = 1.0f; 
bool global_isDay = true;      
bool global_isRain = false;    
float global_rainOffset = 0.0f; 
float s1_cloudOffset = 0.0f; 
float s1_birdPhase = 0.0f;

bool s1_planeActive = false;
float s1_planeX = -150.0f;
float s1_planeY = 550.0f;
float s1_bombX = 0.0f;
float s1_bombY = 0.0f;
int s1_bombState = 0; // 0=none, 1=falling, 2=exploding, 3=done
float s1_explRadius = 0.0f;
bool s1_isDestroyed = false;
float s1_debris_x[20]; float s1_debris_y[20]; float s1_debris_vx[20]; float s1_debris_vy[20];

float s2_subX = 0.0f;
float s2_subY = 60.0f;
float s2_wavePhase = 0.0f;
float s2_propAngle = 0.0f;
float s2_firePhase = 0.0f;
float s2_smokeOffset = 0.0f;
float s2_craneLift = 0.0f; 
float s2_craneDir = 1.0f;    
int s2_bombState = 0; 
float s2_bombX = 0.0f, s2_bombY = 0.0f;
float s2_targetX = 0.0f, s2_targetY = 0.0f;
float s2_explRadius = 0.0f;

float s2_bubble_x[20]; float s2_bubble_y[20]; float s2_bubble_alpha[20]; int s2_bubble_active[20];
float s2_debris_x[12]; float s2_debris_y[12]; float s2_debris_vx[12]; float s2_debris_vy[12];

float s2_b_x[12]; float s2_b_y[12]; float s2_b_width[12]; float s2_b_height[12];
float s2_b_r[12]; float s2_b_g[12]; float s2_b_b[12];
int s2_b_floors[12]; int s2_b_type[12];
float s2_b_progress[12]; int s2_b_isDestroyed[12];

float s2_boat1_x = 200.0f; float s2_boat2_x = 600.0f;

float BurntBlack_r = 0.05f; float BurntBlack_g = 0.05f; float BurntBlack_b = 0.05f;
float BrickRed_r = 0.55f; float BrickRed_g = 0.20f; float BrickRed_b = 0.15f;
float BrickBrown_r = 0.45f; float BrickBrown_g = 0.28f; float BrickBrown_b = 0.16f;
float BrickOrange_r = 0.62f; float BrickOrange_g = 0.34f; float BrickOrange_b = 0.18f;
float EmberYellow_r = 1.0f; float EmberYellow_g = 0.80f; float EmberYellow_b = 0.20f;
float EmberOrange_r = 0.95f; float EmberOrange_g = 0.45f; float EmberOrange_b = 0.10f;
float EmberRed_r = 0.80f; float EmberRed_g = 0.15f; float EmberRed_b = 0.10f;

float s3_cloudOffset = 0.0f;
float s3_treeAngle = 0.0f;
int s3_lightOn = 1;
GLfloat s3_wheelAngle = 0.0f;
GLfloat s3_carX = 0.0f;
GLfloat s3_carSpeed = 0.0f;

#define S3_MAX_DROPS 600
#define S3_MAX_BUBBLES 300
const float s3_groundY = 70.0f;
float s3_dropX[S3_MAX_DROPS];
float s3_dropY[S3_MAX_DROPS];
float s3_dropSpeed[S3_MAX_DROPS];
int s3_dropCount = 0;

float s3_bubbleX[S3_MAX_BUBBLES];
float s3_bubbleY[S3_MAX_BUBBLES];
float s3_bubbleRadius[S3_MAX_BUBBLES];
float s3_bubbleAlpha[S3_MAX_BUBBLES];
int s3_bubbleActive[S3_MAX_BUBBLES];
int s3_bubbleCount = 0;

float s4_stars_x[40]; float s4_stars_y[40]; float s4_stars_size[40]; float s4_stars_phase[40];
float s4_rain_x[200]; float s4_rain_y[200]; float s4_rain_speed[200]; float s4_rain_len[200];
float s4_buildings_x[12]; float s4_buildings_w[12]; float s4_buildings_h[12]; int s4_buildings_lit[12];

float s4_car1_x = -100.0f;
float s4_car2_x = 400.0f;
float s4_car3_x = 900.0f;
void drawBeautifulHouse(float x, float y, float scale, float b = 1.0f) {
    glPushMatrix(); 
    glTranslatef(x, y, 0); 
    glScalef(scale, scale, 1);
    
    glColor3f(0.85f * b, 0.82f * b, 0.75f * b); glBegin(GL_QUADS);
    glVertex2f(-45, 0);
    glVertex2f(45, 0);
    glVertex2f(45, 50);
    glVertex2f(-45, 50);
    glEnd();
    glColor3f(0.5f * b, 0.5f * b, 0.5f * b); glBegin(GL_QUADS);
    glVertex2f(-50, -5);
    glVertex2f(50, -5);
    glVertex2f(50, 0);
    glVertex2f(-50, 0);
    glEnd(); 
    
    glColor3f(0.3f * b, 0.15f * b, 0.05f * b); glBegin(GL_QUADS);
    glVertex2f(-12, 0);
    glVertex2f(12, 0);
    glVertex2f(12, 35);
    glVertex2f(-12, 35);
    glEnd();
    glColor3f(0.9f * b, 0.8f * b, 0.1f * b); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((8) + (3) * cos(_theta), (17) + (3) * sin(_theta));
    }
    glEnd(); 
    
    glColor3f(0.4f * b, 0.7f * b, 0.9f * b);
    glBegin(GL_QUADS);
    glVertex2f(-35, 15);
    glVertex2f(-18, 15);
    glVertex2f(-18, 35);
    glVertex2f(-35, 35);
    glEnd(); glBegin(GL_QUADS);
    glVertex2f(18, 15);
    glVertex2f(35, 15);
    glVertex2f(35, 35);
    glVertex2f(18, 35);
    glEnd();
    glColor3f(0.2f * b, 0.1f * b, 0.05f * b); glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-26.5f, 15); glVertex2f(-26.5f, 35); 
    glVertex2f(-35, 25); glVertex2f(-18, 25);
    glVertex2f(26.5f, 15); glVertex2f(26.5f, 35); 
    glVertex2f(18, 25); glVertex2f(35, 25);
    glEnd(); 
    glLineWidth(1.0f);
    
    glColor3f(0.4f * b, 0.15f * b, 0.15f * b); glBegin(GL_QUADS);
    glVertex2f(25, 40);
    glVertex2f(35, 40);
    glVertex2f(35, 75);
    glVertex2f(25, 75);
    glEnd(); 
    glColor3f(0.2f * b, 0.2f * b, 0.2f * b); glBegin(GL_QUADS);
    glVertex2f(22, 75);
    glVertex2f(38, 75);
    glVertex2f(38, 80);
    glVertex2f(22, 80);
    glEnd(); 
    glColor3f(0.6f * b, 0.25f * b, 0.2f * b); glBegin(GL_TRIANGLES);
    glVertex2f(-55, 50);
    glVertex2f(55, 50);
    glVertex2f(0, 95);
    glEnd(); 
    
    glPopMatrix();
}

void drawAirplane(float x, float y, float scale, float b = 1.0f) {
    glPushMatrix(); 
    glTranslatef(x, y, 0); 
    glScalef(scale, scale, 1);
    
    glColor3f(0.2f * b, 0.25f * b, 0.2f * b); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((0) + (50) * cos(_theta), (0) + (12) * sin(_theta));
    }
    glEnd();
    
    glColor3f(0.15f * b, 0.2f * b, 0.15f * b); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-45, 5);
    glVertex2f(-35, 5);
    glVertex2f(-45, 25);
    glEnd();
    
    glColor3f(0.15f * b, 0.2f * b, 0.15f * b); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-20, -5);
    glVertex2f(10, -5);
    glVertex2f(-10, -25);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(-15, 5);
    glVertex2f(15, 5);
    glVertex2f(0, 30);
    glEnd();
    
    glColor3f(0.1f * b, 0.5f * b, 0.8f * b); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((25) + (12) * cos(_theta), (5) + (5) * sin(_theta));
    }
    glEnd();
    
    glPopMatrix();
}

void drawMountainWithTrees(float cx, float cy, float rx, float ry, float ambient) {
    glColor3f(0.30f * ambient, 0.52f * ambient, 0.22f * ambient); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((cx) + (rx) * cos(_theta), (cy) + (ry) * sin(_theta));
    }
    glEnd();
    
    glColor3f(0.05f * ambient, 0.20f * ambient, 0.08f * ambient); 
    glPointSize(3.0f); 
    srand((int)(cx + cy)); 
    glBegin(GL_POINTS);
    for(int i = 0; i < 1200; i++) { 
        float px = cx - rx + (rand() % (int)(rx * 2)); 
        float py = cy - ry + (rand() % (int)(ry * 2));
        float dx = (px - cx) / rx; 
        float dy = (py - cy) / ry;
        if (dx*dx + dy*dy <= 0.90f) glVertex2f(px, py);     
    }
    glEnd();
}

void drawCloud(float cx, float cy, float scale, float ambient) {
    glPushMatrix(); 
    glTranslatef(cx, cy, 0); 
    glScalef(scale, scale, 1.0f);
    glEnable(GL_BLEND);
    glColor4f(0.85f * ambient, 0.87f * ambient, 0.90f * ambient, ambient);
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((0) + (30.0f) * cos(_theta), (-5.0f) + (30.0f) * sin(_theta));
    }
    glEnd(); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((30.0f) + (36.0f) * cos(_theta), (0) + (36.0f) * sin(_theta));
    }
    glEnd(); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((65.0f) + (26.0f) * cos(_theta), (-5.0f) + (26.0f) * sin(_theta));
    }
    glEnd(); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((20.0f) + (24.0f) * cos(_theta), (-15.0f) + (24.0f) * sin(_theta));
    }
    glEnd(); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((50.0f) + (22.0f) * cos(_theta), (-15.0f) + (22.0f) * sin(_theta));
    }
    glEnd();
    
    glColor4f(1.0f * ambient, 1.0f * ambient, 1.0f * ambient, ambient);
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((0) + (26.0f) * cos(_theta), (0) + (26.0f) * sin(_theta));
    }
    glEnd(); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((30.0f) + (32.0f) * cos(_theta), (8.0f) + (32.0f) * sin(_theta));
    }
    glEnd(); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((62.0f) + (22.0f) * cos(_theta), (0) + (22.0f) * sin(_theta));
    }
    glEnd();
    glDisable(GL_BLEND); 
    glPopMatrix();
}

void drawPaddyField(float x, float y, float width, float height, float ambient) {
    glColor3f(0.40f * ambient, 0.65f * ambient, 0.15f * ambient); 
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    glColor3f(0.25f * ambient, 0.45f * ambient, 0.10f * ambient); 
    glLineWidth(2.0f);
    glBegin(GL_LINES); 
    for(float i = x + 10; i < x + width; i += 25) { 
        glVertex2f(i, y); 
        glVertex2f(i + 30, y + height); 
    } 
    glEnd();
    glLineWidth(1.0f); 
    
    glColor3f(0.55f * ambient, 0.80f * ambient, 0.15f * ambient); 
    glBegin(GL_TRIANGLES);
    srand((int)(x + y));
    for(int i = 0; i < 200; i++) { 
        float px = x + (rand() % (int)width); 
        float py = y + (rand() % (int)height); 
        glVertex2f(px - 2, py); 
        glVertex2f(px + 2, py); 
        glVertex2f(px, py + 8); 
    }
    glEnd();
}

void drawTree(float x, float y, float scale, float b = 1.0f) {
    glPushMatrix(); 
    glTranslatef(x, y, 0); 
    glScalef(scale, scale, 1);
    
    glColor3f(0.35f * b, 0.20f * b, 0.10f * b); 
    glBegin(GL_QUADS);
    glVertex2f(-10, 0);
    glVertex2f(10, 0);
    glVertex2f(10, 60);
    glVertex2f(-10, 60);
    glEnd();
    glColor3f(0.1f * b, 0.4f * b, 0.1f * b); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((-20) + (25) * cos(_theta), (60) + (25) * sin(_theta));
    }
    glEnd();
    glColor3f(0.15f * b, 0.45f * b, 0.15f * b); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((20) + (25) * cos(_theta), (60) + (25) * sin(_theta));
    }
    glEnd();
    glColor3f(0.2f * b, 0.5f * b, 0.2f * b); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((0) + (30) * cos(_theta), (85) + (30) * sin(_theta));
    }
    glEnd();
    glColor3f(0.25f * b, 0.55f * b, 0.25f * b); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((0) + (35) * cos(_theta), (50) + (35) * sin(_theta));
    }
    glEnd(); 
    
    glPopMatrix();
}


void drawCityTrees(float ambient) {
    drawTree(120.0f, 150.0f, 0.75f, ambient);
    drawTree(285.0f, 145.0f, 0.65f, ambient);
    drawTree(1020.0f, 145.0f, 0.70f, ambient);
    drawTree(1190.0f, 150.0f, 0.80f, ambient);
    drawTree(1500.0f, 145.0f, 0.75f, ambient);
}





void buildingNormal(int b_idx, float ambient, float brightness) {
    glColor3f(s2_b_r[b_idx] * ambient, s2_b_g[b_idx] * ambient, s2_b_b[b_idx] * ambient); 
    glBegin(GL_QUADS);
    glVertex2f(s2_b_x[b_idx], s2_b_y[b_idx]);
    glVertex2f(s2_b_x[b_idx] + s2_b_width[b_idx], s2_b_y[b_idx]);
    glVertex2f(s2_b_x[b_idx] + s2_b_width[b_idx], s2_b_y[b_idx] + s2_b_height[b_idx] * s2_b_progress[b_idx]);
    glVertex2f(s2_b_x[b_idx], s2_b_y[b_idx] + s2_b_height[b_idx] * s2_b_progress[b_idx]);
    glEnd();
    
    glColor3f(s2_b_r[b_idx] * 0.75f * ambient, s2_b_g[b_idx] * 0.75f * ambient, s2_b_b[b_idx] * 0.75f * ambient); 
    glBegin(GL_QUADS);
    glVertex2f(s2_b_x[b_idx] - 3, s2_b_y[b_idx] + s2_b_height[b_idx] * s2_b_progress[b_idx]);
    glVertex2f(s2_b_x[b_idx] - 3 + s2_b_width[b_idx] + 6, s2_b_y[b_idx] + s2_b_height[b_idx] * s2_b_progress[b_idx]);
    glVertex2f(s2_b_x[b_idx] - 3 + s2_b_width[b_idx] + 6, s2_b_y[b_idx] + s2_b_height[b_idx] * s2_b_progress[b_idx] + 8);
    glVertex2f(s2_b_x[b_idx] - 3, s2_b_y[b_idx] + s2_b_height[b_idx] * s2_b_progress[b_idx] + 8);
    glEnd();
    
    float floorHeight = s2_b_height[b_idx] / s2_b_floors[b_idx]; 
    int drawFloors = s2_b_floors[b_idx] * s2_b_progress[b_idx];
    bool isNight = (brightness < 0.4f); 
    srand((int)(s2_b_x[b_idx] * 100)); 
    
    for (int floor = 0; floor < drawFloors; floor++) {
        float wy = s2_b_y[b_idx] + floor * floorHeight + floorHeight * 0.30f;
        int columns = (int)(s2_b_width[b_idx] / 35.0f); 
        if (columns < 1) columns = 1; 
        float spacing = s2_b_width[b_idx] / (columns + 1);
        
        for (int col = 0; col < columns; col++) {
            float wx = s2_b_x[b_idx] + spacing * (col + 1) - 7;
            if (isNight && (rand() % 100 > 60)) 
                glColor3f(1.0f, 0.85f, 0.3f); 
            else 
                glColor3f(0.78f * ambient, 0.88f * ambient, 0.92f * ambient);
            glBegin(GL_QUADS);
    glVertex2f(wx, wy);
    glVertex2f(wx + 14, wy);
    glVertex2f(wx + 14, wy + floorHeight * 0.35f);
    glVertex2f(wx, wy + floorHeight * 0.35f);
    glEnd();
        }
    }
}

void buildingGlass(int b_idx, float ambient, float brightness) {
    glColor3f(s2_b_r[b_idx] * ambient, s2_b_g[b_idx] * ambient, s2_b_b[b_idx] * ambient); 
    glBegin(GL_QUADS);
    glVertex2f(s2_b_x[b_idx], s2_b_y[b_idx]);
    glVertex2f(s2_b_x[b_idx] + s2_b_width[b_idx], s2_b_y[b_idx]);
    glVertex2f(s2_b_x[b_idx] + s2_b_width[b_idx], s2_b_y[b_idx] + s2_b_height[b_idx] * s2_b_progress[b_idx]);
    glVertex2f(s2_b_x[b_idx], s2_b_y[b_idx] + s2_b_height[b_idx] * s2_b_progress[b_idx]);
    glEnd();
    
    int columns = (int)(s2_b_width[b_idx] / 25.0f); 
    glColor3f(0.20f * ambient, 0.38f * ambient, 0.48f * ambient);
    for (int i = 1; i < columns; i++) { 
        float x = s2_b_x[b_idx] + i * s2_b_width[b_idx] / columns; 
        glBegin(GL_LINES);
    glVertex2f(x, s2_b_y[b_idx]);
    glVertex2f(x, s2_b_y[b_idx] + s2_b_height[b_idx] * s2_b_progress[b_idx]);
    glEnd(); 
    }
    
    int drawFloors = s2_b_floors[b_idx] * s2_b_progress[b_idx]; 
    for (int i = 1; i < drawFloors; i++) { 
        float y = s2_b_y[b_idx] + i * s2_b_height[b_idx] / s2_b_floors[b_idx]; 
        glBegin(GL_LINES);
    glVertex2f(s2_b_x[b_idx], y);
    glVertex2f(s2_b_x[b_idx] + s2_b_width[b_idx], y);
    glEnd(); 
    }
    
    bool isNight = (brightness < 0.4f); 
    srand((int)(s2_b_x[b_idx] * 50));
    
    for (int floor = 0; floor < drawFloors; floor++) {
        float y = s2_b_y[b_idx] + floor * s2_b_height[b_idx] / s2_b_floors[b_idx] + 10;
        for (int col = 0; col < columns; col++) {
            float x = s2_b_x[b_idx] + col * s2_b_width[b_idx] / columns + 5;
            if (isNight && (rand() % 100 > 60)) 
                glColor3f(1.0f, 0.90f, 0.4f); 
            else 
                glColor3f(0.75f * ambient, 0.90f * ambient, 0.95f * ambient);
            glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + s2_b_width[b_idx] / columns - 10, y);
    glVertex2f(x + s2_b_width[b_idx] / columns - 10, y + s2_b_height[b_idx] / s2_b_floors[b_idx] - 18);
    glVertex2f(x, y + s2_b_height[b_idx] / s2_b_floors[b_idx] - 18);
    glEnd();
        }
    }
}

void drawHalfConstructedBuilding(int b_idx, float ambient, float brightness) {
    float currentHeight = s2_b_height[b_idx] * s2_b_progress[b_idx]; 
    glColor3f(s2_b_r[b_idx] * ambient, s2_b_g[b_idx] * ambient, s2_b_b[b_idx] * ambient); 
    glBegin(GL_QUADS);
    glVertex2f(s2_b_x[b_idx], s2_b_y[b_idx]);
    glVertex2f(s2_b_x[b_idx] + s2_b_width[b_idx], s2_b_y[b_idx]);
    glVertex2f(s2_b_x[b_idx] + s2_b_width[b_idx], s2_b_y[b_idx] + currentHeight);
    glVertex2f(s2_b_x[b_idx], s2_b_y[b_idx] + currentHeight);
    glEnd();
    
    float floorHeight = s2_b_height[b_idx] / s2_b_floors[b_idx]; 
    glColor3f(0.30f * ambient, 0.32f * ambient, 0.34f * ambient); 
    int completedFloors = (int)(currentHeight / floorHeight);
    
    for (int i = 0; i <= completedFloors; i++) { 
        float y = s2_b_y[b_idx] + i * floorHeight; 
        if (y <= s2_b_y[b_idx] + currentHeight) {
            glBegin(GL_QUADS);
    glVertex2f(s2_b_x[b_idx] - 4, y);
    glVertex2f(s2_b_x[b_idx] - 4 + s2_b_width[b_idx] + 8, y);
    glVertex2f(s2_b_x[b_idx] - 4 + s2_b_width[b_idx] + 8, y + 5);
    glVertex2f(s2_b_x[b_idx] - 4, y + 5);
    glEnd(); 
        }
    }
    
    bool isNight = (brightness < 0.4f); 
    srand((int)(s2_b_x[b_idx] * 200));
    
    for (int floor = 0; floor < completedFloors; floor++) {
        float y = s2_b_y[b_idx] + floor * floorHeight + 10; 
        int columns = (int)(s2_b_width[b_idx] / 30.0f); 
        if (columns < 1) columns = 1; 
        float windowWidth = s2_b_width[b_idx] / (columns + 1);
        
        for (int c = 0; c < columns; c++) {
            float x = s2_b_x[b_idx] + windowWidth * (c + 1) - 6;
            if (isNight && (rand() % 100 > 60)) 
                glColor3f(1.0f, 0.85f, 0.3f); 
            else 
                glColor3f(0.75f * ambient, 0.88f * ambient, 0.93f * ambient);
            glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 12, y);
    glVertex2f(x + 12, y + floorHeight * 0.35f);
    glVertex2f(x, y + floorHeight * 0.35f);
    glEnd();
        }
    }
    
    if (s2_b_progress[b_idx] < 1.0f) {
        glColor3f(0.25f * ambient, 0.28f * ambient, 0.30f * ambient); 
        float topY = s2_b_y[b_idx] + currentHeight; 
        glLineWidth(2.0f);
        for (float x = s2_b_x[b_idx] + 8; x < s2_b_x[b_idx] + s2_b_width[b_idx]; x += 16) {
            glBegin(GL_LINES);
    glVertex2f(x, topY);
    glVertex2f(x, topY + 12);
    glEnd(); 
        }
        glLineWidth(1.0f);
    }
}

void drawSteelSuperstructure(float x, float y, float scaleX, float scaleY, float ambient) {
    glPushMatrix(); 
    glTranslatef(x, y, 0.0f); 
    glScalef(scaleX, scaleY, 1.0f);
    
    glColor3f(0.55f * ambient, 0.65f * ambient, 0.72f * ambient);
    glBegin(GL_POLYGON); 
    glVertex2f(18, 0); glVertex2f(40, 0); glVertex2f(40, 230); glVertex2f(18, 230); 
    glEnd();
    
    glBegin(GL_POLYGON); 
    glVertex2f(54, 0); glVertex2f(78, 0); glVertex2f(78, 255); glVertex2f(54, 255); 
    glEnd();
    
    glColor3f(0.60f * ambient, 0.42f * ambient, 0.22f * ambient);
    glBegin(GL_POLYGON); 
    glVertex2f(12, 4); glVertex2f(38, 4); glVertex2f(38, 22); glVertex2f(12, 22); 
    glEnd();
    
    glBegin(GL_POLYGON); 
    glVertex2f(48, 4); glVertex2f(70, 4); glVertex2f(70, 16); glVertex2f(48, 16); 
    glEnd();
    
    glColor3f(0.38f * ambient, 0.45f * ambient, 0.50f * ambient);
    glBegin(GL_QUADS);
    glVertex2f(2, 0); glVertex2f(8, 0); glVertex2f(8, 260); glVertex2f(2, 260);
    glVertex2f(42, 0); glVertex2f(48, 0); glVertex2f(48, 260); glVertex2f(42, 260);
    glVertex2f(82, 0); glVertex2f(88, 0); glVertex2f(88, 295); glVertex2f(82, 295);
    glVertex2f(118, 20); glVertex2f(124, 20); glVertex2f(124, 165); glVertex2f(118, 165);
    for (int i = 1; i <= 8; i++) { 
        float yPos = i * 28.0f; 
        glVertex2f(2, yPos); glVertex2f(88, yPos); glVertex2f(88, yPos + 6); glVertex2f(2, yPos + 6); 
    }
    glVertex2f(88, 30); glVertex2f(128, 30); glVertex2f(128, 36); glVertex2f(88, 36);
    glEnd();
    
    glLineWidth(4.0f); 
    glColor3f(0.32f * ambient, 0.38f * ambient, 0.44f * ambient);
    glBegin(GL_LINES); 
    glVertex2f(48, 6); glVertex2f(82, 28); 
    glVertex2f(48, 34); glVertex2f(82, 56); 
    glVertex2f(48, 62); glVertex2f(82, 84); 
    glEnd();
    
    glLineWidth(1.0f); 
    glColor3f(0.20f * ambient, 0.25f * ambient, 0.28f * ambient);
    glBegin(GL_LINES); 
    glVertex2f(2, 0); glVertex2f(2, 260); 
    glVertex2f(42, 0); glVertex2f(42, 260); 
    glVertex2f(82, 0); glVertex2f(82, 295); 
    glVertex2f(118, 20); glVertex2f(118, 165); 
    glEnd();
    
    glPopMatrix();
}

void drawCraneModel(float ambient) {
    glLineWidth(1.6f); 
    glColor3f(0.16f * ambient, 0.22f * ambient, 0.28f * ambient);
    glBegin(GL_QUADS); 
    glVertex2f(-22, 0); glVertex2f(22, 0); glVertex2f(22, 8); glVertex2f(-22, 8); 
    glEnd();
    
    glBegin(GL_LINES);
    glVertex2f(-20, 8); glVertex2f(-10, 35); 
    glVertex2f(20, 8); glVertex2f(10, 35);
    glVertex2f(-20, 8); glVertex2f(10, 8); 
    glVertex2f(20, 8); glVertex2f(-10, 8);
    glVertex2f(-10, 0); glVertex2f(-10, 400); 
    glVertex2f(10, 0); glVertex2f(10, 400);
    for (float y = 0; y < 400; y += 50) { 
        glVertex2f(-10, y + 50); glVertex2f(10, y + 50); 
        glVertex2f(-10, y); glVertex2f(10, y + 50); 
        glVertex2f(10, y); glVertex2f(-10, y + 50); 
    }
    glEnd();
    
    glColor3f(0.18f * ambient, 0.24f * ambient, 0.30f * ambient); 
    glBegin(GL_POLYGON); 
    glVertex2f(2, 375); glVertex2f(24, 375); glVertex2f(29, 387); glVertex2f(24, 397); glVertex2f(2, 397); 
    glEnd();
    
    glColor3f(0.72f * ambient, 0.86f * ambient, 0.94f * ambient); 
    glBegin(GL_POLYGON); 
    glVertex2f(9, 382); glVertex2f(23, 382); glVertex2f(26, 387); glVertex2f(23, 393); glVertex2f(9, 393); 
    glEnd();
    
    glColor3f(0.16f * ambient, 0.22f * ambient, 0.28f * ambient);
    glBegin(GL_LINES);
    glVertex2f(-10, 400); glVertex2f(0, 465); 
    glVertex2f(10, 400); glVertex2f(0, 465); 
    glVertex2f(0, 400); glVertex2f(0, 465); 
    glVertex2f(-5, 432); glVertex2f(5, 432);
    
    glVertex2f(-10, 400); glVertex2f(5, 432); 
    glVertex2f(10, 400); glVertex2f(-5, 432); 
    glVertex2f(-75, 416); glVertex2f(180, 416); 
    glVertex2f(-75, 400); glVertex2f(180, 400);
    
    glVertex2f(-75, 400); glVertex2f(-75, 416); 
    glVertex2f(180, 400); glVertex2f(180, 416);
    
    for (float x = -75; x < 180; x += 25) { 
        float nx = std::min(x + 25.0f, 180.0f); 
        glVertex2f(nx, 400); glVertex2f(nx, 416); 
        glVertex2f(x, 400); glVertex2f(nx, 416); 
        glVertex2f(x, 416); glVertex2f(nx, 400); 
    }
    glEnd();
    
    glColor3f(0.10f * ambient, 0.14f * ambient, 0.18f * ambient); 
    glBegin(GL_LINES); 
    glVertex2f(0, 465); glVertex2f(-70, 416); 
    glVertex2f(0, 465); glVertex2f(-45, 416); 
    glVertex2f(0, 465); glVertex2f(90, 416); 
    glVertex2f(0, 465); glVertex2f(160, 416); 
    glEnd();
    
    glColor3f(0.38f * ambient, 0.44f * ambient, 0.48f * ambient); 
    glBegin(GL_QUADS); 
    glVertex2f(-71, 380); glVertex2f(-39, 380); 
    glVertex2f(-39, 400); glVertex2f(-71, 400); 
    glEnd();
    
    glColor3f(0.22f * ambient, 0.28f * ambient, 0.34f * ambient); 
    glBegin(GL_QUADS); 
    glVertex2f(106, 395); glVertex2f(124, 395); 
    glVertex2f(124, 400); glVertex2f(106, 400); 
    glEnd();
    
    float liftTopY = 395.0f - s2_craneLift; 
    glColor3f(0.10f * ambient, 0.14f * ambient, 0.18f * ambient);
    glBegin(GL_LINES); 
    glVertex2f(112, 395); glVertex2f(112, liftTopY); 
    glVertex2f(118, 395); glVertex2f(118, liftTopY); 
    glVertex2f(115, liftTopY - 6); glVertex2f(92, liftTopY - 28); 
    glVertex2f(115, liftTopY - 6); glVertex2f(138, liftTopY - 28); 
    glEnd();
    
    glColor3f(0.50f * ambient, 0.56f * ambient, 0.60f * ambient); 
    glBegin(GL_QUADS);
    glVertex2f(87, liftTopY - 36);
    glVertex2f(87 + 56, liftTopY - 36);
    glVertex2f(87 + 56, liftTopY - 36 + 8);
    glVertex2f(87, liftTopY - 36 + 8);
    glEnd(); 
    glLineWidth(1.0f);
}

void drawTranslatedCrane(float x, float y, float scaleX, float scaleY, float ambient) {
    glPushMatrix(); 
    glTranslatef(x, y, 0.0f); 
    glScalef(scaleX, scaleY, 1.0f); 
    drawCraneModel(ambient); 
    glPopMatrix();
}

void drawMaterials(float x, float y, float ambient) {
    glColor3f(0.72f * ambient, 0.40f * ambient, 0.20f * ambient); 
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 45, y);
    glVertex2f(x + 45, y + 12);
    glVertex2f(x, y + 12);
    glEnd(); 
    glBegin(GL_QUADS);
    glVertex2f(x + 5, y + 12);
    glVertex2f(x + 5 + 45, y + 12);
    glVertex2f(x + 5 + 45, y + 12 + 12);
    glVertex2f(x + 5, y + 12 + 12);
    glEnd(); 
    glBegin(GL_QUADS);
    glVertex2f(x - 5, y + 24);
    glVertex2f(x - 5 + 45, y + 24);
    glVertex2f(x - 5 + 45, y + 24 + 12);
    glVertex2f(x - 5, y + 24 + 12);
    glEnd();
    
    glColor3f(0.35f * ambient, 0.37f * ambient, 0.38f * ambient); 
    for (int i = 0; i < 5; i++) {
        glBegin(GL_LINES);
    glVertex2f(x + i * 8, y + 40);
    glVertex2f(x + i * 8 + 20, y + 40);
    glEnd();
    }
}

void drawBarrier(float x, float y, float ambient) {
    glColor3f(0.95f * ambient, 0.65f * ambient, 0.05f * ambient); 
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 75, y);
    glVertex2f(x + 75, y + 8);
    glVertex2f(x, y + 8);
    glEnd();
    
    glColor3f(0.25f * ambient, 0.25f * ambient, 0.25f * ambient); 
    glBegin(GL_QUADS);
    glVertex2f(x + 5, y - 25);
    glVertex2f(x + 5 + 7, y - 25);
    glVertex2f(x + 5 + 7, y - 25 + 25);
    glVertex2f(x + 5, y - 25 + 25);
    glEnd(); 
    glBegin(GL_QUADS);
    glVertex2f(x + 63, y - 25);
    glVertex2f(x + 63 + 7, y - 25);
    glVertex2f(x + 63 + 7, y - 25 + 25);
    glVertex2f(x + 63, y - 25 + 25);
    glEnd();
    
    glColor3f(0.15f * ambient, 0.15f * ambient, 0.15f * ambient); 
    glBegin(GL_LINES);
    glVertex2f(x + 5, y);
    glVertex2f(x + 25, y + 8);
    glEnd(); 
    glBegin(GL_LINES);
    glVertex2f(x + 30, y);
    glVertex2f(x + 50, y + 8);
    glEnd(); 
    glBegin(GL_LINES);
    glVertex2f(x + 55, y);
    glVertex2f(x + 70, y + 8);
    glEnd();
}

void drawConstructionSign(float x, float y, float ambient) {
    glColor3f(0.25f * ambient, 0.25f * ambient, 0.25f * ambient); 
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 6, y);
    glVertex2f(x + 6, y + 70);
    glVertex2f(x, y + 70);
    glEnd(); 
    glBegin(GL_QUADS);
    glVertex2f(x + 74, y);
    glVertex2f(x + 74 + 6, y);
    glVertex2f(x + 74 + 6, y + 70);
    glVertex2f(x + 74, y + 70);
    glEnd();
    
    glColor3f(0.95f * ambient, 0.65f * ambient, 0.05f * ambient); 
    glBegin(GL_QUADS);
    glVertex2f(x - 10, y + 50);
    glVertex2f(x - 10 + 100, y + 50);
    glVertex2f(x - 10 + 100, y + 50 + 45);
    glVertex2f(x - 10, y + 50 + 45);
    glEnd();
    
    glColor3f(0.15f * ambient, 0.15f * ambient, 0.15f * ambient); 
    glBegin(GL_QUADS);
    glVertex2f(x + 35, y + 65);
    glVertex2f(x + 35 + 10, y + 65);
    glVertex2f(x + 35 + 10, y + 65 + 20);
    glVertex2f(x + 35, y + 65 + 20);
    glEnd(); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((x + 40) + (5) * cos(_theta), (y + 58) + (5) * sin(_theta));
    }
    glEnd();
}

void drawShip(float x, float y, float scale, float ambient, float brightness) {
    glPushMatrix(); 
    glTranslatef(x, y, 0); 
    glScalef(scale, scale, 1.0f);
    
    glColor3f(0.40f * ambient, 0.15f * ambient, 0.15f * ambient); 
    glBegin(GL_POLYGON); 
    glVertex2f(-60, 0); glVertex2f(60, 0); glVertex2f(80, 20); glVertex2f(-70, 20); 
    glEnd();
    
    glColor3f(0.80f * ambient, 0.80f * ambient, 0.85f * ambient); 
    glBegin(GL_QUADS);
    glVertex2f(-30, 20);
    glVertex2f(20, 20);
    glVertex2f(20, 45);
    glVertex2f(-30, 45);
    glEnd();
    
    if(brightness < 0.4f) glColor3f(1.0f, 0.9f, 0.4f); 
    else glColor3f(0.2f * ambient, 0.4f * ambient, 0.6f * ambient);
    glBegin(GL_QUADS);
    glVertex2f(-20, 28);
    glVertex2f(-5, 28);
    glVertex2f(-5, 38);
    glVertex2f(-20, 38);
    glEnd(); 
    glBegin(GL_QUADS);
    glVertex2f(0, 28);
    glVertex2f(15, 28);
    glVertex2f(15, 38);
    glVertex2f(0, 38);
    glEnd();
    
    glColor3f(0.2f * ambient, 0.2f * ambient, 0.2f * ambient); 
    glBegin(GL_QUADS);
    glVertex2f(25, 20);
    glVertex2f(35, 20);
    glVertex2f(35, 55);
    glVertex2f(25, 55);
    glEnd();
    
    glPopMatrix();
}

void drawBrick(float x, float y, float w, float h, float rotation, float r, float g, float b, float ambient) {
    float hw = w * 0.5f; 
    float hh = h * 0.5f;
    glPushMatrix(); 
    glTranslatef(x, y, 0.0f); 
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    
    glColor3f(r * ambient, g * ambient, b * ambient); 
    glBegin(GL_QUADS); 
    glVertex2f(-hw, -hh); glVertex2f(hw, -hh); glVertex2f(hw, hh); glVertex2f(-hw, hh); 
    glEnd();
    
    glColor3f(r * 0.55f * ambient, g * 0.55f * ambient, b * 0.55f * ambient); 
    glLineWidth(1.0f); 
    glBegin(GL_LINE_LOOP); 
    glVertex2f(-hw, -hh); glVertex2f(hw, -hh); glVertex2f(hw, hh); glVertex2f(-hw, hh); 
    glEnd();
    
    glPopMatrix();
}

void drawBrickPileLarge(float x, float y, float scale, float ambient) {
    glPushMatrix(); 
    glTranslatef(x, y, 0.0f); 
    glScalef(scale, scale, 1.0f);
    drawBrick(-40.0f, 4.0f, 30.0f, 13.0f, -4.0f, BrickBrown_r, BrickBrown_g, BrickBrown_b, ambient); 
    drawBrick(-8.0f, 3.0f, 32.0f, 13.0f, 5.0f, BrickRed_r, BrickRed_g, BrickRed_b, ambient);
    drawBrick(26.0f, 5.0f, 28.0f, 12.0f, -8.0f, BrickOrange_r, BrickOrange_g, BrickOrange_b, ambient); 
    drawBrick(52.0f, 2.0f, 26.0f, 12.0f, 10.0f, BrickRed_r, BrickRed_g, BrickRed_b, ambient);
    drawBrick(-22.0f, 18.0f, 28.0f, 12.0f, 6.0f, BrickRed_r, BrickRed_g, BrickRed_b, ambient); 
    drawBrick(6.0f, 20.0f, 26.0f, 12.0f, -5.0f, BrickBrown_r, BrickBrown_g, BrickBrown_b, ambient);
    drawBrick(34.0f, 17.0f, 24.0f, 11.0f, 9.0f, BrickOrange_r, BrickOrange_g, BrickOrange_b, ambient); 
    drawBrick(-6.0f, 34.0f, 22.0f, 10.0f, -12.0f, BrickBrown_r, BrickBrown_g, BrickBrown_b, ambient); 
    drawBrick(18.0f, 33.0f, 20.0f, 9.0f, 4.0f, BrickRed_r, BrickRed_g, BrickRed_b, ambient);
    glPopMatrix();
}

void NEW1drawHouseBlack(float ambient) {
    glColor3f(55/255.0f*ambient, 55/255.0f*ambient, 62/255.0f*ambient);
    glBegin(GL_POLYGON); glVertex2f(-3.0f, -1.6f); glVertex2f(-2.1f, -1.9f); glVertex2f(-1.9f, -2.8f); glVertex2f(-2.8f, -2.7f); glEnd();
    glBegin(GL_POLYGON); glVertex2f(0.9f, -1.5f); glVertex2f(1.5f, -1.8f); glVertex2f(1.3f, -2.8f); glVertex2f(0.7f, -2.6f); glEnd();
    glBegin(GL_POLYGON); glVertex2f(1.6f, -2.2f); glVertex2f(2.3f, -2.1f); glVertex2f(2.1f, -3.3f); glVertex2f(1.4f, -3.1f); glEnd();
    
    glColor3f(40/255.0f*ambient, 38/255.0f*ambient, 48/255.0f*ambient); 
    glBegin(GL_POLYGON); glVertex2f(-1.7f, -4.8f); glVertex2f(1.7f, -4.8f); glVertex2f(1.6f, -3.0f); glVertex2f(0.1f, -3.4f); glVertex2f(-1.6f, -3.1f); glEnd();
    
    glLineWidth(2.5f); 
    glColor3f(15/255.0f*ambient, 12/255.0f*ambient, 20/255.0f*ambient); 
    glBegin(GL_LINES); glVertex2f(-1.1f, -3.1f); glVertex2f(-0.8f, -4.0f); glVertex2f(0.8f, -3.2f); glVertex2f(1.2f, -4.2f); glEnd();
    
    glColor3f(75/255.0f*ambient, 60/255.0f*ambient, 48/255.0f*ambient); 
    glBegin(GL_POLYGON); glVertex2f(-0.3f, -1.3f); glVertex2f(0.1f, -1.1f); glVertex2f(0.2f, -2.4f); glVertex2f(-0.2f, -2.5f); glEnd(); 
    glBegin(GL_POLYGON); glVertex2f(-2.4f, -2.3f); glVertex2f(-0.5f, -2.0f); glVertex2f(-0.6f, -2.3f); glVertex2f(-2.5f, -2.6f); glEnd();
    
    glColor3f(22/255.0f*ambient, 20/255.0f*ambient, 28/255.0f*ambient); 
    glBegin(GL_POLYGON); glVertex2f(-0.2f, -1.5f); glVertex2f(-3.7f, -3.7f); glVertex2f(-3.3f, -4.1f); glVertex2f(-0.4f, -2.3f); glEnd();
    
    glColor3f(18/255.0f*ambient, 16/255.0f*ambient, 24/255.0f*ambient); 
    glBegin(GL_POLYGON); glVertex2f(0.3f, -1.8f); glVertex2f(3.7f, -3.9f); glVertex2f(3.3f, -4.3f); glVertex2f(0.1f, -2.6f); glEnd();
    
    glColor3f(140/255.0f*ambient, 175/255.0f*ambient, 200/255.0f*ambient); 
    glBegin(GL_TRIANGLES); glVertex2f(-0.9f, -3.1f); glVertex2f(-0.5f, -3.1f); glVertex2f(-0.8f, -2.6f); glVertex2f(-0.3f, -3.0f); glVertex2f(-0.2f, -2.5f); glVertex2f(-0.4f, -2.7f); glVertex2f(0.2f, -3.1f); glVertex2f(0.7f, -3.1f); glVertex2f(0.3f, -2.4f); glEnd();
    
    glColor3f(30/255.0f*ambient, 28/255.0f*ambient, 36/255.0f*ambient); 
    glBegin(GL_POLYGON); glVertex2f(-5.3f, -4.2f); glVertex2f(-5.0f, -3.5f); glVertex2f(-3.0f, -3.0f); glVertex2f(-3.1f, -3.5f); glEnd(); 
    glBegin(GL_POLYGON); glVertex2f(-3.2f, -3.1f); glVertex2f(-1.5f, -2.4f); glVertex2f(-1.7f, -3.0f); glVertex2f(-3.3f, -3.6f); glEnd();
    
    glColor3f(50/255.0f*ambient, 48/255.0f*ambient, 56/255.0f*ambient); 
    glBegin(GL_POLYGON); glVertex2f(-4.6f, -4.7f); glVertex2f(-0.5f, -5.1f); glVertex2f(-0.3f, -5.6f); glVertex2f(-4.4f, -5.2f); glEnd();
    
    glColor3f(30/255.0f*ambient, 28/255.0f*ambient, 36/255.0f*ambient); 
    glBegin(GL_POLYGON); glVertex2f(1.2f, -2.4f); glVertex2f(4.3f, -3.6f); glVertex2f(4.0f, -4.2f); glVertex2f(1.0f, -2.8f); glEnd();
    
    glColor3f(35/255.0f*ambient, 35/255.0f*ambient, 40/255.0f*ambient); 
    glBegin(GL_POLYGON); glVertex2f(-4.2f, -5.3f); glVertex2f(-2.7f, -5.4f); glVertex2f(-2.8f, -5.9f); glVertex2f(-4.2f, -5.8f); glEnd(); 
    glBegin(GL_POLYGON); glVertex2f(-2.3f, -5.4f); glVertex2f(-1.1f, -5.3f); glVertex2f(-1.2f, -5.9f); glVertex2f(-2.4f, -5.9f); glEnd(); 
    glBegin(GL_POLYGON); glVertex2f(1.8f, -5.4f); glVertex2f(3.2f, -5.3f); glVertex2f(3.1f, -5.9f); glVertex2f(1.7f, -5.9f); glEnd();
    
    glColor3f(45/255.0f*ambient, 42/255.0f*ambient, 50/255.0f*ambient); 
    glBegin(GL_POLYGON); glVertex2f(2.3f, -4.5f); glVertex2f(3.7f, -4.4f); glVertex2f(3.9f, -5.1f); glVertex2f(2.4f, -5.3f); glEnd();
    
    glColor3f(230/255.0f*ambient, 230/255.0f*ambient, 225/255.0f*ambient); 
    glBegin(GL_POLYGON); glVertex2f(3.9f, -3.0f); glVertex2f(5.1f, -2.9f); glVertex2f(5.0f, -3.3f); glVertex2f(3.8f, -3.2f); glEnd(); 
    glBegin(GL_POLYGON); glVertex2f(4.1f, -3.7f); glVertex2f(5.3f, -4.0f); glVertex2f(5.1f, -4.3f); glVertex2f(4.0f, -4.0f); glEnd();
    
    glColor3f(180/255.0f*ambient, 180/255.0f*ambient, 175/255.0f*ambient); 
    glBegin(GL_TRIANGLES); 
    glVertex2f(-4.8f, -5.6f); glVertex2f(-4.5f, -5.3f); glVertex2f(-4.4f, -5.7f); 
    glVertex2f(-0.2f, -5.8f); glVertex2f(0.1f, -5.4f); glVertex2f(0.3f, -5.9f); 
    glVertex2f(3.5f, -5.7f); glVertex2f(3.8f, -5.3f); glVertex2f(4.0f, -5.8f); 
    glEnd();
}

void drawRuinedHouseBlackNEW(float x, float y, float scale, float ambient) {
    glPushMatrix(); 
    glTranslatef(x, y + 50.0f, 0.0f); 
    glScalef(scale, scale, 1.0f); 
    NEW1drawHouseBlack(ambient); 
    glPopMatrix();
}

void drawFire(float x, float y, float scale) {
    float flick = 0.9f + 0.2f * sin(s2_firePhase * 10.0f); 
    float flick2 = 0.9f + 0.2f * cos(s2_firePhase * 15.0f);
    glPushMatrix(); 
    glTranslatef(x, y, 0.0f); 
    glScalef(scale, scale, 1.0f);
    
    glColor3f(EmberRed_r, EmberRed_g, EmberRed_b); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-16.0f, 0.0f);
    glVertex2f(16.0f, 0.0f);
    glVertex2f(0.0f, 55.0f * flick);
    glEnd(); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-10.0f, 0.0f);
    glVertex2f(6.0f, 0.0f);
    glVertex2f(-4.0f, 40.0f * flick2);
    glEnd(); 
    
    glColor3f(EmberOrange_r, EmberOrange_g, EmberOrange_b); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-10.0f, 0.0f);
    glVertex2f(10.0f, 0.0f);
    glVertex2f(0.0f, 40.0f * flick2);
    glEnd();
    
    glColor3f(EmberYellow_r, EmberYellow_g, EmberYellow_b); 
    glBegin(GL_TRIANGLES);
    glVertex2f(-5.0f, 0.0f);
    glVertex2f(5.0f, 0.0f);
    glVertex2f(0.0f, 24.0f * flick);
    glEnd();
    
    glPopMatrix();
}

void drawSmoke(float x, float y, float scale) {
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glColor4f(0.35f, 0.35f, 0.35f, 0.45f);
    
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 12; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(12);
        glVertex2f((x) + (9.0f * scale) * cos(_theta), (y + (10.0f * scale)) + (9.0f * scale) * sin(_theta));
    }
    glEnd(); 
    float midX = x + 2.0f; 
    float midY = y + s2_smokeOffset + (28.0f * scale);
    
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 12; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(12);
        glVertex2f((midX) + (14.0f * scale) * cos(_theta), (midY) + (14.0f * scale) * sin(_theta));
    }
    glEnd(); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 12; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(12);
        glVertex2f((midX - (10.0f * scale)) + (11.0f * scale) * cos(_theta), (midY + (2.0f * scale)) + (11.0f * scale) * sin(_theta));
    }
    glEnd(); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 12; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(12);
        glVertex2f((midX + (10.0f * scale)) + (11.0f * scale) * cos(_theta), (midY + (2.0f * scale)) + (11.0f * scale) * sin(_theta));
    }
    glEnd();
    
    float topX = x + 4.0f; 
    float topY = y + (s2_smokeOffset * 1.5f) + (50.0f * scale); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 12; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(12);
        glVertex2f((topX) + (18.0f * scale) * cos(_theta), (topY) + (18.0f * scale) * sin(_theta));
    }
    glEnd(); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 12; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(12);
        glVertex2f((topX - (13.0f * scale)) + (14.0f * scale) * cos(_theta), (topY + (3.0f * scale)) + (14.0f * scale) * sin(_theta));
    }
    glEnd(); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 12; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(12);
        glVertex2f((topX + (13.0f * scale)) + (14.0f * scale) * cos(_theta), (topY + (3.0f * scale)) + (14.0f * scale) * sin(_theta));
    }
    glEnd(); 
    
    glDisable(GL_BLEND);
}

void drawUniversalRain() {
    if (global_isRain) {
        glEnable(GL_BLEND); 
        glColor4f(0.7f, 0.8f, 0.9f, 0.6f); 
        glLineWidth(1.5f);
        glBegin(GL_LINES); 
        srand(12345); 
        for(int i = 0; i < 350; i++) {
            float rx = (rand() % 1400) - 100; 
            float ry = (rand() % 700);
            int finalY = ((int)(ry - global_rainOffset)) % 700; 
            if (finalY < 0) finalY += 700;
            glVertex2f(rx, finalY); 
            glVertex2f(rx - 8, finalY + 20); 
        }
        glEnd(); 
        glLineWidth(1.0f); 
        glDisable(GL_BLEND);
    }
}


void circle_Monsoon(float radius, float xc, float yc, float r, float g, float b) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 200; i++) {
        glColor3ub(r, g, b);
        float pi = 3.1416;
        float A = (i * 2 * pi) / 200;
        float rad = radius;
        float x = rad * cos(A);
        float y = rad * sin(A);
        glVertex2f(x + xc, y + yc);
    }
    glEnd();
}

void drawGround_Monsoon() {
    glColor3ub(94, 153, 84);
    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(1200, 0);
    glVertex2f(1200, 160); glVertex2f(0, 160);
    glEnd();
}

void drawRoad_Monsoon() {
    glColor3ub(70, 70, 75);
    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(1200, 0);
    glVertex2f(1200, 70); glVertex2f(0, 70);
    glEnd();

    glColor3ub(230, 230, 230);
    for (int x = 20; x < 1200; x += 70) {
        glBegin(GL_QUADS);
        glVertex2f(x, 33); glVertex2f(x + 35, 33);
        glVertex2f(x + 35, 37); glVertex2f(x, 37);
        glEnd();
    }
}

void drawClouds_Monsoon() {
    glPushMatrix();
    glTranslatef(s3_cloudOffset, 0.0f, 0.0f);
    circle_Monsoon(45, 120, 510, 55, 60, 70);
    circle_Monsoon(55, 170, 525, 55, 60, 70);
    circle_Monsoon(40, 230, 505, 55, 60, 70);
    
    circle_Monsoon(50, 450, 530, 45, 50, 60);
    circle_Monsoon(60, 510, 515, 45, 50, 60);
    circle_Monsoon(45, 580, 535, 45, 50, 60);
    
    circle_Monsoon(40, 680, 510, 50, 55, 65);
    circle_Monsoon(50, 730, 525, 50, 55, 65);
    
    circle_Monsoon(50, 950, 530, 45, 50, 60);
    circle_Monsoon(60, 1010, 515, 45, 50, 60);
    circle_Monsoon(45, 1080, 535, 45, 50, 60);
    glPopMatrix();
}

void drawTree_Monsoon(float xOffset) {
    glPushMatrix();
    glTranslatef(xOffset, 0.0f, 0.0f);
    
    glColor3ub(90, 60, 35);
    glBegin(GL_QUADS);
    glVertex2f(90, 160); glVertex2f(120, 160);
    glVertex2f(115, 300); glVertex2f(95, 300);
    glEnd();
    
    glPushMatrix();
    glTranslatef(105.0f, 300.0f, 0.0f);
    glRotatef(10 * sin(s3_treeAngle), 0, 0, 1);
    glTranslatef(-105.0f, -300.0f, 0.0f);
    
    circle_Monsoon(55, 105, 340, 45, 120, 45);
    circle_Monsoon(45, 65, 310, 45, 120, 45);
    circle_Monsoon(45, 150, 310, 45, 120, 45);
    circle_Monsoon(50, 105, 380, 45, 120, 45);
    glPopMatrix();
    
    glPopMatrix();
}

void drawBench_Monsoon(float xOffset) {
    glPushMatrix();
    glTranslatef(xOffset, 0.0f, 0.0f);
    glColor3ub(110, 55, 40);
    glBegin(GL_QUADS); glVertex2f(224, 160); glVertex2f(230, 160); glVertex2f(230, 190); glVertex2f(224, 190); glEnd();
    glBegin(GL_QUADS); glVertex2f(234, 160); glVertex2f(240, 160); glVertex2f(240, 190); glVertex2f(234, 190); glEnd();
    glBegin(GL_QUADS); glVertex2f(300, 160); glVertex2f(306, 160); glVertex2f(306, 190); glVertex2f(300, 190); glEnd();
    glBegin(GL_QUADS); glVertex2f(310, 160); glVertex2f(316, 160); glVertex2f(316, 190); glVertex2f(310, 190); glEnd();
    glBegin(GL_QUADS); glVertex2f(224, 190); glVertex2f(230, 190); glVertex2f(230, 248); glVertex2f(224, 248); glEnd();
    glBegin(GL_QUADS); glVertex2f(310, 190); glVertex2f(316, 190); glVertex2f(316, 248); glVertex2f(310, 248); glEnd();
    glColor3ub(190, 95, 65);
    glBegin(GL_QUADS); glVertex2f(222, 190); glVertex2f(318, 190); glVertex2f(318, 196); glVertex2f(222, 196); glEnd();
    glBegin(GL_QUADS); glVertex2f(222, 198); glVertex2f(318, 198); glVertex2f(318, 204); glVertex2f(222, 204); glEnd();
    glBegin(GL_QUADS); glVertex2f(222, 210); glVertex2f(318, 210); glVertex2f(318, 216); glVertex2f(222, 216); glEnd();
    glBegin(GL_QUADS); glVertex2f(222, 220); glVertex2f(318, 220); glVertex2f(318, 226); glVertex2f(222, 226); glEnd();
    glBegin(GL_QUADS); glVertex2f(222, 230); glVertex2f(318, 230); glVertex2f(318, 236); glVertex2f(222, 236); glEnd();
    glPopMatrix();
}
void drawStreetLamp_Monsoon(float xOffset) {
    glPushMatrix();
    glTranslatef(xOffset, 160.0f, 0.0f);
    glScalef(20.0f, 20.0f, 1.0f);
    glColor3ub(80, 80, 80);
    glBegin(GL_TRIANGLES); glVertex2f(2.0f, 0.5f); glVertex2f(1.5f, 0.0f); glVertex2f(2.5f, 0.0f); glEnd();
    glBegin(GL_QUADS); glVertex2f(1.5f, 0.0f); glVertex2f(2.5f, 0.0f); glVertex2f(2.2008f, 0.5043f); glVertex2f(1.7784f, 0.5118f); glEnd();
    glBegin(GL_QUADS); glVertex2f(1.7784f, 0.5118f); glVertex2f(2.2008f, 0.5043f); glVertex2f(2.2008f, 5.0f); glVertex2f(1.7784f, 5.0f); glEnd();
    glBegin(GL_QUADS); glVertex2f(1.5f, 5.0f); glVertex2f(2.5f, 5.0f); glVertex2f(3.0f, 6.0f); glVertex2f(1.0f, 6.0f); glEnd();
    glBegin(GL_TRIANGLES); glVertex2f(0.7f, 6.0f); glVertex2f(3.3f, 6.0f); glVertex2f(2.0f, 7.0f); glEnd();
    glColor3ub(255, 200, 50);
    glBegin(GL_QUADS); glVertex2f(1.1f, 5.92f); glVertex2f(1.5f, 5.08f); glVertex2f(1.92f, 5.08f); glVertex2f(1.92f, 5.92f); glEnd();
    glBegin(GL_QUADS); glVertex2f(2.08f, 5.92f); glVertex2f(2.08f, 5.08f); glVertex2f(2.5f, 5.08f); glVertex2f(2.9f, 5.92f); glEnd();
    glPopMatrix();
}
void drawHouse_Monsoon() {
    glColor3ub(230, 218, 195);
    glBegin(GL_QUADS); glVertex2f(620, 160); glVertex2f(780, 160); glVertex2f(780, 270); glVertex2f(620, 270); glEnd();
    glColor3ub(55, 85, 135);
    glBegin(GL_TRIANGLES); glVertex2f(600, 270); glVertex2f(800, 270); glVertex2f(700, 335); glEnd();
    glColor3ub(150, 100, 80);
    glBegin(GL_QUADS); glVertex2f(730, 270); glVertex2f(755, 270); glVertex2f(755, 345); glVertex2f(730, 345); glEnd();
    glColor3ub(95, 65, 40);
    glBegin(GL_QUADS); glVertex2f(688, 160); glVertex2f(716, 160); glVertex2f(716, 212); glVertex2f(688, 212); glEnd();
    circle_Monsoon(2, 710, 186, 220, 190, 90);
    if (s3_lightOn == 1) glColor3ub(255, 225, 90);
    else glColor3ub(90, 90, 100);
    glBegin(GL_QUADS); glVertex2f(638, 210); glVertex2f(672, 210); glVertex2f(672, 244); glVertex2f(638, 244); glEnd();
    glColor3ub(150, 100, 80);
    glBegin(GL_QUADS); glVertex2f(654, 210); glVertex2f(656, 210); glVertex2f(656, 244); glVertex2f(654, 244); glEnd();
    glBegin(GL_QUADS); glVertex2f(638, 226); glVertex2f(672, 226); glVertex2f(672, 228); glVertex2f(638, 228); glEnd();
}
void drawMailbox_Monsoon() {
    glColor3ub(70, 70, 70);
    glBegin(GL_QUADS); glVertex2f(825.0f, 160.0f); glVertex2f(835.0f, 160.0f); glVertex2f(835.0f, 210.0f); glVertex2f(825.0f, 210.0f); glEnd();
    glColor3ub(195, 35, 35);
    glBegin(GL_QUADS); glVertex2f(805.0f, 210.0f); glVertex2f(855.0f, 210.0f); glVertex2f(855.0f, 235.0f); glVertex2f(805.0f, 235.0f); glEnd();
    glBegin(GL_TRIANGLES); glVertex2f(805.0f, 235.0f); glVertex2f(855.0f, 235.0f); glVertex2f(830.0f, 250.0f); glEnd();
    glColor3ub(120, 15, 15);
    glBegin(GL_QUADS); glVertex2f(815.0f, 220.0f); glVertex2f(845.0f, 220.0f); glVertex2f(845.0f, 224.0f); glVertex2f(815.0f, 224.0f); glEnd();

    glColor3ub(230, 230, 230);
    glBegin(GL_QUADS); glVertex2f(852.0f, 226.0f); glVertex2f(858.0f, 226.0f); glVertex2f(858.0f, 234.0f); glVertex2f(852.0f, 234.0f); glEnd();
}

void drawFence_Monsoon() {
    glColor3ub(180, 140, 90);
    for(int i = 0; i < 5; i++) {
        float fx = 15.0f + (i * 18.0f);
        glBegin(GL_QUADS); glVertex2f(fx, 160.0f); glVertex2f(fx+10.0f, 160.0f); glVertex2f(fx+10.0f, 195.0f); glVertex2f(fx, 195.0f); glEnd();
        glBegin(GL_TRIANGLES); glVertex2f(fx, 195.0f); glVertex2f(fx+10.0f, 195.0f); glVertex2f(fx+5.0f, 206.0f); glEnd();}
    glColor3ub(120, 80, 50);
    glBegin(GL_QUADS); glVertex2f(10.0f, 172.0f); glVertex2f(100.0f, 172.0f); glVertex2f(100.0f, 178.0f); glVertex2f(10.0f, 178.0f); glEnd();
    glBegin(GL_QUADS); glVertex2f(10.0f, 186.0f); glVertex2f(100.0f, 186.0f); glVertex2f(100.0f, 192.0f); glVertex2f(10.0f, 192.0f); glEnd();
}

void drawPuddles_Monsoon() {
    circle_Monsoon(22, 150, 130, 140, 180, 210);
    circle_Monsoon(18, 260, 125, 140, 180, 210);
}
void tire_Monsoon(float cx, float cy, float radius, float red, float green, float blue) {
    glColor3ub((GLubyte)red, (GLubyte)green, (GLubyte)blue);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 200; i++) {
        float angle = (i * 2 * 3.1416f) / 200;
        glVertex2f(cx + radius * cos(angle), cy + radius * sin(angle));
    }
    glEnd();
}
void carlowerPart_Monsoon() {
    glColor3ub(200, 30, 30);
    glBegin(GL_QUADS); glVertex2f(-0.5f, 0.0f); glVertex2f(0.5f, 0.0f); glVertex2f(0.5f, 0.2f); glVertex2f(-0.5f, 0.2f); glEnd();
}
void carupperPart_Monsoon() {
    glColor3ub(150, 200, 240);
    glBegin(GL_QUADS); glVertex2f(-0.25f, 0.2f); glVertex2f(0.25f, 0.2f); glVertex2f(0.15f, 0.38f); glVertex2f(-0.15f, 0.38f); glEnd();
}
void carfrontWheel_Monsoon() {
    glPushMatrix();
    glTranslatef(s3_carX + 0.3f, -0.05f, 0.0f);
    glRotatef(s3_wheelAngle, 0.0f, 0.0f, -1.0f);
    tire_Monsoon(0.0f, 0.0f, 0.12f, 20, 20, 20);
    glColor3ub(220, 220, 220);
    glBegin(GL_LINES); glVertex2f(0.0f, 0.0f); glVertex2f(0.0f, 0.12f); glEnd();
    glPopMatrix();
}
void carbackWheel_Monsoon() {
    glPushMatrix();
    glTranslatef(s3_carX - 0.3f, -0.05f, 0.0f);
    glRotatef(s3_wheelAngle, 0.0f, 0.0f, -1.0f);
    tire_Monsoon(0.0f, 0.0f, 0.12f, 20, 20, 20);
    glColor3ub(220, 220, 220);
    glBegin(GL_LINES); glVertex2f(0.0f, 0.0f); glVertex2f(0.0f, 0.12f); glEnd();
    glPopMatrix();
}
void drawCar_Monsoon() {
    glPushMatrix();
    glTranslatef(600.0f, 46.0f, 0.0f);
    glScalef(150.0f, 150.0f, 1.0f);   
    glPushMatrix();
    glTranslatef(s3_carX, 0.0f, 0.0f);
    carlowerPart_Monsoon();
    carupperPart_Monsoon();
    glPopMatrix();
    carfrontWheel_Monsoon();
    carbackWheel_Monsoon();
    glPopMatrix();
}
void drawBubble(float x, float y, float radius, float alpha) {
    int segments = 32;
    glColor4f(0.5f, 0.7f, 1.0f, alpha);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float ang = 2.0f * 3.14159f * i / segments;
        glVertex2f(x + cosf(ang) * radius, y + sinf(ang) * radius);
    }
    glEnd();
}
void drawDrops() {
    glColor3f(0.2f, 0.4f, 1.0f);
    for (int i = 0; i < s3_dropCount; ++i) {
        glBegin(GL_LINES);
        glVertex2f(s3_dropX[i], s3_dropY[i]);
        glVertex2f(s3_dropX[i], s3_dropY[i] - 15.0f);
        glEnd();
    }
}
void drawBubbles() {
    for (int i = 0; i < s3_bubbleCount; ++i) {
        if (s3_bubbleActive[i]) {
            drawBubble(s3_bubbleX[i], s3_bubbleY[i], s3_bubbleRadius[i], s3_bubbleAlpha[i]);
        }
    }
}
// ==========================================
// SCENE 1
// ==========================================
void scene1() {
    // Plotting the sky background
    float topR = 0.02f + global_brightness * 0.38f; float topG = 0.02f + global_brightness * 0.58f; float topB = 0.15f + global_brightness * 0.70f; 
    float botR = 0.04f + global_brightness * 0.54f; float botG = 0.05f + global_brightness * 0.73f; float botB = 0.18f + global_brightness * 0.74f;
    glBegin(GL_QUADS);
    glColor3f(botR, botG, botB);
    glVertex2f(0, 0);
    glVertex2f(1200, 0);
    glColor3f(topR, topG, topB);
    glVertex2f(1200, 700);
    glVertex2f(0, 700);
    glEnd();

    // Plotting the sun or moon
    if (global_brightness > 0.2f) {
        float sunR = 1.0f, sunG = 0.85f, sunB = 0.0f; 
        glColor3f(sunR, sunG, sunB); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((600) + (40) * cos(_theta), (global_sunY) + (40) * sin(_theta));
    }
    glEnd();
        glEnable(GL_BLEND); glColor4f(sunR, sunG, sunB, global_brightness * 0.5f); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((600) + (70) * cos(_theta), (global_sunY) + (70) * sin(_theta));
    }
    glEnd();
        glColor4f(sunR, sunG, sunB, global_brightness * 0.2f); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((600) + (110) * cos(_theta), (global_sunY) + (110) * sin(_theta));
    }
    glEnd(); glDisable(GL_BLEND);
    } else {
        float moonR = 0.85f, moonG = 0.90f, moonB = 0.95f; 
        glColor3f(moonR, moonG, moonB); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((600) + (40) * cos(_theta), (450) + (40) * sin(_theta));
    }
    glEnd(); 
        glColor3f(0.7f, 0.75f, 0.85f); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((590) + (8) * cos(_theta), (460) + (8) * sin(_theta));
    }
    glEnd(); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((610) + (10) * cos(_theta), (440) + (10) * sin(_theta));
    }
    glEnd();
        glEnable(GL_BLEND); float glowAlpha = 1.0f - global_brightness;
        glColor4f(moonR, moonG, moonB, 0.3f * glowAlpha); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((600) + (75) * cos(_theta), (450) + (75) * sin(_theta));
    }
    glEnd();
        glColor4f(moonR, moonG, moonB, 0.15f * glowAlpha); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((600) + (120) * cos(_theta), (450) + (120) * sin(_theta));
    }
    glEnd(); glDisable(GL_BLEND);
    }
    
    // Plotting the clouds
    if (global_brightness > 0.1f) {
        float cloudAlpha = global_brightness; 
        drawCloud(fmod(100 + s1_cloudOffset * 0.8f, 1500.0f) - 200.0f, 550, 1.0f, cloudAlpha);
        drawCloud(fmod(500 + s1_cloudOffset * 1.0f, 1500.0f) - 200.0f, 620, 0.8f, cloudAlpha);
        drawCloud(fmod(850 + s1_cloudOffset * 0.6f, 1500.0f) - 200.0f, 530, 1.1f, cloudAlpha);
        drawCloud(fmod(1200 + s1_cloudOffset * 1.2f, 1500.0f) - 200.0f, 580, 0.9f, cloudAlpha);
    }

    if (global_brightness > 0.5f) {
        glColor3f(0.1f, 0.1f, 0.1f); glLineWidth(2.0f);
        for (int i = 0; i < 4; i++) {
            float bx = 300 + i * 60; float by = 500 + i * 15; float wingY = 12 * sin(s1_birdPhase + i); 
            glBegin(GL_LINE_STRIP); glVertex2f(bx - 12, by + wingY); glVertex2f(bx, by); glVertex2f(bx + 12, by + wingY); glEnd();
        }
        glLineWidth(1.0f);
    }

    float ambient = 0.35f + (global_brightness * 0.65f); 
    // Plotting mountains
    drawMountainWithTrees(300, 200, 450, 150, ambient); 
    drawMountainWithTrees(900, 200, 550, 180, ambient); 
    
    glColor3f(0.30f * ambient, 0.55f * ambient, 0.25f * ambient); 
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(1200, 0);
    glVertex2f(1200, 250);
    glVertex2f(0, 250);
    glEnd();
    glColor3f(0.55f * ambient, 0.42f * ambient, 0.28f * ambient);
    glBegin(GL_QUAD_STRIP);
    for (float y = 0; y <= 250; y += 20) {
        float xCenter = 600 + 120 * sin(y * 0.02f); 
        float width = 30 + (250 - y) * 0.3f;
        glVertex2f(xCenter - width, y); glVertex2f(xCenter + width, y);
    }
    glEnd();

    // Plotting paddy fields
    drawPaddyField(40, 20, 360, 110, ambient); 
    drawPaddyField(760, 20, 400, 120, ambient);  

    // Plotting trees and houses
    drawTree(100, 250, 0.9f, ambient); drawTree(900, 250, 0.9f, ambient); 
    drawTree(320, 240, 1.0f, ambient); drawTree(1050, 240, 1.0f, ambient);
    drawTree(700, 230, 1.1f, ambient); drawTree(850, 230, 1.1f, ambient); 
    drawTree(180, 220, 1.2f, ambient); drawTree(1150, 220, 1.2f, ambient);
    drawTree(950, 200, 1.3f, ambient); drawTree(50, 190, 1.4f, ambient); 
    drawTree(250, 180, 1.5f, ambient);
    float housePositions[8][3] = { {150, 260, 0.8f}, {720, 260, 0.8f}, {550, 250, 0.85f}, {980, 240, 0.9f}, {220, 230, 1.0f}, {880, 220, 1.0f}, {480, 210, 1.1f}, {780, 190, 1.3f} };

    for(int i=0; i<8; i++) {
        if(!s1_isDestroyed) {
            drawBeautifulHouse(housePositions[i][0], housePositions[i][1], housePositions[i][2], ambient);
        } else {
            drawRuinedHouseBlackNEW(housePositions[i][0], housePositions[i][1], housePositions[i][2] * 8.0f, ambient);
            drawBrickPileLarge(housePositions[i][0], housePositions[i][1], housePositions[i][2] * 1.5f, ambient);
            drawFire(housePositions[i][0], housePositions[i][1], housePositions[i][2] * 1.5f);
            drawSmoke(housePositions[i][0], housePositions[i][1], housePositions[i][2] * 1.5f);
        }
    }
    // Plotting airplane and bomb
    if (s1_planeActive) { drawAirplane(s1_planeX, s1_planeY, 1.5f, ambient); }
    if (s1_bombState == 1) { 
        glColor3f(0.2f, 0.2f, 0.2f); 
        glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((s1_bombX) + (18) * cos(_theta), (s1_bombY) + (7) * sin(_theta));
    }
    glEnd(); 
        glBegin(GL_TRIANGLES);
    glVertex2f(s1_bombX - 18, s1_bombY);
    glVertex2f(s1_bombX - 25, s1_bombY + 6);
    glVertex2f(s1_bombX - 25, s1_bombY - 6);
    glEnd();
    } else if (s1_bombState == 2) { 
        glEnable(GL_BLEND); 
        float alpha = std::max(0.0f, 1.0f - (s1_explRadius / 350.0f));
        glColor4f(1.0f, 0.2f, 0.0f, alpha); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((s1_bombX) + (s1_explRadius) * cos(_theta), (s1_bombY) + (s1_explRadius) * sin(_theta));
    }
    glEnd();
        glColor4f(1.0f, 0.6f, 0.0f, alpha); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((s1_bombX) + (s1_explRadius * 0.7f) * cos(_theta), (s1_bombY) + (s1_explRadius * 0.7f) * sin(_theta));
    }
    glEnd();
        glColor4f(1.0f, 1.0f, 0.6f, alpha); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((s1_bombX) + (s1_explRadius * 0.4f) * cos(_theta), (s1_bombY) + (s1_explRadius * 0.4f) * sin(_theta));
    }
    glEnd(); 
        glDisable(GL_BLEND);
        glColor3f(0.1f, 0.1f, 0.1f); 
        for (int i = 0; i < 20; i++) { glBegin(GL_QUADS);
    glVertex2f(s1_debris_x[i], s1_debris_y[i]);
    glVertex2f(s1_debris_x[i] + 5, s1_debris_y[i]);
    glVertex2f(s1_debris_x[i] + 5, s1_debris_y[i] + 5);
    glVertex2f(s1_debris_x[i], s1_debris_y[i] + 5);
    glEnd();
    }
    }
    drawUniversalRain();
}

void drawS2Boat1() {
    glPushMatrix(); 
    glTranslatef(s2_boat1_x, 140, 0); 
    glScalef(0.6f, 0.6f, 1.0f);
    
    glColor3f(0.6f, 0.3f, 0.2f); 
    glBegin(GL_POLYGON); 
    glVertex2f(-30, 0); glVertex2f(30, 0); glVertex2f(45, 15); glVertex2f(-40, 15); 
    glEnd();
    
    glColor3f(0.8f, 0.8f, 0.8f); 
    glBegin(GL_POLYGON);
    glVertex2f(-10, 15); glVertex2f(10, 15); glVertex2f(10, 30); glVertex2f(-10, 30);
    glEnd();
    glPopMatrix();
}

void drawS2Boat2() {
    glPushMatrix(); 
    glTranslatef(s2_boat2_x, 130, 0); 
    glScalef(0.5f, 0.5f, 1.0f);
    
    glColor3f(0.3f, 0.4f, 0.7f); 
    glBegin(GL_POLYGON); 
    glVertex2f(-25, 0); glVertex2f(25, 0); glVertex2f(35, 12); glVertex2f(-35, 12); 
    glEnd();
    
    glColor3f(0.9f, 0.9f, 0.9f); 
    glBegin(GL_POLYGON);
    glVertex2f(-15, 12); glVertex2f(5, 12); glVertex2f(5, 25); glVertex2f(-15, 25);
    glEnd();
    glPopMatrix();
}


// SCENE 2

void scene2() {
    // Plotting the sky background
    float topR = 0.02f + global_brightness * 0.38f; float topG = 0.02f + global_brightness * 0.58f; float topB = 0.15f + global_brightness * 0.70f; 
    float botR = 0.04f + global_brightness * 0.54f; float botG = 0.05f + global_brightness * 0.73f; float botB = 0.18f + global_brightness * 0.74f;
    glBegin(GL_QUADS);
    glColor3f(botR, botG, botB);
    glVertex2f(0, 0);
    glVertex2f(1200, 0);
    glColor3f(topR, topG, topB);
    glVertex2f(1200, 700);
    glVertex2f(0, 700);
    glEnd();
    
    float ambient = 0.35f + (global_brightness * 0.65f); 

    // Plotting the sun or moon


    if (global_brightness > 0.2f) {
        float sunR = 1.0f, sunG = 0.85f, sunB = 0.0f; 
        glColor3f(sunR, sunG, sunB); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((600) + (40) * cos(_theta), (global_sunY) + (40) * sin(_theta));}
    glEnd();


        glEnable(GL_BLEND); glColor4f(sunR, sunG, sunB, global_brightness * 0.5f); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((600) + (70) * cos(_theta), (global_sunY) + (70) * sin(_theta));}
    glEnd(); 



        glColor4f(sunR, sunG, sunB, global_brightness * 0.2f); glBegin(GL_POLYGON);
 for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((600) + (110) * cos(_theta), (global_sunY) + (110) * sin(_theta));}
    glEnd(); glDisable(GL_BLEND);} 
    
    
    else {
        float moonR = 0.85f, moonG = 0.90f, moonB = 0.95f; glColor3f(moonR, moonG, moonB); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((600) + (40) * cos(_theta), (450) + (40) * sin(_theta));}
    glEnd(); 



        glColor3f(0.7f, 0.75f, 0.85f); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((590) + (8) * cos(_theta), (460) + (8) * sin(_theta));}
    glEnd(); glBegin(GL_POLYGON);




    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((610) + (10) * cos(_theta), (440) + (10) * sin(_theta));
    }
    glEnd();



        glEnable(GL_BLEND); float glowAlpha = 1.0f - global_brightness; glColor4f(moonR, moonG, moonB, 0.3f * glowAlpha); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((600) + (75) * cos(_theta), (450) + (75) * sin(_theta));
    }
    glEnd(); glColor4f(moonR, moonG, moonB, 0.15f * glowAlpha); glBegin(GL_POLYGON);



    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((600) + (120) * cos(_theta), (450) + (120) * sin(_theta)); }
    glEnd(); glDisable(GL_BLEND);}




    // Plotting city ground and distant structures


    glColor3f(0.20f * ambient, 0.22f * ambient, 0.25f * ambient); glBegin(GL_QUADS);
    glVertex2f(0, 120);
    glVertex2f(1200, 120);
    glVertex2f(1200, 160);
    glVertex2f(0, 160);
    glEnd(); 

    glPushMatrix(); glTranslatef(0.0f, 58.0f, 0.0f); glScalef(0.7f, 0.7f, 1.0f); 
    glColor3f(0.48f * ambient, 0.57f * ambient, 0.63f * ambient); glBegin(GL_QUADS);
    glVertex2f(0, 160);
    glVertex2f(0 + 70, 160);
    glVertex2f(0 + 70, 160 + 230);
    glVertex2f(0, 160 + 230);
    glEnd();
    glColor3f(0.43f * ambient, 0.52f * ambient, 0.58f * ambient); glBegin(GL_QUADS);
    glVertex2f(75, 160);
    glVertex2f(75 + 70, 160);
    glVertex2f(75 + 70, 160 + 270);
    glVertex2f(75, 160 + 270);
    glEnd();
    glColor3f(0.46f * ambient, 0.56f * ambient, 0.63f * ambient); glBegin(GL_QUADS);
    glVertex2f(1525, 160);
    glVertex2f(1525 + 75, 160);
    glVertex2f(1525 + 75, 160 + 270);
    glVertex2f(1525, 160 + 270);
    glEnd();
    glColor3f(0.85f * ambient, 0.90f * ambient, 0.88f * ambient);
    for (int y = 190; y < 390; y += 35) { glBegin(GL_QUADS);
    glVertex2f(15, y);
    glVertex2f(15 + 40, y);
    glVertex2f(15 + 40, y + 10);
    glVertex2f(15, y + 10);
    glEnd(); glBegin(GL_QUADS);
    glVertex2f(90, y);
    glVertex2f(90 + 40, y);
    glVertex2f(90 + 40, y + 10);
    glVertex2f(90, y + 10);
    glEnd(); glBegin(GL_QUADS);
    glVertex2f(1545, y);
    glVertex2f(1545 + 35, y);
    glVertex2f(1545 + 35, y + 10);
    glVertex2f(1545, y + 10);
    glEnd(); }

    // Plotting buildings
    for(int i = 0; i < 12; i++) {
        if(!s2_b_isDestroyed[i]) {
            if(i == 1) buildingGlass(i, ambient, global_brightness);
            else if(i == 4 || i == 8 || i == 9 || i == 10) drawHalfConstructedBuilding(i, ambient, global_brightness);
            else buildingNormal(i, ambient, global_brightness);
            
            if(i == 3 && s2_b_progress[i] > 0.5f) drawSteelSuperstructure(s2_b_x[i] + 5.0f, s2_b_y[i] + s2_b_height[i] * s2_b_progress[i], 0.90f, 0.78f, ambient);
            if(i == 6 && s2_b_progress[i] > 0.5f) drawSteelSuperstructure(s2_b_x[i] + 5.0f, s2_b_y[i] + s2_b_height[i] * s2_b_progress[i], 1.05f, 1.0f, ambient);
        } else {
            drawRuinedHouseBlackNEW(s2_b_x[i] + s2_b_width[i]/2, s2_b_y[i] + 100.0f, 8.0f, ambient);
            drawBrickPileLarge(s2_b_x[i] + s2_b_width[i]/2, s2_b_y[i], 1.5f, ambient);
            drawFire(s2_b_x[i] + s2_b_width[i]/2, s2_b_y[i], 1.5f);
            drawSmoke(s2_b_x[i] + s2_b_width[i]/2, s2_b_y[i], 1.5f);
        }
    }

    // Plotting construction cranes, materials and barriers
    drawTranslatedCrane(520.0f, 160.0f, 1.05f, 1.25f, ambient); drawTranslatedCrane(820.0f, 160.0f, 1.15f, 1.40f, ambient); drawTranslatedCrane(1270.0f, 160.0f, -1.0f, 1.20f, ambient);
    drawMaterials(360, 165, ambient); drawMaterials(680, 165, ambient); drawMaterials(1060, 165, ambient); drawMaterials(1260, 165, ambient);
    drawBarrier(420, 170, ambient); drawBarrier(720, 170, ambient); drawBarrier(1030, 170, ambient); drawBarrier(1320, 170, ambient);
    drawConstructionSign(285, 165, ambient); drawConstructionSign(1080, 165, ambient);
    drawCityTrees(ambient);
    glPopMatrix();

    // Plotting submarine
    glPushMatrix(); glTranslatef(s2_subX, s2_subY, 0);
    glColor3f(0.9f * ambient, 0.8f * ambient, 0.2f * ambient); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((0) + (70) * cos(_theta), (0) + (25) * sin(_theta));
    }
    glEnd();
    glColor3f(0.8f * ambient, 0.7f * ambient, 0.1f * ambient); glBegin(GL_QUADS);
    glVertex2f(-15, 15);
    glVertex2f(15, 15);
    glVertex2f(15, 45);
    glVertex2f(-15, 45);
    glEnd();
    glColor3f(0.4f * ambient, 0.4f * ambient, 0.4f * ambient); glBegin(GL_LINES); glVertex2f(5, 45); glVertex2f(5, 60); glVertex2f(15, 60); glEnd();
    glColor3f(0.1f * ambient, 0.5f * ambient, 0.8f * ambient); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((-25) + (8) * cos(_theta), (0) + (8) * sin(_theta));
    }
    glEnd(); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((0) + (8) * cos(_theta), (0) + (8) * sin(_theta));
    }
    glEnd(); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((25) + (8) * cos(_theta), (0) + (8) * sin(_theta));
    }
    glEnd();
    glColor3f(0.5f * ambient, 0.5f * ambient, 0.5f * ambient); glBegin(GL_QUADS);
    glVertex2f(-80, -5);
    glVertex2f(-70, -5);
    glVertex2f(-70, 5);
    glVertex2f(-80, 5);
    glEnd();
    glPushMatrix(); glTranslatef(-80, 0, 0); glRotatef(s2_propAngle, 1, 0, 0); glBegin(GL_QUADS);
    glVertex2f(-2, -20);
    glVertex2f(2, -20);
    glVertex2f(2, 20);
    glVertex2f(-2, 20);
    glEnd(); glPopMatrix();
    glPopMatrix(); 
    // Plotting boats
    drawS2Boat1();
    drawS2Boat2();
    glEnable(GL_BLEND); glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
    for (int i = 0; i < 20; i++) if (s2_bubble_active[i]) { glBegin(GL_LINE_LOOP);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((s2_bubble_x[i]) + (5) * cos(_theta), (s2_bubble_y[i]) + (5) * sin(_theta));
    }
    glEnd();
    }
    glDisable(GL_BLEND);

    if (s2_bombState == 1) { 
        glColor3f(0.2f, 0.2f, 0.2f); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((s2_bombX) + (18) * cos(_theta), (s2_bombY) + (7) * sin(_theta));
    }
    glEnd(); glBegin(GL_TRIANGLES);
    glVertex2f(s2_bombX - 18, s2_bombY);
    glVertex2f(s2_bombX - 25, s2_bombY + 6);
    glVertex2f(s2_bombX - 25, s2_bombY - 6);
    glEnd();
    }
    else if (s2_bombState == 2) { 
        glEnable(GL_BLEND); float alpha = std::max(0.0f, 1.0f - (s2_explRadius / 150.0f));
        glColor4f(1.0f, 0.2f, 0.0f, alpha); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((s2_bombX) + (s2_explRadius) * cos(_theta), (s2_bombY) + (s2_explRadius) * sin(_theta));
    }
    glEnd();
        glColor4f(1.0f, 0.6f, 0.0f, alpha); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((s2_bombX) + (s2_explRadius * 0.7f) * cos(_theta), (s2_bombY) + (s2_explRadius * 0.7f) * sin(_theta));
    }
    glEnd();
        glColor4f(1.0f, 1.0f, 0.6f, alpha); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((s2_bombX) + (s2_explRadius * 0.4f) * cos(_theta), (s2_bombY) + (s2_explRadius * 0.4f) * sin(_theta));
    }
    glEnd(); glDisable(GL_BLEND);
        glColor3f(0.1f, 0.1f, 0.1f); for (int i = 0; i < 12; i++) { glBegin(GL_QUADS);
    glVertex2f(s2_debris_x[i], s2_debris_y[i]);
    glVertex2f(s2_debris_x[i] + 5, s2_debris_y[i]);
    glVertex2f(s2_debris_x[i] + 5, s2_debris_y[i] + 5);
    glVertex2f(s2_debris_x[i], s2_debris_y[i] + 5);
    glEnd();
    }
    }
    // Plotting water waves
    glEnable(GL_BLEND); glColor4f(0.05f * ambient, 0.40f * ambient, 0.60f * ambient, 0.85f); 
    glBegin(GL_QUAD_STRIP);
    for (float x = 0; x <= 1200; x += 20) { float y = 140 + 10 * sin(x * 0.015f + s2_wavePhase); glVertex2f(x, y); glVertex2f(x, 0); }
    glEnd(); glDisable(GL_BLEND);
    
    drawUniversalRain();
}

// SCENE 3

void scene3() {
    glClearColor(0.10f, 0.13f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Plotting ground and road
    drawGround_Monsoon();
    drawRoad_Monsoon();


    // Plotting clouds

    drawClouds_Monsoon();
    
    // Plotting trees

    drawTree_Monsoon(0.0f);
    drawTree_Monsoon(300.0f);
    drawTree_Monsoon(800.0f);
    
    // Plotting benches and street lamps
    drawBench_Monsoon(0.0f);
    drawBench_Monsoon(600.0f);
    
    drawStreetLamp_Monsoon(500.0f);
    drawStreetLamp_Monsoon(100.0f);
    drawStreetLamp_Monsoon(900.0f);
    
    // Plotting house, mailbox, fence and puddles

    drawHouse_Monsoon();
    drawMailbox_Monsoon();
    drawFence_Monsoon();
    drawPuddles_Monsoon();
    
    // Plotting car

    drawCar_Monsoon();
    
    // Plotting rain and bubbles

    if (global_isRain) {
        drawDrops();
        drawBubbles();
    }
}
void drawS4Tree(float x, float y) {
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.1f, 0.05f);
    glVertex2f(x - 5, y);
    glVertex2f(x + 5, y);
    glVertex2f(x + 5, y + 40);
    glVertex2f(x - 5, y + 40);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.1f, 0.4f, 0.1f);
    glVertex2f(x - 25, y + 30);
    glVertex2f(x + 25, y + 30);
    glVertex2f(x, y + 70);
    glVertex2f(x - 20, y + 55);
    glVertex2f(x + 20, y + 55);
    glVertex2f(x, y + 95);
    glVertex2f(x - 15, y + 75);
    glVertex2f(x + 15, y + 75);
    glVertex2f(x, y + 110);
    glEnd();
}
void drawS4Shop(float x, float y, float r, float g, float b) {
    glBegin(GL_QUADS);

    // Main Body

    glColor3f(r, g, b);
    glVertex2f(x, y);
    glVertex2f(x + 60, y);
    glVertex2f(x + 60, y + 50);
    glVertex2f(x, y + 50);

    // Door

    glColor3f(0.3f, 0.2f, 0.1f);
    glVertex2f(x + 20, y);
    glVertex2f(x + 40, y);
    glVertex2f(x + 40, y + 35);
    glVertex2f(x + 20, y + 35);

    // Roof

    glColor3f(0.8f, 0.1f, 0.1f);
    glVertex2f(x - 10, y + 50);
    glVertex2f(x + 70, y + 50);
    glVertex2f(x + 60, y + 70);
    glVertex2f(x, y + 70);
    
    // Window

    glColor3f(0.9f, 0.9f, 0.3f);
    glVertex2f(x + 5, y + 20);
    glVertex2f(x + 15, y + 20);
    glVertex2f(x + 15, y + 35);
    glVertex2f(x + 5, y + 35);
    glVertex2f(x + 45, y + 20);
    glVertex2f(x + 55, y + 20);
    glVertex2f(x + 55, y + 35);
    glVertex2f(x + 45, y + 35);
    glEnd();
}

void drawS4Road() {
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(1200, 0);
    glVertex2f(1200, 60);
    glVertex2f(0, 60);
    glEnd();
    glColor3f(0.8f, 0.8f, 0.0f);
    for (int i = 20; i < 1200; i += 80) {
        glBegin(GL_QUADS);
    glVertex2f(i, 28);
    glVertex2f(i + 40, 28);
    glVertex2f(i + 40, 32);
    glVertex2f(i, 32);
    glEnd();
    }
}



void drawS4Car1() {
    glPushMatrix();
    glTranslatef(s4_car1_x, 15, 0);
    glScalef(0.8f, 0.8f, 1.0f);
    glColor3f(0.8f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(0, 10);
    glVertex2f(80, 10);
    glVertex2f(80, 30);
    glVertex2f(0, 30);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(15, 30);
    glVertex2f(60, 30);
    glVertex2f(60, 50);
    glVertex2f(15, 50);
    glEnd();
    glColor3f(0.8f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(75, 15);
    glVertex2f(80, 15);
    glVertex2f(80, 25);
    glVertex2f(75, 25);
    glEnd(); 
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((20) + (12) * cos(_theta), (10) + (12) * sin(_theta));
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((60) + (12) * cos(_theta), (10) + (12) * sin(_theta));
    }
    glEnd();
    glPopMatrix();
}
void drawS4Car2() {
    glPushMatrix();
    glTranslatef(s4_car2_x, 10, 0);
    glScalef(0.7f, 0.7f, 1.0f);
    glColor3f(0.1f, 0.2f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(0, 10);
    glVertex2f(90, 10);
    glVertex2f(90, 35);
    glVertex2f(0, 35);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(20, 35);
    glVertex2f(70, 35);
    glVertex2f(70, 55);
    glVertex2f(20, 55);
    glEnd();
    glColor3f(0.8f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(85, 15);
    glVertex2f(90, 15);
    glVertex2f(90, 25);
    glVertex2f(85, 25);
    glEnd(); 
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((25) + (14) * cos(_theta), (10) + (14) * sin(_theta));
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((65) + (14) * cos(_theta), (10) + (14) * sin(_theta));
    }
    glEnd();
    glPopMatrix();
}

void drawS4Car3() {
    glPushMatrix();
    glTranslatef(s4_car3_x, 20, 0);
    glScalef(0.6f, 0.6f, 1.0f);
    glColor3f(0.1f, 0.8f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(0, 10);
    glVertex2f(70, 10);
    glVertex2f(70, 25);
    glVertex2f(0, 25);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(10, 25);
    glVertex2f(50, 25);
    glVertex2f(50, 45);
    glVertex2f(10, 45);
    glEnd();
    glColor3f(0.8f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(65, 12);
    glVertex2f(70, 12);
    glVertex2f(70, 20);
    glVertex2f(65, 20);
    glEnd(); 
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((15) + (10) * cos(_theta), (10) + (10) * sin(_theta));
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((55) + (10) * cos(_theta), (10) + (10) * sin(_theta));
    }
    glEnd();
    glPopMatrix();
}


// SCENE 4
void scene4() {
    // Plotting the sky background
    float s4_dayNight = 1.0f - global_brightness; 
    float tR = 0.5f * (1 - s4_dayNight) + 0.03f * s4_dayNight; 
    float tG = 0.7f * (1 - s4_dayNight) + 0.03f * s4_dayNight; 
    float tB = 0.9f * (1 - s4_dayNight) + 0.12f * s4_dayNight;
    float bR = 0.8f * (1 - s4_dayNight) + 0.1f * s4_dayNight; 
    float bG = 0.9f * (1 - s4_dayNight) + 0.1f * s4_dayNight; 
    float bB = 1.0f * (1 - s4_dayNight) + 0.2f * s4_dayNight;
    glBegin(GL_QUADS);
    glColor3f(bR, bG, bB);
    glVertex2f(0, 0);
    glVertex2f(1200, 0);
    glColor3f(tR, tG, tB);
    glVertex2f(1200, 700);
    glVertex2f(0, 700);
    glEnd();

    // Plotting stars
    if (s4_dayNight > 0.1f) {
        glEnable(GL_BLEND);
        for (int i = 0; i < 40; i++) {
            float twinkle = 0.5f + 0.5f * sin(s1_birdPhase * 2.0f + s4_stars_phase[i]);
            glColor4f(1, 1, 1, s4_dayNight * twinkle); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((s4_stars_x[i]) + (s4_stars_size[i]) * cos(_theta), (s4_stars_y[i]) + (s4_stars_size[i]) * sin(_theta));
    }
    glEnd();
        }
        glDisable(GL_BLEND);
    }

    // Plotting the sun and moon
    float sunY = 500 - s4_dayNight * 400; 
    glColor4f(1.0f, 0.9f, 0.4f, 1.0f - s4_dayNight); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((300) + (45) * cos(_theta), (sunY) + (45) * sin(_theta));
    }
    glEnd();
    
    float moonY = 100 + s4_dayNight * 400; 
    glColor4f(0.9f, 0.9f, 0.9f, s4_dayNight); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((900) + (40) * cos(_theta), (moonY) + (40) * sin(_theta));
    }
    glEnd();
    glColor4f(0.7f, 0.7f, 0.7f, s4_dayNight); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((890) + (7) * cos(_theta), (moonY + 12) + (7) * sin(_theta));
    }
    glEnd(); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((915) + (9) * cos(_theta), (moonY - 5) + (9) * sin(_theta));
    }
    glEnd(); 
    glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((885) + (6) * cos(_theta), (moonY - 18) + (6) * sin(_theta));
    }
    glEnd();

    // Plotting city buildings
    for (int i = 0; i < 12; i++) {
        float col = 0.5f * (1.0f - s4_dayNight) + 0.1f * s4_dayNight; 
        glColor3f(col, col, col + 0.05f);
        glBegin(GL_QUADS);
    glVertex2f(s4_buildings_x[i], 200);
    glVertex2f(s4_buildings_x[i] + s4_buildings_w[i], 200);
    glVertex2f(s4_buildings_x[i] + s4_buildings_w[i], 200 + s4_buildings_h[i]);
    glVertex2f(s4_buildings_x[i], 200 + s4_buildings_h[i]);
    glEnd();
        
        if (s4_buildings_lit[i] && s4_dayNight > 0.5f) 
            glColor4f(1.0f, 0.9f, 0.5f, (s4_dayNight - 0.5f) * 2.0f); 
        else 
            glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
            
        for (float wy = 220; wy < 200 + s4_buildings_h[i] - 15; wy += 25) {
            for (float wx = s4_buildings_x[i] + 10; wx < s4_buildings_x[i] + s4_buildings_w[i] - 10; wx += 20) {
                glBegin(GL_QUADS);
    glVertex2f(wx, wy);
    glVertex2f(wx + 10, wy);
    glVertex2f(wx + 10, wy + 15);
    glVertex2f(wx, wy + 15);
    glEnd(); 
            }
        }
    }

    // Plotting shops and trees
    glColor3f(0.15f, 0.15f, 0.15f); 
    glBegin(GL_QUADS);
    glVertex2f(0, 60);
    glVertex2f(1200, 60);
    glVertex2f(1200, 200);
    glVertex2f(0, 200);
    glEnd();
    drawS4Tree(20, 60);
    drawS4Shop(80, 60, 0.4f, 0.6f, 0.7f);
    drawS4Tree(160, 60);
    drawS4Shop(210, 60, 0.7f, 0.5f, 0.4f);
    drawS4Tree(300, 60);
    drawS4Tree(350, 60);
    drawS4Shop(400, 60, 0.5f, 0.7f, 0.5f);
    drawS4Tree(480, 60);
    drawS4Shop(540, 60, 0.6f, 0.4f, 0.6f);
    drawS4Tree(620, 60);
    drawS4Tree(670, 60);
    drawS4Shop(720, 60, 0.8f, 0.6f, 0.4f);
    drawS4Tree(800, 60);
    drawS4Shop(850, 60, 0.4f, 0.4f, 0.8f);
    drawS4Tree(940, 60);
    drawS4Shop(1000, 60, 0.5f, 0.7f, 0.8f);
    drawS4Tree(1080, 60);
    drawS4Tree(1130, 60);

    // Plotting road and cars
    drawS4Road();
    drawS4Car1();
    drawS4Car2();
    drawS4Car3();

    // Plotting clouds
    float cloudBright = global_isRain ? 0.4f : 0.7f;
    glPushMatrix(); glTranslatef(200, 600, 0); glScalef(2.0f, 2.0f, 1.0f); glColor4f(cloudBright, cloudBright, cloudBright, 0.8f); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((-20) + (20) * cos(_theta), (0) + (20) * sin(_theta));
    }
    glEnd(); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((0) + (25) * cos(_theta), (10) + (25) * sin(_theta));
    }
    glEnd(); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((20) + (20) * cos(_theta), (0) + (20) * sin(_theta));
    }
    glEnd(); glPopMatrix();
    glPushMatrix(); glTranslatef(500, 550, 0); glScalef(1.5f, 1.5f, 1.0f); glColor4f(cloudBright, cloudBright, cloudBright, 0.8f); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((-20) + (20) * cos(_theta), (0) + (20) * sin(_theta));
    }
    glEnd(); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((0) + (25) * cos(_theta), (10) + (25) * sin(_theta));
    }
    glEnd(); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((20) + (20) * cos(_theta), (0) + (20) * sin(_theta));
    }
    glEnd(); glPopMatrix();
    glPushMatrix(); glTranslatef(800, 620, 0); glScalef(2.5f, 2.5f, 1.0f); glColor4f(cloudBright, cloudBright, cloudBright, 0.8f); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((-20) + (20) * cos(_theta), (0) + (20) * sin(_theta));
    }
    glEnd(); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((0) + (25) * cos(_theta), (10) + (25) * sin(_theta));
    }
    glEnd(); glBegin(GL_POLYGON);
    for (int _i = 0; _i < 50; _i++) {
        float _theta = 2.0f * 3.14159265359f * float(_i) / float(50);
        glVertex2f((20) + (20) * cos(_theta), (0) + (20) * sin(_theta));
    }
    glEnd(); glPopMatrix();

    drawUniversalRain();
}


void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}
void drawHUD() {
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, 1200, 0, 700);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glEnable(GL_BLEND); glColor4f(0.0f, 0.0f, 0.0f, 0.7f); glBegin(GL_QUADS);
    glVertex2f(10, 10);
    glVertex2f(1190, 10);
    glVertex2f(1190, 40);
    glVertex2f(10, 40);
    glEnd(); glDisable(GL_BLEND);
    glColor3f(1.0f, 1.0f, 1.0f); char hudText[256] = "";

    switch (currentScene) {
    case 1: sprintf(hudText, "Scene 1, Left Click: Bomb"); break;
    case 2: sprintf(hudText, "Scene 2, Port City | Left Click: Submarine Bomb"); break;
    case 3: sprintf(hudText, "Scene 3, A/S: Move Car, Mouse: Toggle Light"); break;
    case 4: sprintf(hudText, "Scene 4, R: Rain"); break;
    }

    drawText(20, 20, hudText);
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
}

void initData() {
    for (int i = 0; i < 20; i++) {
        s2_bubble_x[i] = 0;
        s2_bubble_y[i] = 0;
        s2_bubble_alpha[i] = 1.0f;
        s2_bubble_active[i] = 0;
    }
    for (int i = 0; i < 40; i++) {
        s4_stars_x[i] = (float)(rand() % 1200);
        s4_stars_y[i] = (float)(400 + rand() % 300);
        s4_stars_size[i] = (float)(1 + rand() % 3);
        s4_stars_phase[i] = (float)(rand() % 10);
    }
    for (int i = 0; i < 200; i++) {
        s4_rain_x[i] = (float)(rand() % 1300);
        s4_rain_y[i] = (float)(rand() % 700);
        s4_rain_speed[i] = (float)(10 + rand() % 15);
        s4_rain_len[i] = (float)(10 + rand() % 20);
    }
    
    float bx = 300;
    for (int i = 0; i < 12; i++) {
        s4_buildings_x[i] = bx;
        s4_buildings_w[i] = (float)(60 + rand() % 50);
        s4_buildings_h[i] = (float)(200 + rand() % 200);
        s4_buildings_lit[i] = (rand() % 2 == 0) ? 1 : 0;
        bx += s4_buildings_w[i] + 10;
    }

    s2_b_x[0] = 45; s2_b_y[0] = 160; s2_b_width[0] = 100; s2_b_height[0] = 150; s2_b_r[0] = 0.72f; s2_b_g[0] = 0.43f; s2_b_b[0] = 0.35f; s2_b_floors[0] = 5; s2_b_type[0] = 0; s2_b_progress[0] = 1.0f; s2_b_isDestroyed[0] = 0;
    s2_b_x[1] = 175; s2_b_y[1] = 160; s2_b_width[1] = 110; s2_b_height[1] = 190; s2_b_r[1] = 0.45f; s2_b_g[1] = 0.65f; s2_b_b[1] = 0.75f; s2_b_floors[1] = 6; s2_b_type[1] = 1; s2_b_progress[1] = 1.0f; s2_b_isDestroyed[1] = 0;
    s2_b_x[2] = 310; s2_b_y[2] = 160; s2_b_width[2] = 90; s2_b_height[2] = 140; s2_b_r[2] = 0.72f; s2_b_g[2] = 0.58f; s2_b_b[2] = 0.46f; s2_b_floors[2] = 5; s2_b_type[2] = 0; s2_b_progress[2] = 1.0f; s2_b_isDestroyed[2] = 0;
    s2_b_x[3] = 670; s2_b_y[3] = 160; s2_b_width[3] = 140; s2_b_height[3] = 150; s2_b_r[3] = 0.48f; s2_b_g[3] = 0.68f; s2_b_b[3] = 0.76f; s2_b_floors[3] = 5; s2_b_type[3] = 1; s2_b_progress[3] = 1.0f; s2_b_isDestroyed[3] = 0;
    s2_b_x[4] = 420; s2_b_y[4] = 160; s2_b_width[4] = 115; s2_b_height[4] = 360; s2_b_r[4] = 0.35f; s2_b_g[4] = 0.55f; s2_b_b[4] = 0.68f; s2_b_floors[4] = 12; s2_b_type[4] = 2; s2_b_progress[4] = 0.65f; s2_b_isDestroyed[4] = 0;
    s2_b_x[5] = 550; s2_b_y[5] = 160; s2_b_width[5] = 105; s2_b_height[5] = 230; s2_b_r[5] = 0.70f; s2_b_g[5] = 0.55f; s2_b_b[5] = 0.43f; s2_b_floors[5] = 7; s2_b_type[5] = 0; s2_b_progress[5] = 1.0f; s2_b_isDestroyed[5] = 0;
    s2_b_x[6] = 830; s2_b_y[6] = 160; s2_b_width[6] = 140; s2_b_height[6] = 200; s2_b_r[6] = 0.45f; s2_b_g[6] = 0.66f; s2_b_b[6] = 0.76f; s2_b_floors[6] = 6; s2_b_type[6] = 1; s2_b_progress[6] = 1.0f; s2_b_isDestroyed[6] = 0;
    s2_b_x[7] = 995; s2_b_y[7] = 160; s2_b_width[7] = 115; s2_b_height[7] = 280; s2_b_r[7] = 0.72f; s2_b_g[7] = 0.62f; s2_b_b[7] = 0.52f; s2_b_floors[7] = 9; s2_b_type[7] = 0; s2_b_progress[7] = 1.0f; s2_b_isDestroyed[7] = 0;
    s2_b_x[8] = 1130; s2_b_y[8] = 160; s2_b_width[8] = 120; s2_b_height[8] = 400; s2_b_r[8] = 0.42f; s2_b_g[8] = 0.61f; s2_b_b[8] = 0.70f; s2_b_floors[8] = 14; s2_b_type[8] = 1; s2_b_progress[8] = 0.55f; s2_b_isDestroyed[8] = 0;
    s2_b_x[9] = 1280; s2_b_y[9] = 160; s2_b_width[9] = 95; s2_b_height[9] = 420; s2_b_r[9] = 0.52f; s2_b_g[9] = 0.63f; s2_b_b[9] = 0.70f; s2_b_floors[9] = 15; s2_b_type[9] = 1; s2_b_progress[9] = 0.40f; s2_b_isDestroyed[9] = 0;
    s2_b_x[10]= 1410; s2_b_y[10] = 160; s2_b_width[10] = 95; s2_b_height[10] = 420; s2_b_r[10] = 0.12f; s2_b_g[10] = 0.48f; s2_b_b[10] = 0.66f; s2_b_floors[10] = 15; s2_b_type[10] = 1; s2_b_progress[10] = 0.25f; s2_b_isDestroyed[10] = 0;
    s2_b_x[11]= 1520; s2_b_y[11] = 160; s2_b_width[11] = 80; s2_b_height[11] = 240; s2_b_r[11] = 0.68f; s2_b_g[11] = 0.42f; s2_b_b[11] = 0.38f; s2_b_floors[11] = 7; s2_b_type[11] = 0; s2_b_progress[11] = 1.0f; s2_b_isDestroyed[11] = 0;
}

void resetScene(int scene) {
    if (scene == 1) { 
        s1_planeActive = false; s1_planeX = -150.0f; s1_bombState = 0; s1_isDestroyed = false; 
    }
    else if (scene == 2) { 
        s2_subX = -100.0f; s2_subY = 60.0f; s2_bombState = 0; s2_explRadius = 0; s2_craneLift = 0.0f; s2_craneDir = 1.0f;
        for (int i = 0; i < 20; i++) s2_bubble_active[i] = 0;
        initData(); 
    }
}

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
    if (global_isDay) { if (global_brightness < 1.0f) global_brightness += 0.015f; if (global_sunY < 400.0f) global_sunY += 2.0f; } 
    else { if (global_brightness > 0.0f) global_brightness -= 0.015f; if (global_sunY > -50.0f) global_sunY -= 2.0f; }
    
    s1_birdPhase += 0.1f; s1_cloudOffset += 0.3f; 
    if (global_isRain) global_rainOffset += 20.0f; 
    
    if (currentScene == 1 && s1_planeActive) {
        s1_planeX += 10.0f;
        if (s1_planeX > 450.0f && s1_bombState == 0) {
            s1_bombState = 1; s1_bombX = s1_planeX; s1_bombY = s1_planeY - 20.0f;
        }
        if (s1_bombState == 1) {
            s1_bombY -= 12.0f; s1_bombX += 3.0f; 
            if (s1_bombY <= 250.0f) { 
                s1_bombState = 2; s1_explRadius = 0.0f; s1_isDestroyed = true;
                for (int i = 0; i < 20; i++) {
                    s1_debris_x[i] = s1_bombX;
                    s1_debris_y[i] = s1_bombY;
                    s1_debris_vx[i] = (float)(-10.0f + (rand() % 20));
                    s1_debris_vy[i] = (float)(5.0f + (rand() % 15));
                }
            }
        } else if (s1_bombState == 2) {
            s1_explRadius += 12.0f;
            for (int i = 0; i < 20; i++) { s1_debris_x[i] += s1_debris_vx[i]; s1_debris_y[i] += s1_debris_vy[i]; s1_debris_vy[i] -= 0.5f; }
            if (s1_explRadius > 350.0f) s1_bombState = 3;
        }
        if (s1_planeX > 1400.0f) { s1_planeActive = false; }
    }
    
    if (currentScene == 2) {
        s2_wavePhase += 0.05f; s2_propAngle += 20.0f; s2_firePhase += 0.1f; s2_smokeOffset += 0.5f; if(s2_smokeOffset > 20.0f) s2_smokeOffset = 0.0f;
        s2_craneLift += s2_craneDir * 1.5f; if (s2_craneLift >= 250.0f) s2_craneDir = -1.0f; if (s2_craneLift <= 0.0f) s2_craneDir = 1.0f;  
        s2_subX += 2.0f; if (s2_subX > 1300) s2_subX = -100; if (s2_subY > 80) s2_subY -= 1.0f; 
        
        s2_boat1_x += 1.5f;
        if (s2_boat1_x > 1300) s2_boat1_x = -100;
        s2_boat2_x -= 1.0f;
        if (s2_boat2_x < -100) s2_boat2_x = 1300;

        if (rand() % 5 == 0) {
            for (int i = 0; i < 20; i++) {
                if (s2_bubble_active[i] == 0) {
                    s2_bubble_x[i] = s2_subX - 80;
                    s2_bubble_y[i] = s2_subY;
                    s2_bubble_alpha[i] = 1.0f;
                    s2_bubble_active[i] = 1;
                    break;
                }
            }
        }
        for (int i = 0; i < 20; i++) { if (s2_bubble_active[i] == 1) { s2_bubble_y[i] += 3.0f; if (s2_bubble_y[i] > 150) s2_bubble_active[i] = 0; } }

        if (s2_bombState == 1) { 
            float dx = s2_targetX - s2_bombX; float dy = s2_targetY - s2_bombY; float dist = sqrt(dx * dx + dy * dy);
            if (dist < 15.0f) { 
                s2_bombState = 2; s2_explRadius = 0.0f;
                for (int i = 0; i < 12; i++) {
                    s2_debris_x[i] = s2_bombX;
                    s2_debris_y[i] = s2_bombY;
                    s2_debris_vx[i] = (float)(-6.0f + (rand() % 12));
                    s2_debris_vy[i] = (float)(3.0f + (rand() % 10));
                }
                float mappedX = s2_targetX / 0.7f; 
                for(int i = 0; i < 12; i++) {
                    if(mappedX >= s2_b_x[i] - 10 && mappedX <= s2_b_x[i] + s2_b_width[i] + 10) {
                        s2_b_progress[i] -= 0.35f; s2_b_isDestroyed[i] = 1; if (s2_b_progress[i] < 0.15f) s2_b_progress[i] = 0.15f; 
                    }
                }
            } else { s2_bombX += (dx / dist) * 15.0f; s2_bombY += (dy / dist) * 15.0f; }
        }
        else if (s2_bombState == 2) { 
            s2_explRadius += 4.0f; for (int i = 0; i < 12; i++) { s2_debris_x[i] += s2_debris_vx[i]; s2_debris_y[i] += s2_debris_vy[i]; s2_debris_vy[i] -= 0.4f; }
            if (s2_explRadius > 150.0f) s2_bombState = 3; 
        }
    }
    
    if (currentScene == 3) {
        s3_cloudOffset += 0.5f;
        if (s3_cloudOffset > 1200.0f) s3_cloudOffset = -400.0f;
        
        s3_treeAngle += 0.1f;
        if (s3_treeAngle > 2 * PI) s3_treeAngle -= 2 * PI;
        
        s3_wheelAngle += s3_carSpeed * -100.0f; 
        s3_carX += s3_carSpeed;
        
        if (s3_carX > 4.5f) s3_carX = -2.3f;
        if (s3_carX < -2.3f) s3_carX = 4.5f;
        
        if (global_isRain) {
            if (s3_dropCount < S3_MAX_DROPS && (rand() % 2) == 0) {
                s3_dropX[s3_dropCount] = (float)(rand() % 1200);
                s3_dropY[s3_dropCount] = 700.0f;
                s3_dropSpeed[s3_dropCount] = 6.0f + (rand() % 100 / 100.0f) * 4.0f;
                s3_dropCount++;
            }
            for (int i = 0; i < s3_dropCount; ) {
                s3_dropY[i] -= s3_dropSpeed[i];
                if (s3_dropY[i] <= s3_groundY + 3.0f) {
                    if (s3_bubbleCount < S3_MAX_BUBBLES) {
                        s3_bubbleX[s3_bubbleCount] = s3_dropX[i];
                        s3_bubbleY[s3_bubbleCount] = s3_groundY + 3.0f;
                        s3_bubbleRadius[s3_bubbleCount] = 3.0f;
                        s3_bubbleAlpha[s3_bubbleCount] = 1.0f;
                        s3_bubbleActive[s3_bubbleCount] = 1;
                        s3_bubbleCount++;
                    }
                    s3_dropX[i] = s3_dropX[s3_dropCount - 1];
                    s3_dropY[i] = s3_dropY[s3_dropCount - 1];
                    s3_dropSpeed[i] = s3_dropSpeed[s3_dropCount - 1];
                    s3_dropCount--;
                } else {
                    i++;
                }
            }
            for (int i = 0; i < s3_bubbleCount; ++i) {
                if (s3_bubbleActive[i]) {
                    s3_bubbleRadius[i] += 1.5f;
                    s3_bubbleAlpha[i] -= 0.04f;
                    if (s3_bubbleAlpha[i] <= 0.0f) s3_bubbleActive[i] = 0;
                }
            }
        }
    }

    if (currentScene == 4) {
        s4_car1_x += 2.0f;
        if (s4_car1_x > 1300) s4_car1_x = -100;
        
        s4_car2_x += 3.0f;
        if (s4_car2_x > 1300) s4_car2_x = -100;

        s4_car3_x -= 2.5f;
        if (s4_car3_x < -100) s4_car3_x = 1300;
    }

    glutPostRedisplay(); 
    glutTimerFunc(16, update, 0); 
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0); 
    // Scene switching removed for individual scene file

    if (key == 'd' || key == 'D') global_isDay = !global_isDay; 
    if (key == 'r' || key == 'R') global_isRain = !global_isRain;

    if (currentScene == 3) {
        if (key == 'a' || key == 'A') s3_carSpeed = -0.04f;
        if (key == 's' || key == 'S') s3_carSpeed = 0.04f;
    }
}

void mouse(int button, int state, int x, int y) {
    if (currentScene == 1 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!s1_planeActive) {
            s1_planeActive = true; s1_planeX = -150.0f; s1_bombState = 0; s1_isDestroyed = false; 
        }
    }
    if (currentScene == 2 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (s2_bombState == 0 || s2_bombState == 3) {
            s2_targetX = (float)x; s2_targetY = (float)(windowHeight - y); s2_bombState = 1; s2_bombX = s2_subX; s2_bombY = s2_subY + 20.0f; 
        }
    }
    if (currentScene == 3 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float scaledX = (float)x / windowWidth * 900.0f;
        float scaledY = (1.0f - (float)y / windowHeight) * 600.0f;
        
        if (scaledX > 600 && scaledX < 750 && scaledY > 150 && scaledY < 300) {
            s3_lightOn = !s3_lightOn;
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Computer Graphics G-9");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); 
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initData(); 
    resetScene(currentScene);

    glutDisplayFunc(display); 
    glutKeyboardFunc(keyboard); 
    glutMouseFunc(mouse); 
    glutTimerFunc(16, update, 0); 
    
    glutMainLoop(); 
    return 0;
}






