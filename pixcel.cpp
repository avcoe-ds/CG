#include <iostream>
#include <GL/glut.h>

using namespace std;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 0.0f, 0.0f);   // Red color
    glPointSize(10.0f);             // Point size

    glBegin(GL_POINTS);
        glVertex2f(10.0f, 10.0f);
        glVertex2f(150.0f, 80.0f);
        glVertex2f(100.0f, 20.0f);
        glVertex2f(200.0f, 100.0f);
    glEnd();

    glFlush();
}

void myinit()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // White background

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 499.0, 0.0, 499.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Points Drawing");

    myinit();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
