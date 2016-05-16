#include "my_gl.h"

#define min(a,b) ((a) < (b)? a:b)
#define FALSE 0 
#define TRUE  1

#define COUNTERCLOCKWISE  1
#define CLOCKWISE		 -1
#define FORWARD			  1
#define BACKWARD		 -1
#define UP				1.1
#define DOWN		   -1.1

#define DELTA_TIME 100
#define DELTA_DEG  5

/* define index constants into the colors array */
/* colors */
#define BLACK   0
#define RED     1
#define YELLOW  2
#define ORANGE  3
#define GREEN   4
#define BLUE    5
#define WHITE   6
#define MAGENTA 7
#define CYAN    8
#define GREY    9

GLfloat colors[][3] = {
		{ 0.0, 0.0, 0.0 },  /* black   */
		{ 1.0, 0.0, 0.0 },  /* red     */
		{ 1.0, 1.0, 0.0 },  /* yellow  */
		{ 1.0, 0.5, 0.0 },  /* orange  */
		{ 0.0, 1.0, 0.0 },  /* green   */
		{ 0.0, 0.0, 1.0 },  /* blue    */
		{ 1.0, 1.0, 1.0 },   /* white   */
		{ 1.0, 0.0, 1.0 },  /* magenta */
		{ 0.0, 1.0, 1.0 },  /* cyan    */
		{ 0.5, 0.5, 0.5 }  /* 50%grey */
};

GLfloat vertices[][3] = {
		{ -1, 1, 1 }, { 1, 1, 1 }, { 1, 1, -1 }, { -1, 1, -1 },
		{ -1, -1, 1 }, { 1, -1, 1 }, { 1, -1, -1 }, { -1, -1, -1 }
};

GLfloat cylinder[][3] = {0};

void glut_setup(void);
void gl_setup(void);
void my_setup(void);
void my_display(void);
void my_reshape(int w, int h);
void my_keyboard(unsigned char key, int x, int y);
void my_timer(int val);

void generate_cylinder(void);
void draw_cylinder(int, int);
void draw_quad(int, int, int, int, GLfloat);
void draw_triangle(int, int, GLfloat, GLfloat);

int theta;
int bGrid;
int iRotY;
int vertMove;
int speed;
int zero;
int ticker;
int draw;
int dir;
float size;
float scale;
float sides;
float shades;