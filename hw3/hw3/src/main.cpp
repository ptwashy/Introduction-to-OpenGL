#include "main.h"
#include "my_gl.h"

int main(int argc, char **argv) {

	/* General initialization for GLUT and OpenGL
	Must be called first */
	glutInit(&argc, argv);

	/* we define these setup procedures */
	glut_setup();
	gl_setup();
	my_setup();

	/* go into the main event loop */
	glutMainLoop();

	return(0);
}

/* This function sets up the windowing related glut calls */
void glut_setup(void) {

	/* specify display mode -- here we ask for a double buffer and RGB coloring */
	/* NEW: tell display we care about depth now */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	/* make a 400x400 window with the title of "GLUT Skeleton" placed at the top left corner */
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Cube in Perspective View");

	/*initialize callback functions */
	glutDisplayFunc(my_display);
	glutReshapeFunc(my_reshape);
	glutKeyboardFunc(my_keyboard);
	return;
}

/* This function sets up the initial states of OpenGL related enivornment */
void gl_setup(void) {

	/* specifies a background color: black in this case */
	glClearColor(0, 0, 0, 0);

	/* NEW: now we have to enable depth handling (z-buffer) */
	glEnable(GL_DEPTH_TEST);

	/* NEW: setup for 3d projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// perspective view:
	// fovy = 20 degrees
	// aspect ratio is 1 (width = height
	// near plane of the view volume is at 1
	//  far plane of the view volume is at 100
	//my_perspective(20.0, 1.0, 1.0, 100.0);
	//my_frustum(-3,3,-3,3,-3,3);
	my_ortho(-3,3,-3,3,-3,3);
	/* NEW: safe practice: initialize modelview matrix */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(0.0, 5.0, 15.0,  //only enable for perspective view
		0.0, 0.0, 0.0,  
		0.0, 1.0, 0.0);

	return;
}

void my_setup(void) {
	theta = 0;
	bGrid = FALSE;
	iRotY = 0;
	vertMove = 0;
	speed = 1;
	size = 1;
	scale = 1;
	zero = 0;
	sides = 10;
	ticker = 100;
	shades = 1.0f / sides;
	draw = -1;
	dir = FORWARD;
	generate_cylinder();
	return;
}

void my_reshape(int w, int h) {

	/* define viewport -- x, y, (origin is at lower left corner) width, height */
	glViewport(0, 0, min(w, h), min(w, h));
	return;
}

void my_keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case'u':
	case'U':
		break;
	case'd':
	case'D':
		break;
	case 'a':
		speed++;
		break;
	case 'A':
		speed--;
		break;
	case 's':
		scale = 1.1;
		size += 0.1;
		glutTimerFunc(DELTA_DEG, my_timer, 0);
		break;
	case 'S':
		scale = .9;
		size -= 0.1;
		glutTimerFunc(DELTA_DEG, my_timer, 0);
		break;
	case 'y':
		iRotY = COUNTERCLOCKWISE;
		theta += (speed*iRotY);
		glutTimerFunc(DELTA_DEG, my_timer, 0);
		break;
	case 'Y':
		iRotY = CLOCKWISE;
		theta += (speed*iRotY);
		glutTimerFunc(DELTA_DEG, my_timer, 0);
		break;
	case 'q':
	case 'Q':
		exit(0);
	case'z':
	case'Z':
		zero = 1;
		glutTimerFunc(DELTA_DEG, my_timer, 0);
		break;
	default: break;
	}
	glutPostRedisplay();
	return;
}

/* helper drawing function. Input are 4 indices into the vertices
array to define a face and an index into the color array.  For the
normal to work out, follow left-hand-rule (i.e., counter clock
wise) */
void make_quad(int iv1, int iv2, int iv3, int iv4, int ic) {
	glBegin(GL_POLYGON);
	{
		glColor3fv(colors[ic]);
		glVertex3fv(vertices[iv1]);
		glVertex3fv(vertices[iv2]);
		glVertex3fv(vertices[iv3]);
		glVertex3fv(vertices[iv4]);
	}
	glEnd();
}

