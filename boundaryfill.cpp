#include <iostream>
#include <cmath>
#include <GL/glut.h>

using namespace std;

const int WIDTH = 640;
const int HEIGHT = 480;

// Compare two RGB colors with a tolerance.
bool sameColor(const float a[3], const float b[3], float tolerance = 0.01f)
{
    return fabs(a[0] - b[0]) < tolerance &&
           fabs(a[1] - b[1]) < tolerance &&
           fabs(a[2] - b[2]) < tolerance;
}

// Draw the closed polygon boundary.
void polygon()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 0.0f, 0.0f);  // Red boundary
    glLineWidth(3.0f);

    glBegin(GL_LINE_LOOP);
        glVertex2i(150, 100);
        glVertex2i(300, 300);
        glVertex2i(450, 100);
    glEnd();

    glFlush();
}

// Iterative 4-connected boundary fill.
// The fill stops when it reaches the red boundary or already-filled blue pixels.
void boundaryFill(int startX, int startY,
                  const float fillColor[3],
                  const float boundaryColor[3])
{
    if (startX < 0 || startX >= WIDTH || startY < 0 || startY >= HEIGHT)
        return;

    float startColor[3];
    glReadPixels(startX, startY, 1, 1, GL_RGB, GL_FLOAT, startColor);

    // Do not start if the click is on the boundary or in an already-filled area.
    if (sameColor(startColor, boundaryColor) || sameColor(startColor, fillColor))
        return;

    struct Point
    {
        int x;
        int y;
    };

    // Allocate enough space for every pixel in the window.
    static Point stack[WIDTH * HEIGHT];
    int top = -1;

    stack[++top] = {startX, startY};

    glColor3fv(fillColor);
    glPointSize(1.0f);
    glBegin(GL_POINTS);

    while (top >= 0)
    {
        Point p = stack[top--];

        if (p.x < 0 || p.x >= WIDTH || p.y < 0 || p.y >= HEIGHT)
            continue;

        float currentColor[3];
        glReadPixels(p.x, p.y, 1, 1, GL_RGB, GL_FLOAT, currentColor);

        if (sameColor(currentColor, boundaryColor) ||
            sameColor(currentColor, fillColor))
            continue;

        // Color this pixel. Subsequent reads will recognize it as filled.
        glVertex2i(p.x, p.y);

        // Four connected neighbors.
        if (top + 4 < WIDTH * HEIGHT - 1)
        {
            stack[++top] = {p.x + 1, p.y};
            stack[++top] = {p.x - 1, p.y};
            stack[++top] = {p.x, p.y + 1};
            stack[++top] = {p.x, p.y - 1};
        }
    }

    glEnd();
    glFlush();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // Convert GLUT mouse coordinates (top-left origin) to OpenGL coordinates.
        int openGLY = HEIGHT - 1 - y;

        const float boundaryColor[3] = {1.0f, 0.0f, 0.0f};
        const float fillColor[3] = {0.0f, 0.0f, 1.0f};

        boundaryFill(x, openGLY, fillColor, boundaryColor);
    }
}

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Polygon Filling - Boundary Fill");

    init();
    glutDisplayFunc(polygon);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}

