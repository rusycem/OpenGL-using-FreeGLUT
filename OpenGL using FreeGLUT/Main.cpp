/* recursive subdivision of a tetrahedron to form 3D Sierpinski gasket */
/* number of recursive steps given on command line */
#include <stdlib.h>
#include <GL/glut.h>

/* Initial tetrahedron */
GLfloat v[4][3] = {
    { 0.0, 0.0, 1.0 },
    { 0.0, 0.942809, -0.33333 },
    { -0.816497, -0.471405, -0.333333 },
    { 0.816497, -0.471405, -0.333333 }
};

/* Colors for each tetrahedron */
GLfloat colors[5][3] = {
    { 1.0, 0.0, 0.0 },   // Red
    { 0.0, 1.0, 0.0 },   // Green
    { 0.0, 0.0, 1.0 },   // Blue
    { 1.0, 1.0, 0.0 },   // Yellow
    { 1.0, 0.0, 1.0 }    // Purple
};

GLfloat angle = 0.0;             // Angle of rotation
GLfloat rotationSpeed = 0.0f;    // Rotation speed
GLfloat scale = 0.5f;            // Scale of the object
int n;
int o_values[5] = { 2, 3, 4, 5, 6 }; // Different number of iterations

void triangle(GLfloat* va, GLfloat* vb, GLfloat* vc)
{
    glVertex3fv(va);
    glVertex3fv(vb);
    glVertex3fv(vc);
}

void tetra(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d)
{
    glColor3fv(colors[0]);
    triangle(a, b, c);
    glColor3fv(colors[1]);
    triangle(a, c, d);
    glColor3fv(colors[2]);
    triangle(a, d, b);
    glColor3fv(colors[3]);
    triangle(b, d, c);
}

void divide_tetra(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d, int m, int o, int color)
{
    GLfloat mid[6][3];
    int j;
    if (m > 0)
    {
        /* Compute six midpoints */
        for (j = 0; j < 3; j++) mid[0][j] = (a[j] + b[j]) / 2;
        for (j = 0; j < 3; j++) mid[1][j] = (a[j] + c[j]) / 2;
        for (j = 0; j < 3; j++) mid[2][j] = (a[j] + d[j]) / 2;
        for (j = 0; j < 3; j++) mid[3][j] = (b[j] + c[j]) / 2;
        for (j = 0; j < 3; j++) mid[4][j] = (c[j] + d[j]) / 2;
        for (j = 0; j < 3; j++) mid[5][j] = (b[j] + d[j]) / 2;

        /* Create 4 tetrahedrons by subdivision */
        divide_tetra(a, mid[0], mid[1], mid[2], m - 1, o, (color + 1) % 5);
        divide_tetra(mid[0], b, mid[3], mid[5], m - 1, o, (color + 1) % 5);
        divide_tetra(mid[1], mid[3], c, mid[4], m - 1, o, (color + 1) % 5);
        divide_tetra(mid[2], mid[4], d, mid[5], m - 1, o, (color + 1) % 5);
    }
    else {
        glColor3fv(colors[color]);
        tetra(a, b, c, d); /* Draw tetrahedron at end of recursion */
    }
}

void drawTetrahedron(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d, int o)
{
    glBegin(GL_TRIANGLES);
    divide_tetra(a, b, c, d, n, o, 0);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Apply rotation transformation
    for (int i = 1; i < 5; i++) {
        int current_o = o_values[i]; // Get the current 'n' value
        glPushMatrix(); // Save the current matrix

        // Apply scaling and translation transformations
        glScalef(scale, scale, scale); // Scale down the tetrahedron
        if (i == 1) {
            glTranslatef(0.0, 2.0, 0.0); // Translate along the x-axis
            glRotatef(angle * i, 1.0, 2.0, 4.0); // Rotate around the y-axis
        }
        else if (i == 2) {
            glTranslatef(2.0, 0.0, 0.0); // Translate along the x-axis
            glRotatef(angle * i, 2.0, 1.0, 3.0); // Rotate around the y-axis
        }
        else if (i == 3) {
            glTranslatef(-2.0, 0.0, 0.0); // Translate along the x and y axes
            glRotatef(angle * i, 3.0, 2.0, 1.0); // Rotate around the y-axis
        }
        else if (i == 4) {
            glTranslatef(0.0, -2.0, 0.0); // Translate along the x and y axes
            glRotatef(angle * i, 1.0, 3.0, 2.0); // Rotate around the y-axis
        }

        // Draw the tetrahedron with the current 'n' value
        drawTetrahedron(v[0], v[1], v[2], v[3], current_o);
        glPopMatrix(); // Restore the previous matrix
    }

    glFlush();
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
            2.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
    else
        glOrtho(-2.0 * (GLfloat)w / (GLfloat)h,
            2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void idle()
{
    angle += rotationSpeed;
    if (angle >= 360.0)
        angle -= 360.0;
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    n = 2; /* or enter the number of subdivision steps here */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Rotating Sierpinski Gasket");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle); // Register the idle function
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutMainLoop();
}
