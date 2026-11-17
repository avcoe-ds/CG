#include <iostream>
#include <cmath>
#include <GL/glut.h>

using namespace std;

const double SQRT3_BY_2 = 0.8660254037844386;
int level;

// Draw one Koch curve segment from (x1, y1) to (x2, y2)
void koch(double x1, double y1, double x2, double y2, int depth)
{
    // Points dividing the line into three equal parts
    double dx = (x2 - x1) / 3.0;
    double dy = (y2 - y1) / 3.0;

    double xA = x1 + dx;
    double yA = y1 + dy;

    double xC = x1 + 2.0 * dx;
    double yC = y1 + 2.0 * dy;

    // Tip of the equilateral triangle
    double vx = xC - xA;
    double vy = yC - yA;

    double xB = xA + 0.5 * vx + SQRT3_BY_2 * vy;
    double yB = yA - SQRT3_BY_2 * vx + 0.5 * vy;

    if (depth == 0)
    {
        glBegin(GL_LINES);
            glVertex2d(x1, y1);
            glVertex2d(xA, yA);

            glVertex2d(xA, yA);
            glVertex2d(xB, yB);

            glVertex2d(xB, yB);
            glVertex2d(xC, yC);

            glVertex2d(xC, yC);
            glVertex2d(x2, y2);
        glEnd();
        return;
    }

    // Recursively generate the four smaller segments
    koch(x1, y1, xA, yA, depth - 1);
    koch(xA, yA, xB, yB, depth - 1);
    koch(xB, yB, xC, yC, depth - 1);
    koch(xC, yC, x2, y2, depth - 1);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.5f);

    // Initial horizontal line segment
    koch(40.0, 300.0, 590.0, 300.0, level);

    glFlush();
}

void initialize()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Keep the same downward-increasing Y coordinate orientation
    gluOrtho2D(0.0, 650.0, 650.0, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    cout << "KOCH CURVE GENERATION\n";
    cout << "Enter the level of curve generation (0 to 7): ";
    cin >> level;

    if (!cin || level < 0 || level > 7)
    {
        cout << "Invalid level. Please enter an integer from 0 to 7." << endl;
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(650, 650);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Koch Curve");

    initialize();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