/* defines a hollow cube (no top or bottom) */
void make_cube()
{
	make_quad(4, 5, 1, 0, BLUE);
	make_quad(5, 6, 2, 1, CYAN);
	make_quad(3, 2, 6, 7, YELLOW);
	make_quad(0, 3, 7, 4, MAGENTA);
	make_quad(3, 2, 1, 0, RED);
	make_quad(7, 6, 5, 4, WHITE);
}

void my_display(void) {
	/* clear the buffer */
	/* NEW: now we have to clear depth as well */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	if (zero == 0)
	{
		my_rotate((speed*iRotY), 0, 1, 0);
		my_scale(scale, scale, scale);
	}
	else
	{
		my_rotate((-1*theta)%360, 0, 1, 0);
		my_scale(1/size, 1/size, 1/size);
		zero = 0;
		theta = 0;
		size = 1;
	}
	//make_cube();
	glPushMatrix();
	my_translate(0, -1, 0);
	if (ticker == 100)
	{
		if (draw < sides)
		{
			if (dir == FORWARD)
				draw++;
			else if (dir == BACKWARD)
				draw--;
		}
		else if (draw == sides)
		{
			dir = BACKWARD;
			draw--;
		}
		else if (draw <= 0)
		{
			dir = FORWARD;
			draw ++;
		}

		draw_cylinder(dir, draw);
		ticker = 0;
	}
	else
	{
		draw_cylinder(dir, draw);
		ticker += 5;
	}
	glPopMatrix();

	/* buffer is ready */
	glutSwapBuffers();

	return;
}

void my_timer(int val) {
	iRotY = 0;
	scale = 1;

	glutPostRedisplay();
	return;
}


void generate_cylinder( void )
{
	int i;
	float angle = 6.28318530718 / sides;
	for (i = 0; i < sides; i++)
	{
		cylinder[i][0] = cosf(angle*i);
		cylinder[i][1] = 0;
		cylinder[i][2] = sinf(angle*i);
	}
	for (i = sides; i < 2 * sides; i++)
	{
		cylinder[i][0] = cosf(angle*i);
		cylinder[i][1] = 2;
		cylinder[i][2] = sinf(angle*i);
	}
}

void draw_cylinder(int direction, int number)
{
	int i;
	if (direction == FORWARD)
	{
		for (i = 0; i < number; i++)
		{ 
			int j = i - 1;
			if (j == -1)
			{
				j = sides - 1;
			}
			draw_quad(i, j, j + sides, i + sides, i*shades);
			draw_triangle(i, j, i*shades, 0.0f);
			draw_triangle(i + sides, j + sides, i*shades, 2.0f);
		}
	}
	else if (direction == BACKWARD)
	{
		for (i = (sides - 1); i >= (sides-number); i--)
		{
			int j = i - 1;
			if (j == -1)
			{
				j = sides - 1;
			}
			draw_quad(i, j, j + sides, i + sides, i*shades);
			draw_triangle(i, j, i*shades, 0.0f);
			draw_triangle(i + sides, j + sides, i*shades, 2.0f);
		}
	}
	return;
}
void draw_quad(int v1, int v2, int v3, int v4, GLfloat w)
{
	GLfloat shade[3] = { w, w, 1.0 };
	glBegin(GL_POLYGON);
	{
		glColor3fv(shade);
		glVertex3fv(cylinder[v1]);
		glVertex3fv(cylinder[v2]);
		glVertex3fv(cylinder[v3]);
		glVertex3fv(cylinder[v4]);
	}
	glEnd();
	return;
}
void draw_triangle(int v1, int v2, GLfloat w, GLfloat height)
{
	GLfloat shade[3] = { w, w, 1.0 };
	GLfloat center[3] = { 0, height, 0 };
	glBegin(GL_POLYGON);
	{
		glColor3fv(shade);
		glVertex3fv(cylinder[v1]);
		glVertex3fv(cylinder[v2]);
		glVertex3fv(center);
	}
	glEnd();
	return;
	return;
}