#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

int x1, y1, x2, y2;

// Draw a line using the all-octant Bresenham algorithm.
// style: 0 = simple, 1 = dashed, 2 = dotted
void drawBresenhamLine(int xa, int ya, int xb, int yb, int style)
{
    int dx = abs(xb - xa);
    int dy = abs(yb - ya);

    int sx = (xa < xb) ? 1 : -1;
    int sy = (ya < yb) ? 1 : -1;

    int err = dx - dy;
    int step = 0;

    glBegin(GL_POINTS);

    while (true)
    {
        bool drawPoint = true;

        if (style == 1)          // Dashed line
            drawPoint = ((step / 8) % 2 == 0);
        else if (style == 2)     // Dotted line
            drawPoint = (step % 4 == 0);

        if (drawPoint)
            glVertex2i(xa, ya);

        if (xa == xb && ya == yb)
            break;

        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            xa += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            ya += sy;
        }

        step++;
    }

    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the three styles in separate horizontal bands for comparison.
    glColor3f(0.0f, 1.0f, 0.0f);
    drawBresenhamLine(x1, y1 + 30, x2, y2 + 30, 0);  // Simple

    glColor3f(1.0f, 1.0f, 0.0f);
    drawBresenhamLine(x1, y1, x2, y2, 1);            // Dashed

    glColor3f(0.0f, 0.7f, 1.0f);
    drawBresenhamLine(x1, y1 - 30, x2, y2 - 30, 2);  // Dotted

    glFlush();
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-250.0, 250.0, -250.0, 250.0);

    glPointSize(2.0f);
}

int main(int argc, char** argv)
{
    cout << "Bresenham's Line Drawing Algorithm\n";
    cout << "Enter the starting point (x1 y1): ";
    cin >> x1 >> y1;

    cout << "Enter the ending point (x2 y2): ";
    cin >> x2 >> y2;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Line Drawing Algorithm");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
