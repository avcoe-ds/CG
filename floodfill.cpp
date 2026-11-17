#include <iostream>
#include <cstdlib>
#include <GL/glut.h>

using namespace std;

const int WIDTH = 640;
const int HEIGHT = 480;

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // White background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Draw a closed polygon boundary.
void polygon()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(3.0f);

    glBegin(GL_LINE_LOOP);
        glVertex2i(150, 100);
        glVertex2i(300, 300);
        glVertex2i(450, 100);
    glEnd();

    glFlush();
}

// Iterative 4-connected flood fill.
// It avoids recursive stack overflow and compares pixel colors with a tolerance.
void floodFill(int startX, int startY,
               const float fillColor[3],
               const float boundaryColor[3])
{
    if (startX < 0 || startX >= WIDTH || startY < 0 || startY >= HEIGHT)
        return;

    struct Point
    {
        int x;
        int y;
    };

    // Read the color at the clicked pixel.
    float clickedColor[3];
    glReadPixels(startX, startY, 1, 1, GL_RGB, GL_FLOAT, clickedColor);

    const float tolerance = 0.01f;

    // Do not fill if the click is on the boundary or already filled.
    bool isBoundary =
        fabs(clickedColor[0] - boundaryColor[0]) < tolerance &&
        fabs(clickedColor[1] - boundaryColor[1]) < tolerance &&
        fabs(clickedColor[2] - boundaryColor[2]) < tolerance;

    bool isAlreadyFill =
        fabs(clickedColor[0] - fillColor[0]) < tolerance &&
        fabs(clickedColor[1] - fillColor[1]) < tolerance &&
        fabs(clickedColor[2] - fillColor[2]) < tolerance;

    if (isBoundary || isAlreadyFill)
        return;

    // A simple array-based stack for pending pixels.
    Point stack[WIDTH * HEIGHT];
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

        bool matchesBoundary =
            fabs(currentColor[0] - boundaryColor[0]) < tolerance &&
            fabs(currentColor[1] - boundaryColor[1]) < tolerance &&
            fabs(currentColor[2] - boundaryColor[2]) < tolerance;

        bool matchesFill =
            fabs(currentColor[0] - fillColor[0]) < tolerance &&
            fabs(currentColor[1] - fillColor[1]) < tolerance &&
            fabs(currentColor[2] - fillColor[2]) < tolerance;

        if (matchesBoundary || matchesFill)
            continue;

        glVertex2i(p.x, p.y);

        // Push the four neighboring pixels (4-connected fill).
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
        // OpenGL's origin is bottom-left; mouse coordinates are top-left.
        int openGLY = HEIGHT - 1 - y;

        const float boundaryColor[3] = {1.0f, 0.0f, 0.0f};
        const float fillColor[3] = {0.0f, 0.0f, 1.0f};

        floodFill(x, openGLY, fillColor, boundaryColor);
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Polygon Filling - Flood Fill");

    init();
    glutDisplayFunc(polygon);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}
