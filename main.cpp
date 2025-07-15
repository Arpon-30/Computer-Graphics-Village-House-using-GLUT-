#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265358979323846


int currentScene = 0;
int isNight = 0;
float transitionFactor = 0.0f;
float transitionSpeed = 0.02f;

// Variables for car animation
GLfloat car1_position = 0.0f;
GLfloat car2_position = 0.0f;
GLfloat car_speed = 0.02f;
int car_moving = 0;

// Variables for sun/moon animation
GLfloat sun_position_x = -0.7f;
GLfloat sun_speed = 0.002f;

// Variables for cloud animation
GLfloat cloud_positions[6] = {-0.8f, -0.6f, -0.4f, -0.2f, 0.2f, 0.6f};
GLfloat cloud_speed = 0.002f;

// Variables for man animation
GLfloat man_position_x = -0.4f;
GLfloat man_position_y = 0.0f;
GLfloat man_speed = 0.002f;
int man_moving = 0;
int man_hand_raised = 0;

// --------------------- UTILS ---------------------
void renderBitmapString(float x, float y, float z, void* font, const char* string) {
    const char* c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void drawCircle(float x, float y, float radius) {
    int triangleAmount = 100;
    float twicePi = 2.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= triangleAmount; i++) {
        glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)),
                   y + (radius * sin(i * twicePi / triangleAmount)));
    }
    glEnd();
}

// --------------------- INSTRUCTIONS SCREEN ---------------------
void displayScene0() {
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Updated instructions, removing lines for keys 1-5
    const char* instructions[] = {
        "Instructions : ",
        "Press-> D/d To view the day view of the scenario.",
        "Press-> N/n To view the night view of the scenario.",
        "Press->  >(right arrow) To view the Car Run Again press Car Stop of the scenario.",
        "Press-> Space Button To view the Car Run Again press Car Stop of the scenario.",
        "Press-> H  to view the Man Arise hand Again Press Man Down Hand of the scenario.",
        "Press-> M  to view the Man Move of the scenario.",
        "Press->0 To view the instructions."
    };

    float startX = 50.0f, startY = 700.0f, lineSpacing = 50.0f;
    for (int i = 0; i < 8; i++) {
        renderBitmapString(startX, startY - i * lineSpacing, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, instructions[i]);
    }

    glFlush();
    glutSwapBuffers();
}

// --------------------- STARS ---------------------
void draw_stars() {
    glColor3f(1.0f, 1.0f, 1.0f);
    int triangleAmount = 20;
    float twicePi = 2.0f * PI;

    float star_positions[][2] = {
        {-0.8f, 0.8f}, {-0.5f, 0.7f}, {-0.2f, 0.85f}, {0.0f, 0.75f},
        {0.3f, 0.8f}, {0.5f, 0.7f}, {0.8f, 0.85f}, {-0.6f, 0.9f},
        {-0.3f, 0.65f}, {0.1f, 0.9f}
    };
    float radius = 0.01f;

    for (int i = 0; i < 10; i++) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(star_positions[i][0], star_positions[i][1]);
        for (int j = 0; j <= triangleAmount; j++) {
            glVertex2f(
                star_positions[i][0] + (radius * cos(j * twicePi / triangleAmount)),
                star_positions[i][1] + (radius * sin(j * twicePi / triangleAmount))
            );
        }
        glEnd();
    }
}

