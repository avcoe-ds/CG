#include <iostream>
#include <GL/glut.h>
using namespace std;

int xc, yc, radius;

void plotPoint(int x, int y)
{
    glBegin(GL_POINTS);
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc - y, yc + x);
        glVertex2i(xc + y, yc - x);
        glVertex2i(xc - y, yc - x);
    glEnd();
}

void midpointCircle()
{
    int x = 0;
    int y = radius;
    int decision = 1 - radius;

    while (x <= y)
    {
        plotPoint(x, y);

        if (decision < 0)
        {
            decision += 2 * x + 3;
        }
        else
        {
            decision += 2 * (x - y) + 5;
            y--;
        }

        x++;
    }
}

void initialize()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glPointSize(2.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    midpointCircle();
    glFlush();
}

int main(int argc, char** argv)
{
    cout << "MIDPOINT CIRCLE DRAWING ALGORITHM\n";
    cout << "Enter the X-coordinate of the center: ";
    cin >> xc;

    cout << "Enter the Y-coordinate of the center: ";
    cin >> yc;

    cout << "Enter the radius: ";
    cin >> radius;

    if (radius <= 0)
    {
        cout << "Error: Radius must be a positive integer." << endl;
        return 1;
    }

    if (xc - radius < 0 || xc + radius > 640 ||
        yc - radius < 0 || yc + radius > 480)
    {
        cout << "Warning: The circle may be partially outside the window."
             << endl;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Midpoint Circle Drawing Algorithm");

    initialize();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
