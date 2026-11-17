
#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

int x1, y1, x2, y2;

// Draw a line using the DDA algorithm.
// style: 0 = simple, 1 = dashed, 2 = dotted
void drawDDALine(int xa, int ya, int xb, int yb, int style)
{
    int dx = xb - xa;
    int dy = yb - ya;

    int steps = max(abs(dx), abs(dy));

    // A zero-length line contains only one point.
    if (steps == 0)
    {
        glBegin(GL_POINTS);
        glVertex2i(xa, ya);
        glEnd();
        return;
    }

    float xIncrement = static_cast<float>(dx) / steps;
    float yIncrement = static_cast<float>(dy) / steps;

    float x = static_cast<float>(xa);
    float y = static_cast<float>(ya);

    glBegin(GL_POINTS);

    for (int i = 0; i <= steps; i++)
    {
        bool drawPoint = true;

        if (style == 1)          // Dashed line
            drawPoint = ((i / 8) % 2 == 0);
        else if (style == 2)     // Dotted line
            drawPoint = (i % 4 == 0);

        if (drawPoint)
            glVertex2i(static_cast<int>(round(x)), static_cast<int>(round(y)));

        x += xIncrement;
        y += yIncrement;
    }

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Three styles are shown in separate horizontal bands.
    glColor3f(0.0f, 1.0f, 0.0f);
    drawDDALine(x1, y1 + 30, x2, y2 + 30, 0);  // Simple line

    glColor3f(1.0f, 1.0f, 0.0f);
    drawDDALine(x1, y1, x2, y2, 1);            // Dashed line

    glColor3f(0.0f, 0.7f, 1.0f);
    drawDDALine(x1, y1 - 30, x2, y2 - 30, 2);  // Dotted line

    glFlush();
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
    glPointSize(2.0f);
}

int main(int argc, char** argv)
{
    cout << "DDA Line Drawing Algorithm\n";
    cout << "Enter starting point (x1 y1): ";
    cin >> x1 >> y1;

    cout << "Enter ending point (x2 y2): ";
    cin >> x2 >> y2;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("DDA Line Drawing Algorithm");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
