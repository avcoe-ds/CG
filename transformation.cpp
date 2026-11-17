#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <GL/glut.h>

using namespace std;

const double PI = 3.14159265358979323846;

int choice;
int edges;
vector<int> pntX;
vector<int> pntY;

int transX, transY;
double scaleX, scaleY;
double angleRad;

void drawOriginalPolygon()
{
    glColor3f(1.0f, 0.0f, 0.0f); // Red: original polygon
    glBegin(GL_POLYGON);
    for (int i = 0; i < edges; i++)
        glVertex2i(pntX[i], pntY[i]);
    glEnd();
}

void drawTranslatedPolygon()
{
    glColor3f(0.0f, 0.6f, 0.0f); // Green: translated polygon
    glBegin(GL_POLYGON);
    for (int i = 0; i < edges; i++)
        glVertex2i(pntX[i] + transX, pntY[i] + transY);
    glEnd();
}

void drawScaledPolygon()
{
    glColor3f(0.0f, 0.0f, 1.0f); // Blue: scaled polygon
    glBegin(GL_POLYGON);
    for (int i = 0; i < edges; i++)
        glVertex2d(pntX[i] * scaleX, pntY[i] * scaleY);
    glEnd();
}

void drawRotatedPolygon()
{
    glColor3f(0.0f, 0.0f, 1.0f); // Blue: rotated polygon
    glBegin(GL_POLYGON);

    for (int i = 0; i < edges; i++)
    {
        double x = pntX[i];
        double y = pntY[i];

        // Rotation about the origin
        double newX = x * cos(angleRad) - y * sin(angleRad);
        double newY = x * sin(angleRad) + y * cos(angleRad);

        glVertex2d(newX, newY);
    }

    glEnd();
}

void initialize()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawOriginalPolygon();

    if (choice == 1)
        drawTranslatedPolygon();
    else if (choice == 2)
        drawScaledPolygon();
    else if (choice == 3)
        drawRotatedPolygon();

    glFlush();
}

int main(int argc, char **argv)
{
    cout << "BASIC 2D TRANSFORMATIONS\n";
    cout << "1. Translation\n";
    cout << "2. Scaling\n";
    cout << "3. Rotation\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (!cin || choice < 1 || choice > 4)
    {
        cout << "Invalid choice." << endl;
        return 1;
    }

    if (choice == 4)
        return 0;

    cout << "Enter number of polygon vertices (minimum 3): ";
    cin >> edges;

    if (!cin || edges < 3 || edges > 100)
    {
        cout << "Invalid number of vertices. Enter a value from 3 to 100." << endl;
        return 1;
    }

    for (int i = 0; i < edges; i++)
    {
        int x, y;
        cout << "Enter X and Y coordinates for vertex " << i + 1 << ": ";
        cin >> x >> y;

        if (!cin)
        {
            cout << "Invalid coordinate input." << endl;
            return 1;
        }

        pntX.push_back(x);
        pntY.push_back(y);
    }

    if (choice == 1)
    {
        cout << "Enter translation factors Tx and Ty: ";
        cin >> transX >> transY;
    }
    else if (choice == 2)
    {
        cout << "Enter scaling factors Sx and Sy: ";
        cin >> scaleX >> scaleY;
    }
    else if (choice == 3)
    {
        double angleDegrees;
        cout << "Enter angle of rotation in degrees: ";
        cin >> angleDegrees;
        angleRad = angleDegrees * PI / 180.0;
    }

    if (!cin)
    {
        cout << "Invalid transformation input." << endl;
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Basic 2D Transformations");

    initialize();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
