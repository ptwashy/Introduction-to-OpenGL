#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void my_translate(GLfloat, GLfloat, GLfloat);
void my_scale(GLfloat, GLfloat, GLfloat);
void my_rotate(GLfloat, GLfloat, GLfloat, GLfloat);
void my_lookat(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble); 
void my_ortho(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);
void my_frustum(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);
void my_perspective(GLfloat fov, GLfloat a, GLfloat n, GLfloat f);