#include <GL/glut.h>
#include <cstdlib>

float carX = -2.0f;
float speed = 0.01f;

void initialize(int width, int height)
{
    if (height == 0)
        height = 1;

    glClearColor(0.85f, 0.92f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawWheel(float x, float y)
{
    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glColor3f(0.08f, 0.08f, 0.08f);
        glutSolidSphere(0.22, 24, 24);

        // Small hub in the center of the wheel
        glTranslatef(0.0f, 0.0f, 0.19f);
        glColor3f(0.75f, 0.75f, 0.75f);
        glutSolidSphere(0.08, 16, 16);
    glPopMatrix();
}

void drawCar()
{
    glPushMatrix();
        glTranslatef(carX, 0.0f, -6.0f);

        // Main body
        glColor3f(0.85f, 0.12f, 0.08f);
        glBegin(GL_POLYGON);
            glVertex3f(-1.0f, -0.10f, 0.0f);
            glVertex3f(-0.85f, 0.35f, 0.0f);
            glVertex3f(-0.45f, 0.75f, 0.0f);
            glVertex3f(0.35f, 0.75f, 0.0f);
            glVertex3f(0.85f, 0.30f, 0.0f);
            glVertex3f(1.0f, -0.10f, 0.0f);
        glEnd();

        // Lower chassis
        glColor3f(0.65f, 0.05f, 0.04f);
        glBegin(GL_QUADS);
            glVertex3f(-1.0f, -0.10f, 0.01f);
            glVertex3f(1.0f, -0.10f, 0.01f);
            glVertex3f(0.85f, -0.35f, 0.01f);
            glVertex3f(-0.85f, -0.35f, 0.01f);
        glEnd();

        // Windows
        glColor3f(0.55f, 0.85f, 1.0f);
        glBegin(GL_QUADS);
            glVertex3f(-0.38f, 0.68f, 0.02f);
            glVertex3f(0.28f, 0.68f, 0.02f);
            glVertex3f(0.68f, 0.32f, 0.02f);
            glVertex3f(-0.68f, 0.32f, 0.02f);
        glEnd();

        // Wheels
        drawWheel(-0.62f, -0.34f);
        drawWheel(0.62f, -0.34f);
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawCar();

    glutSwapBuffers();
}

void update(int value)
{
    carX += speed;

    // Restart the car after it moves beyond the right edge
    if (carX > 2.5f)
        carX = -2.5f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // approximately 60 frames per second
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (height == 0)
        height = 1;
    gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Moving Car Animation");

    initialize(640, 480);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
