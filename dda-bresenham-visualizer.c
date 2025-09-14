#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

// Global variables for points
int x1_user, y1_user, x2_user, y2_user;
int xmax_val, ymax_val; // scaling factors

// Put a pixel with color
void putPixel(int x, int y, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_POINTS);
        glVertex2i(x, y);
    glEnd();
}

// Write text on screen
void drawText(const char *text, int x, int y) {
    glColor3f(0.0, 0.0, 0.0); // Black text
    glRasterPos2i(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

// DDA Algorithm
void drawLineDDA(int x1, int y1, int x2, int y2, int offsetX) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float Xinc = dx / (float) steps;
    float Yinc = dy / (float) steps;

    float x = x1;
    float y = y1;
    for (int i = 0; i <= steps; i++) {
        putPixel(round(x) + offsetX, round(y), 1.0, 0.0, 0.0); // Red
        x += Xinc;
        y += Yinc;
    }
}

// Bresenham’s Algorithm
void drawLineBresenham(int x1, int y1, int x2, int y2, int offsetX) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        putPixel(x1 + offsetX, y1, 0.0, 1.0, 0.0); // Green
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx)  { err += dx; y1 += sy; }
    }
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Labels
    drawText("DDA", xmax_val / 2, ymax_val + 5);
    drawText("Bresenham", xmax_val + 20 + xmax_val / 2, ymax_val + 5);

    // Left side: DDA
    drawLineDDA(x1_user, y1_user, x2_user, y2_user, 0);

    // Right side: Bresenham
    drawLineBresenham(x1_user, y1_user, x2_user, y2_user, xmax_val + 20);

    glFlush();
}

// Initialization with auto-scaling
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // Find max values among points
    xmax_val = (x1_user > x2_user ? x1_user : x2_user);
    ymax_val = (y1_user > y2_user ? y1_user : y2_user);

    if (xmax_val < 10) xmax_val = 10;
    if (ymax_val < 10) ymax_val = 10;

    gluOrtho2D(0, 2 * xmax_val + 50, 0, ymax_val + 30);
}

int main(int argc, char** argv) {
    printf("Enter x1 y1: ");
    scanf("%d %d", &x1_user, &y1_user);
    printf("Enter x2 y2: ");
    scanf("%d %d", &x2_user, &y2_user);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("DDA (Left) vs Bresenham (Right)");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