// --------------------- TREE ---------------------
void ndcCircle(float radiusX, float radiusY, float centerX, float centerY) {
    int triangleAmount = 100;
    float twicePi = 2.0f * PI;

    centerX -= 264.5f;
    centerY -= 150.0f;
    float xNDC = (centerX / 640.0f) - 1.0f;
    float yNDC = (centerY / 400.0f) - 1.0f;
    float rX = radiusX / 640.0f;
    float rY = radiusY / 400.0f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xNDC, yNDC);
    for (int i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            xNDC + (rX * cos(i * twicePi / triangleAmount)),
            yNDC + (rY * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}

void draw_tree() {
    glColor3ub(53, 127, 11);
    ndcCircle(40, 40, 930, 695);
    ndcCircle(30, 30, 880, 660);
    ndcCircle(30, 30, 850, 620);
    ndcCircle(30, 30, 830, 590);
    ndcCircle(30, 30, 870, 560);
    ndcCircle(30, 30, 920, 540);
    ndcCircle(30, 30, 970, 550);
    ndcCircle(35, 35, 1010, 560);
    ndcCircle(35, 35, 1060, 550);
    ndcCircle(35, 35, 1110, 550);
    ndcCircle(35, 35, 1150, 570);
    ndcCircle(35, 35, 1150, 600);
    ndcCircle(35, 35, 1120, 640);
    ndcCircle(35, 35, 1070, 680);
    ndcCircle(45, 45, 1010, 700);

    glColor3ub(98, 169, 41); ndcCircle(35, 35, 1110, 600);
    glColor3ub(53, 127, 11); ndcCircle(30, 30, 1090, 600);
    glColor3ub(98, 169, 41); ndcCircle(30, 30, 1070, 600);
    ndcCircle(25, 25, 1110, 640);
    ndcCircle(30, 30, 1060, 670);
    ndcCircle(45, 45, 1010, 670);
    glColor3ub(53, 127, 11);
    ndcCircle(40, 30, 1060, 630);
    glColor3ub(98, 169, 41);
    ndcCircle(30, 30, 1060, 640);
    glColor3ub(142, 205, 68);
    ndcCircle(40, 25, 1010, 690);
    glColor3ub(98, 169, 41);
    ndcCircle(25, 25, 1050, 580);
    ndcCircle(30, 30, 1010, 570);
    ndcCircle(30, 30, 940, 660);
    ndcCircle(30, 30, 900, 620);
    ndcCircle(30, 30, 880, 590);
    ndcCircle(30, 30, 910, 570);
    ndcCircle(30, 30, 950, 570);
    ndcCircle(30, 30, 950, 600);
    ndcCircle(30, 30, 990, 600);
    ndcCircle(30, 30, 1010, 610);
    ndcCircle(30, 30, 970, 640);
    glColor3ub(53, 127, 11); ndcCircle(35, 35, 900, 600);
    glColor3ub(142, 205, 68); ndcCircle(30, 30, 920, 600);
    glColor3ub(53, 127, 11); ndcCircle(35, 35, 990, 600);
    glColor3ub(142, 205, 68); ndcCircle(30, 30, 1000, 600);
    glColor3ub(209, 245, 134);
    ndcCircle(15, 10, 950, 640);
    ndcCircle(15, 10, 930, 570);
    ndcCircle(10, 5, 1010, 690);

    glBegin(GL_POLYGON);
    glColor3ub(177, 125, 76);
    glVertex2f(0.1641f, -0.0500f);
    glVertex2f(0.1328f, 0.0000f);
    glVertex2f(0.1406f, 0.0125f);
    glVertex2f(0.1719f, -0.0250f);
    glVertex2f(0.1875f, 0.0250f);
    glVertex2f(0.1953f, 0.0125f);
    glVertex2f(0.1797f, -0.0500f);
    glVertex2f(0.1797f, -0.2000f);
    glVertex2f(0.1641f, -0.2000f);
    glEnd();
}

void draw_tree_with_small_trunk() {
    glColor3ub(53, 127, 11);
    ndcCircle(40, 40, 930, 695);
    ndcCircle(30, 30, 880, 660);
    ndcCircle(30, 30, 850, 620);
    ndcCircle(30, 30, 830, 590);
    ndcCircle(30, 30, 870, 560);
    ndcCircle(30, 30, 920, 540);
    ndcCircle(30, 30, 970, 550);
    ndcCircle(35, 35, 1010, 560);
    ndcCircle(35, 35, 1060, 550);
    ndcCircle(35, 35, 1110, 550);
    ndcCircle(35, 35, 1150, 570);
    ndcCircle(35, 35, 1150, 600);
    ndcCircle(35, 35, 1120, 640);
    ndcCircle(35, 35, 1070, 680);
    ndcCircle(45, 45, 1010, 700);
    glColor3ub(98, 169, 41); ndcCircle(35, 35, 1110, 600);
    glColor3ub(53, 127, 11); ndcCircle(30, 30, 1090, 600);
    glColor3ub(98, 169, 41); ndcCircle(30, 30, 1070, 600);
    ndcCircle(25, 25, 1110, 640);
    ndcCircle(30, 30, 1060, 670);
    ndcCircle(45, 45, 1010, 670);
    glColor3ub(53, 127, 11);
    ndcCircle(40, 30, 1060, 630);
    glColor3ub(98, 169, 41);
    ndcCircle(30, 30, 1060, 640);
    glColor3ub(142, 205, 68);
    ndcCircle(40, 25, 1010, 690);
    glColor3ub(98, 169, 41);
    ndcCircle(25, 25, 1050, 580);
    ndcCircle(30, 30, 1010, 570);
    ndcCircle(30, 30, 940, 660);
    ndcCircle(30, 30, 900, 620);
    ndcCircle(30, 30, 880, 590);
    ndcCircle(30, 30, 910, 570);
    ndcCircle(30, 30, 950, 570);
    ndcCircle(30, 30, 950, 600);
    ndcCircle(30, 30, 990, 600);
    ndcCircle(30, 30, 1010, 610);
    ndcCircle(30, 30, 970, 640);
    glColor3ub(53, 127, 11); ndcCircle(37, 37, 900, 600);
    glColor3ub(142, 205, 68); ndcCircle(30, 30, 920, 600);
    glColor3ub(53, 127, 11); ndcCircle(35, 35, 990, 600);
    glColor3ub(142, 205, 68); ndcCircle(30, 30, 1000, 600);
    glColor3ub(209, 245, 134);
    ndcCircle(15, 10, 950, 640);
    ndcCircle(15, 10, 930, 570);
    ndcCircle(10, 5, 1010, 600);

    glBegin(GL_POLYGON);
    glColor3ub(177, 125, 76);
    glVertex2f(0.1484f, -0.1013f);
    glVertex2f(0.1484f, -0.0638f);
    glVertex2f(0.1523f, -0.0544f);
    glVertex2f(0.1641f, -0.0794f);
    glVertex2f(0.1672f, -0.0450f);
    glVertex2f(0.1750f, -0.0544f);
    glVertex2f(0.1641f, -0.1013f);
    glVertex2f(0.1641f, -0.1550f);
    glVertex2f(0.1484f, -0.1550f);
    glEnd();
}

// --------------------- CAR ---------------------
void draw_car() {
    glBegin(GL_POLYGON);
    glColor3f(0.8f, 0.0f, 0.0f);
    glVertex2f(-0.22f, -0.85f);
    glVertex2f(-0.18f, -0.83f);
    glVertex2f(-0.18f, -0.75f);
    glVertex2f(0.18f, -0.75f);
    glVertex2f(0.18f, -0.83f);
    glVertex2f(0.22f, -0.85f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.22f, -0.85f);
    glVertex2f(0.18f, -0.83f);
    glVertex2f(0.18f, -0.75f);
    glVertex2f(-0.18f, -0.75f);
    glVertex2f(-0.18f, -0.83f);
    glVertex2f(-0.22f, -0.85f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(-0.12f, -0.75f);
    glVertex2f(0.12f, -0.75f);
    glVertex2f(0.08f, -0.67f);
    glVertex2f(-0.08f, -0.67f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.7f, 0.9f, 1.0f);
    glVertex2f(0.02f, -0.75f);
    glVertex2f(0.12f, -0.75f);
    glVertex2f(0.08f, -0.67f);
    glVertex2f(0.04f, -0.67f);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.02f, -0.75f);
    glVertex2f(0.08f, -0.67f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.7f, 0.9f, 1.0f);
    glVertex2f(-0.12f, -0.75f);
    glVertex2f(-0.02f, -0.75f);
    glVertex2f(-0.04f, -0.67f);
    glVertex2f(-0.08f, -0.67f);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.12f, -0.75f);
    glVertex2f(-0.04f, -0.67f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 0.0f);
    float centerX = -0.12f, centerY = -0.86f, radius = 0.048f;
    for (int i = 0; i < 24; i++) {
        float angle = 2.0f * PI * i / 24;
        glVertex2f(centerX + radius * cos(angle), centerY + radius * sin(angle));
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.7f, 0.7f, 0.7f);
    radius = 0.024f;
    for (int i = 0; i < 24; i++) {
        float angle = 2.0f * PI * i / 24;
        glVertex2f(centerX + radius * cos(angle), centerY + radius * sin(angle));
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 0.0f);
    centerX = 0.12f;
    radius = 0.048f;
    for (int i = 0; i < 24; i++) {
        float angle = 2.0f * PI * i / 24;
        glVertex2f(centerX + radius * cos(angle), centerY + radius * sin(angle));
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.7f, 0.7f, 0.7f);
    radius = 0.024f;
    for (int i = 0; i < 24; i++) {
        float angle = 2.0f * PI * i / 24;
        glVertex2f(centerX + radius * cos(angle), centerY + radius * sin(angle));
    }
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex2f(0.19f, -0.76f);
    glVertex2f(0.21f, -0.76f);
    glVertex2f(0.21f, -0.78f);
    glVertex2f(0.19f, -0.78f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(0.19f, -0.78f);
    glVertex2f(0.21f, -0.78f);
    glVertex2f(0.21f, -0.80f);
    glVertex2f(0.19f, -0.80f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.21f, -0.76f);
    glVertex2f(-0.19f, -0.76f);
    glVertex2f(-0.19f, -0.78f);
    glVertex2f(-0.21f, -0.78f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.21f, -0.78f);
    glVertex2f(-0.19f, -0.78f);
    glVertex2f(-0.19f, -0.80f);
    glVertex2f(-0.21f, -0.80f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex2f(-0.22f, -0.85f);
    glVertex2f(-0.19f, -0.85f);
    glVertex2f(-0.19f, -0.87f);
    glVertex2f(-0.22f, -0.87f);
    glEnd();
}

void draw_car_mirrored() {
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.6f, 0.0f);
    glVertex2f(0.22f, -0.85f);
    glVertex2f(0.18f, -0.83f);
    glVertex2f(0.18f, -0.75f);
    glVertex2f(-0.18f, -0.75f);
    glVertex2f(-0.18f, -0.83f);
    glVertex2f(-0.22f, -0.85f);
    glColor3f(0.0f, 0.8f, 0.0f);
    glVertex2f(-0.22f, -0.85f);
    glVertex2f(-0.18f, -0.83f);
    glVertex2f(-0.18f, -0.75f);
    glVertex2f(0.18f, -0.75f);
    glVertex2f(0.18f, -0.83f);
    glVertex2f(0.22f, -0.85f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex2f(0.12f, -0.75f);
    glVertex2f(-0.12f, -0.75f);
    glVertex2f(-0.08f, -0.67f);
    glVertex2f(0.08f, -0.67f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.7f, 0.9f, 1.0f);
    glVertex2f(-0.02f, -0.75f);
    glVertex2f(-0.12f, -0.75f);
    glVertex2f(-0.08f, -0.67f);
    glVertex2f(-0.04f, -0.67f);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(-0.02f, -0.75f);
    glVertex2f(-0.08f, -0.67f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.7f, 0.9f, 1.0f);
    glVertex2f(0.12f, -0.75f);
    glVertex2f(0.02f, -0.75f);
    glVertex2f(0.04f, -0.67f);
    glVertex2f(0.08f, -0.67f);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.12f, -0.75f);
    glVertex2f(0.04f, -0.67f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 0.0f);
    float centerX = 0.12f, centerY = -0.86f, radius = 0.048f;
    for (int i = 0; i < 24; i++) {
        float angle = 2.0f * PI * i / 24;
        glVertex2f(centerX + radius * cos(angle), centerY + radius * sin(angle));
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.7f, 0.7f, 0.7f);
    radius = 0.024f;
    for (int i = 0; i < 24; i++) {
        float angle = 2.0f * PI * i / 24;
        glVertex2f(centerX + radius * cos(angle), centerY + radius * sin(angle));
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 0.0f);
    centerX = -0.12f;
    radius = 0.048f;
    for (int i = 0; i < 24; i++) {
        float angle = 2.0f * PI * i / 24;
        glVertex2f(centerX + radius * cos(angle), centerY + radius * sin(angle));
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.7f, 0.7f, 0.7f);
    radius = 0.024f;
    for (int i = 0; i < 24; i++) {
        float angle = 2.0f * PI * i / 24;
        glVertex2f(centerX + radius * cos(angle), centerY + radius * sin(angle));
    }
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex2f(-0.21f, -0.76f);
    glVertex2f(-0.19f, -0.76f);
    glVertex2f(-0.19f, -0.78f);
    glVertex2f(-0.21f, -0.78f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.21f, -0.78f);
    glVertex2f(-0.19f, -0.78f);
    glVertex2f(-0.19f, -0.80f);
    glVertex2f(-0.21f, -0.80f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.19f, -0.76f);
    glVertex2f(0.21f, -0.76f);
    glVertex2f(0.21f, -0.78f);
    glVertex2f(0.19f, -0.78f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(0.19f, -0.78f);
    glVertex2f(0.21f, -0.78f);
    glVertex2f(0.21f, -0.80f);
    glVertex2f(0.19f, -0.80f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex2f(0.19f, -0.85f);
    glVertex2f(0.22f, -0.85f);
    glVertex2f(0.22f, -0.87f);
    glVertex2f(0.19f, -0.87f);
    glEnd();
}

// --------------------- SUN AND MOON ---------------------
void draw_sun_moon() {
    int triangleAmount = 100;
    float twicePi = 2.0f * PI;
    float centerX = sun_position_x, centerY = 0.8f;

    glBegin(GL_TRIANGLE_FAN);
    if (isNight) {
        glColor3f(1.0f, 1.0f, 1.0f);
    } else {
        glColor3f(1.0f, 1.0f, 0.0f);
    }
    glVertex2f(centerX, centerY);
    if (isNight) {
        glColor3f(0.9f, 0.9f, 0.9f);
    } else {
        glColor3f(1.0f, 0.5f, 0.0f);
    }
    for (int i = 0; i <= triangleAmount; i++) {
        glVertex2f(
            centerX + (0.1f * cos(i * twicePi / triangleAmount)),
            centerY + (0.1f * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}

void draw_cloud(float centerX, float centerY, float scale) {
    glColor4f(0.9f, 0.95f, 1.0f, 0.9f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY);
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * PI * i / 100;
        glVertex2f(
            centerX + (0.08f * scale * cos(angle)),
            centerY + (0.06f * scale * sin(angle))
        );
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX - 0.06f * scale, centerY);
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * PI * i / 100;
        glVertex2f(
            centerX - 0.06f * scale + (0.06f * scale * cos(angle)),
            centerY + (0.05f * scale * sin(angle))
        );
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX + 0.06f * scale, centerY);
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * PI * i / 100;
        glVertex2f(
            centerX + 0.06f * scale + (0.06f * scale * cos(angle)),
            centerY + (0.05f * scale * sin(angle))
        );
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY + 0.04f * scale);
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * PI * i / 100;
        glVertex2f(
            centerX + (0.05f * scale * cos(angle)),
            centerY + 0.04f * scale + (0.04f * scale * sin(angle))
        );
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY - 0.04f * scale);
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * PI * i / 100;
        glVertex2f(
            centerX + (0.05f * scale * cos(angle)),
            centerY - 0.04f * scale + (0.04f * scale * sin(angle))
        );
    }
    glEnd();
}

// --------------------- MAN ---------------------
void drawMan() {
    glBegin(GL_POLYGON);
    glColor3ub(255, 128, 128);
    glVertex2f(-0.5195f, -0.2625f);
    glVertex2f(-0.4805f, -0.2625f);
    glVertex2f(-0.4805f, -0.3250f);
    glVertex2f(-0.5195f, -0.3250f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 255, 204);
    glVertex2f(-0.5078f, -0.2625f);
    glVertex2f(-0.4922f, -0.2625f);
    glVertex2f(-0.4922f, -0.2500f);
    glVertex2f(-0.5078f, -0.2500f);
    glEnd();

    int j;
    float x1 = -0.5f;
    float y1 = -0.2250f;
    float radius1 = 0.0391f;
    int triangleAmount1 = 100;
    float twicePi1 = 2.0f * PI;
    glColor3f(1.0f, 0.85f, 0.55f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1, y1);
    for (j = 0; j <= triangleAmount1; j++) {
        glVertex2f(x1 + (radius1 * cos(j * twicePi1 / triangleAmount1)),
                   y1 + (radius1 * sin(j * twicePi1 / triangleAmount1)));
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(153, 153, 255);
    glVertex2f(-0.4805f, -0.3250f);
    glVertex2f(-0.5195f, -0.3250f);
    glVertex2f(-0.5195f, -0.3375f);
    glVertex2f(-0.4805f, -0.3375f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(153, 153, 255);
    glVertex2f(-0.5195f, -0.3375f);
    glVertex2f(-0.5000f, -0.3375f);
    glVertex2f(-0.5078f, -0.3875f);
    glVertex2f(-0.5195f, -0.3875f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(0, 128, 128);
    glVertex2f(-0.5195f, -0.3875f);
    glVertex2f(-0.5078f, -0.3875f);
    glVertex2f(-0.5070f, -0.3938f);
    glVertex2f(-0.5203f, -0.3938f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(153, 153, 255);
    glVertex2f(-0.4805f, -0.3375f);
    glVertex2f(-0.5000f, -0.3375f);
    glVertex2f(-0.4922f, -0.3875f);
    glVertex2f(-0.4805f, -0.3875f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(0, 128, 128);
    glVertex2f(-0.4922f, -0.3875f);
    glVertex2f(-0.4805f, -0.3875f);
    glVertex2f(-0.4797f, -0.3938f);
    glVertex2f(-0.4930f, -0.3938f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 128, 128);
    if (man_hand_raised) {
        glVertex2f(-0.5195f, -0.2625f);
        glVertex2f(-0.5195f, -0.2375f);
        glVertex2f(-0.5227f, -0.2250f);
        glVertex2f(-0.5289f, -0.2250f);
    } else {
        glVertex2f(-0.5195f, -0.2625f);
        glVertex2f(-0.5195f, -0.2875f);
        glVertex2f(-0.5227f, -0.3000f);
        glVertex2f(-0.5289f, -0.3000f);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 255, 204);
    if (man_hand_raised) {
        glVertex2f(-0.5289f, -0.2250f);
        glVertex2f(-0.5227f, -0.2250f);
        glVertex2f(-0.5231f, -0.2187f);
        glVertex2f(-0.5293f, -0.2187f);
    } else {
        glVertex2f(-0.5289f, -0.3000f);
        glVertex2f(-0.5227f, -0.3000f);
        glVertex2f(-0.5231f, -0.3063f);
        glVertex2f(-0.5293f, -0.3063f);
    }
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 128, 128);
    glVertex2f(-0.4805f, -0.2625f);
    glVertex2f(-0.4805f, -0.2875f);
    glVertex2f(-0.4773f, -0.3000f);
    glVertex2f(-0.4711f, -0.3000f);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 255, 204);
    glVertex2f(-0.4773f, -0.3000f);
    glVertex2f(-0.4711f, -0.3000f);
    glVertex2f(-0.4707f, -0.3063f);
    glVertex2f(-0.4769f, -0.3063f);
    glEnd();
}

// --------------------- ANIMATION UPDATE ---------------------
void update(int value) {
    if (currentScene == 1) {
        if (isNight && transitionFactor < 1.0f) {
            transitionFactor += transitionSpeed;
            if (transitionFactor > 1.0f) transitionFactor = 1.0f;
        } else if (!isNight && transitionFactor > 0.0f) {
            transitionFactor -= transitionSpeed;
            if (transitionFactor < 0.0f) transitionFactor = 0.0f;
        }

        if (car_moving && !man_hand_raised) {
            car1_position += car_speed;
            if (car1_position > 1.0f)
                car1_position = -1.0f;

            car2_position -= car_speed;
            if (car2_position < -1.0f)
                car2_position = 1.0f;
        }

        sun_position_x += sun_speed;
        if (sun_position_x > 1.0f)
            sun_position_x = -1.0f;

        if (transitionFactor < 1.0f) {
            for (int i = 0; i < 6; i++) {
                cloud_positions[i] += cloud_speed;
                if (cloud_positions[i] > 1.2f)
                    cloud_positions[i] = -1.2f;
            }
        }

        if (man_moving) {
            if (man_position_y > -0.51f) {
                man_position_y -= 0.005f;
                if (man_position_y < -0.51f)
                    man_position_y = -0.51f;
            } else {
                man_position_x -= man_speed;
                if (man_position_x < -1.0f) {
                    man_position_x = -0.4f;
                    man_position_y = 0.0f;
                    man_moving = 0;
                }
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(50, update, 0);
}

// --------------------- SCENE TRANSITION ---------------------
void switchScene(int value) {
    currentScene = 1;
    glutPostRedisplay();
}

// --------------------- MAIN DISPLAY FUNCTION ---------------------
void display() {
    if (currentScene == 0) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0, 1280.0, 0.0, 800.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        displayScene0();
    } else {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        float dayR = 0.4f, dayG = 0.8f, dayB = 1.0f;
        float nightR = 0.0f, nightG = 0.2f, nightB = 0.4f;
        float r = dayR + (nightR - dayR) * transitionFactor;
        float g = dayG + (nightG - dayG) * transitionFactor;
        float b = dayB + (nightB - dayB) * transitionFactor;

        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (transitionFactor > 0.5f) {
            draw_stars();
        }

        draw_sun_moon();

        if (transitionFactor < 0.5f) {
            draw_cloud(cloud_positions[0], 0.55f, 0.9f);
            draw_cloud(cloud_positions[1], 0.4f, 1.1f);
            draw_cloud(cloud_positions[2], 0.6f, 0.8f);
            draw_cloud(cloud_positions[3], 0.4f, 1.0f);
            draw_cloud(cloud_positions[4], 0.5f, 1.0f);
            draw_cloud(cloud_positions[5], 0.6f, 0.9f);
        }

        glBegin(GL_QUADS);
        glColor3f(0.31f, 0.64f, 0.18f);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(1.0f, -0.2f);
        glVertex2f(-1.0f, -0.2f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.96f, 0.94f, 0.65f);
        glVertex2f(-0.92f, -0.08f);
        glVertex2f(-0.91f, -0.06f);
        glVertex2f(-0.80f, 0.14f);
        glVertex2f(-0.31f, 0.14f);
        glVertex2f(-0.12f, -0.066f);
        glVertex2f(-0.65f, -0.092f);
        glVertex2f(-0.80f, 0.12f);
        glVertex2f(-0.90f, -0.08f);
        glEnd();

        glPushMatrix();
        glTranslatef(-0.7198f, 0.27f, 0.0f);
        glScalef(0.8f, 0.8f, 1.0f);
        draw_tree_with_small_trunk();
        glPopMatrix();

        glBegin(GL_POLYGON);
        glColor3f(0.553f, 0.839f, 0.576f);
        glVertex2f(-0.900f, -0.08f);
        glVertex2f(-0.7925f, 0.112f);
        glVertex2f(-0.776f, 0.0928f);
        glVertex2f(-0.8618f, -0.07812f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.678f, 0.831f, 0.055f);
        glVertex2f(-0.86f, -0.08f);
        glVertex2f(-0.775f, 0.095f);
        glVertex2f(-0.65f, -0.092f);
        glVertex2f(-0.169f, -0.0699f);
        glVertex2f(-0.1682f, -0.176f);
        glVertex2f(-0.6505f, -0.2064f);
        glVertex2f(-0.86f, -0.182f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.8f, 0.36f, 0.23f);
        glVertex2f(-0.86f, -0.1826f);
        glVertex2f(-0.65f, -0.2064f);
        glVertex2f(-0.1682f, -0.176f);
        glVertex2f(-0.168f, -0.244f);
        glVertex2f(-0.6494f, -0.2796f);
        glVertex2f(-0.8605f, -0.2502f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.05f, 0.05f, 0.05f);
        glVertex2f(-0.372f, -0.095f);
        glVertex2f(-0.372f, -0.225f);
        glVertex2f(-0.449f, -0.225f);
        glVertex2f(-0.449f, -0.095f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.05f, 0.05f, 0.05f);
        glVertex2f(-0.57f, -0.102f);
        glVertex2f(-0.51f, -0.1002f);
        glVertex2f(-0.512f, -0.1702f);
        glVertex2f(-0.567f, -0.1702f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.5f, 0.3f, 0.1f);
        glVertex2f(-0.56f, -0.110f);
        glVertex2f(-0.52f, -0.109f);
        glVertex2f(-0.521f, -0.161f);
        glVertex2f(-0.559f, -0.162f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.05f, 0.05f, 0.05f);
        glVertex2f(-0.30f, -0.102f);
        glVertex2f(-0.24f, -0.1002f);
        glVertex2f(-0.2418f, -0.1698f);
        glVertex2f(-0.296f, -0.1702f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.5f, 0.3f, 0.1f);
        glVertex2f(-0.291f, -0.110f);
        glVertex2f(-0.249f, -0.108f);
        glVertex2f(-0.251f, -0.161f);
        glVertex2f(-0.289f, -0.162f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.941f, 0.733f, 0.035f);
        glVertex2f(-0.346f, -0.220f);
        glVertex2f(-0.462f, -0.228f);
        glVertex2f(-0.437f, -0.239f);
        glVertex2f(-0.436f, -0.259f);
        glVertex2f(-0.415f, -0.271f);
        glVertex2f(-0.415f, -0.292f);
        glVertex2f(-0.298f, -0.282f);
        glVertex2f(-0.300f, -0.260f);
        glVertex2f(-0.324f, -0.248f);
        glVertex2f(-0.328f, -0.234f);
        glVertex2f(-0.346f, -0.220f);
        glEnd();

        draw_tree();

        glBegin(GL_QUADS);
        glColor3f(0.82f, 0.78f, 0.72f);
        glVertex2f(0.35f, -0.25f);
        glVertex2f(0.95f, -0.25f);
        glVertex2f(0.95f, -0.05f);
        glVertex2f(0.35f, -0.05f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.18f, 0.33f, 0.2f);
        glVertex2f(0.33f, -0.25f);
        glVertex2f(0.97f, -0.25f);
        glVertex2f(1.00f, -0.35f);
        glVertex2f(0.30f, -0.35f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.89f, 0.48f, 0.17f);
        glVertex2f(0.30f, -0.05f);
        glVertex2f(0.65f, 0.15f);
        glVertex2f(1.00f, -0.05f);
        glVertex2f(0.97f, -0.07f);
        glVertex2f(0.33f, -0.07f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.3f, 0.0f);
        glVertex2f(0.62f, -0.25f);
        glVertex2f(0.68f, -0.25f);
        glVertex2f(0.68f, -0.12f);
        glVertex2f(0.62f, -0.12f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.05f, 0.05f, 0.05f);
        glVertex2f(0.43f, -0.12f);
        glVertex2f(0.51f, -0.12f);
        glVertex2f(0.51f, -0.18f);
        glVertex2f(0.43f, -0.18f);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.05f, 0.05f, 0.05f);
        glVertex2f(0.79f, -0.12f);
        glVertex2f(0.87f, -0.12f);
        glVertex2f(0.87f, -0.18f);
        glVertex2f(0.79f, -0.18f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.6f, 0.6f, 0.6f);
        glVertex2f(0.36f, -0.25f);
        glVertex2f(0.40f, -0.25f);
        glVertex2f(0.41f, -0.22f);
        glVertex2f(0.39f, -0.21f);
        glVertex2f(0.36f, -0.22f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.6f, 0.6f, 0.6f);
        glVertex2f(0.90f, -0.25f);
        glVertex2f(0.94f, -0.25f);
        glVertex2f(0.94f, -0.22f);
        glVertex2f(0.92f, -0.21f);
        glVertex2f(0.90f, -0.22f);
        glEnd();

        glBegin(GL_POLYGON);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(-1.0f, -0.51f);
        glVertex2f(0.991f, -0.517f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(-1.0f, -1.0f);
        glEnd();

        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(-1.0f, -0.51f);
        glVertex2f(0.991f, -0.517f);
        glEnd();

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(-1.025f, -0.76f);
        glVertex2f(-0.975f, -0.76f);
        glVertex2f(-0.975f, -0.74f);
        glVertex2f(-1.025f, -0.74f);
        glEnd();

        glBegin(GL_QUADS);
        glVertex2f(-0.6268f, -0.76f);
        glVertex2f(-0.5768f, -0.76f);
        glVertex2f(-0.5768f, -0.74f);
        glVertex2f(-0.6268f, -0.74f);
        glEnd();

        glBegin(GL_QUADS);
        glVertex2f(-0.2286f, -0.76f);
        glVertex2f(-0.1786f, -0.76f);
        glVertex2f(-0.1786f, -0.74f);
        glVertex2f(-0.2286f, -0.74f);
        glEnd();

        glBegin(GL_QUADS);
        glVertex2f(0.1696f, -0.76f);
        glVertex2f(0.2196f, -0.76f);
        glVertex2f(0.2196f, -0.74f);
        glVertex2f(0.1696f, -0.74f);
        glEnd();

        glBegin(GL_QUADS);
        glVertex2f(0.5678f, -0.76f);
        glVertex2f(0.6178f, -0.76f);
        glVertex2f(0.6178f, -0.74f);
        glVertex2f(0.5678f, -0.74f);
        glEnd();

        glBegin(GL_QUADS);
        glVertex2f(0.966f, -0.76f);
        glVertex2f(1.016f, -0.76f);
        glVertex2f(1.016f, -0.74f);
        glVertex2f(0.966f, -0.74f);
        glEnd();

        glPushMatrix();
        glTranslatef(car1_position, -0.35f, 0.0f);
        glScalef(0.5f, 0.5f, 1.0f);
        draw_car();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(car2_position, -0.45f, 0.0f);
        glScalef(0.5f, 0.5f, 1.0f);
        draw_car_mirrored();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(man_position_x, man_position_y, 0.0f);
        drawMan();
        glPopMatrix();

        glDisable(GL_BLEND);

        glFlush();
        glutSwapBuffers();
    }
}

// --------------------- KEYBOARD HANDLER ---------------------
void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case '0':
            currentScene = 0;
            glutPostRedisplay();
            break;
        case 'n':
        case 'N':
            if (currentScene == 1) {
                isNight = 1;
                glutPostRedisplay();
            }
            break;
        case 'd':
        case 'D':
            if (currentScene == 1) {
                isNight = 0;
                glutPostRedisplay();
            }
            break;
        case 'm':
        case 'M':
            if (currentScene == 1) {
                man_moving = !man_moving;
                if (!man_moving) {
                    man_position_x = -0.4f;
                    man_position_y = 0.0f;
                    man_hand_raised = 0;
                }
                glutPostRedisplay();
            }
            break;
        case 'h':
        case 'H':
            if (currentScene == 1) {
                man_hand_raised = !man_hand_raised;
                glutPostRedisplay();
            }
            break;
        case ' ':
            if (currentScene == 1) {
                car_moving = !car_moving;
                glutPostRedisplay();
            }
            break;
    }
}

// --------------------- SPECIAL KEY HANDLER ---------------------
void handleSpecialKeypress(int key, int x, int y) {
    if (currentScene == 1 && key == GLUT_KEY_RIGHT) {
        car_moving = !car_moving;
        glutPostRedisplay();
    }
}

// --------------------- RESHAPE ---------------------
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

// --------------------- INIT ---------------------
void init(void) {
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
}

// --------------------- MAIN ---------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1280, 800);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("The Village House");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleSpecialKeypress);
    glutTimerFunc(50, update, 0);
    glutTimerFunc(5000, switchScene, 0); // Switch to animated scene after 5 seconds
    glutMainLoop();
    return 0;
}
