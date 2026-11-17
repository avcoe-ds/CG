#include <iostream>
#include <GL/glut.h>
using namespace std;

// Clipping window
const float XMIN = 150.0f;
const float YMIN = 150.0f;
const float XMAX = 350.0f;
const float YMAX = 350.0f;

// Original line coordinates
float x1, y1, x2, y2;

// Clipped line coordinates
float cx1, cy1, cx2, cy2;

bool processed = false;
bool accepted = false;

// Region codes
const int INSIDE = 0;
const int LEFT   = 1;
const int RIGHT  = 2;
const int BOTTOM = 4;
const int TOP    = 8;

int computeCode(float x, float y)
{
    int code = INSIDE;

    if (x < XMIN)
        code |= LEFT;
    else if (x > XMAX)
        code |= RIGHT;

    if (y < YMIN)
        code |= BOTTOM;
    else if (y > YMAX)
        code |= TOP;

    return code;
}

bool cohenSutherlandClip(float &ax, float &ay, float &bx, float &by)
{
    int code1 = computeCode(ax, ay);
    int code2 = computeCode(bx, by);

    while (true)
    {
        // Both endpoints are inside
        if (code1 == INSIDE && code2 == INSIDE)
            return true;

        // Both endpoints are outside in a common region
        if (code1 & code2)
            return false;

        int codeOut = (code1 != INSIDE) ? code1 : code2;
        float x = 0.0f;
        float y = 0.0f;

        if (codeOut & TOP)
        {
            if (by == ay) return false;
            x = ax + (bx - ax) * (YMAX - ay) / (by - ay);
            y = YMAX;
        }
        else if (codeOut & BOTTOM)
        {
            if (by == ay) return false;
            x = ax + (bx - ax) * (YMIN - ay) / (by - ay);
            y = YMIN;
        }
        else if (codeOut & RIGHT)
        {
            if (bx == ax) return false;
            y = ay + (by - ay) * (XMAX - ax) / (bx - ax);
            x = XMAX;
        }
        else if (codeOut & LEFT)
        {
            if (bx == ax) return false;
            y = ay + (by - ay) * (XMIN - ax) / (bx - ax);
            x = XMIN;
        }

        if (codeOut == code1)
        {
            ax = x;
            ay = y;
            code1 = computeCode(ax, ay);
        }
        else
        {
            bx = x;
            by = y;
            code2 = computeCode(bx, by);
        }
    }
}

void drawText(float x, float y, const char *text)
{
    glRasterPos2f(x, y);
    for (const char *p = text; *p != '\0'; ++p)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *p);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw clipping rectangle
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(XMIN, YMIN);
        glVertex2f(XMAX, YMIN);
        glVertex2f(XMAX, YMAX);
        glVertex2f(XMIN, YMAX);
    glEnd();

    if (!processed)
    {
        // Original line before clipping
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(10.0f, 475.0f, "Click left mouse button to clip the line");
    }
    else if (accepted)
    {
        // Accepted clipped portion
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINES);
            glVertex2f(cx1, cy1);
            glVertex2f(cx2, cy2);
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(10.0f, 475.0f, "Line accepted: clipped portion shown in green");
    }
    else
    {
        // Rejected line
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(10.0f, 475.0f, "Line rejected: outside clipping window");
    }

    glFlush();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !processed)
    {
        // Clip copies so original coordinates remain unchanged
        cx1 = x1;
        cy1 = y1;
        cx2 = x2;
        cy2 = y2;

        accepted = cohenSutherlandClip(cx1, cy1, cx2, cy2);
        processed = true;
        glutPostRedisplay();
    }
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    cout << "COHEN-SUTHERLAND LINE CLIPPING ALGORITHM\n";
    cout << "Clipping window: (150,150) to (350,350)\n";
    cout << "Enter line coordinates (x1 y1 x2 y2): ";
    cin >> x1 >> y1 >> x2 >> y2;

    if (!cin)
    {
        cout << "Invalid input. Please enter four numeric coordinates.\n";
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cohen-Sutherland Line Clipping");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();

    return 0;
}
