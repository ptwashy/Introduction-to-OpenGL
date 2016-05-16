#include "my_gl.h"

void my_translate(GLfloat x, GLfloat y, GLfloat z)
{
	/*standard translation matrix, in column-major for matrix mult*/
	GLfloat a[16] = { 1, 0, 0, 0,
					  0, 1, 0, 0,
					  0, 0, 1, 0,
					  x, y, z, 1 };
	glMultMatrixf(a);
	return;
}

void my_scale(GLfloat x, GLfloat y, GLfloat z)
{
	/*standard scaling matrix*/
	GLfloat a[16] = { x, 0, 0, 0,
					  0, y, 0, 0,
					  0, 0, z, 0,
					  0, 0, 0, 1 };
	glMultMatrixf(a);
	return;
}

void my_rotate(GLfloat angle, GLfloat oldX, GLfloat oldY, GLfloat oldZ)
{
	GLfloat x,y,z,mag,c,s;
	
	/*normalize the rotation normal*/
	mag = sqrtf((oldX * oldX) + (oldY * oldY) + (oldZ * oldZ));
	x = oldX / mag;
	y = oldY / mag;
	z = oldZ / mag;
	
	/*get the sin and cos of the angle*/
	c = cosf(angle * 3.14159265359 / 180);
	s = sinf(angle * 3.14159265359 / 180);


	/*a rotation matrix that handles all cases of rotational direction; it's in column-major for correct mult*/
	GLfloat a[16] = { ((x*x*(1 - c)) +  c ), ((y*x*(1 - c)) + z*s), ((z*x*(1 - c)) - y*s), 0,
					  ((x*y*(1 - c)) - z*s), ((y*y*(1 - c)) +  c ), ((z*y*(1 - c)) + x*s), 0,
					  ((x*z*(1 - c)) + y*s), ((y*z*(1 - c)) - x*s), ((z*z*(1 - c)) +  c ), 0,
					  0,					 0,						0,					   1 };

	glMultMatrixf(a);

	return;
}

void my_lookat(GLdouble eX, GLdouble eY, GLdouble eZ, GLdouble cX, GLdouble cY, GLdouble cZ, GLdouble uX, GLdouble uY, GLdouble uZ)
{
	GLdouble fVec[3], fMag, fNorm[3], uVec[3], uMag, uNorm[3], s[3], sMag, sNorm[3], u[3];
	
	/*get the normalized vector for eye direction*/
	fVec[0] = cX - eX;
	fVec[1] = cY - eY;
	fVec[2] = cZ - eZ;
	fMag = sqrt((fVec[0] * fVec[0]) + (fVec[1] * fVec[1]) + (fVec[2] * fVec[2]));
	fNorm[0] = fVec[0] / fMag;
	fNorm[1] = fVec[1] / fMag;
	fNorm[2] = fVec[2] / fMag;

	/*normalize the up vector*/
	uVec[0] = uX;
	uVec[1] = uY;
	uVec[2] = uZ;
	uMag = sqrt((uVec[0] * uVec[0]) + (uVec[1] * uVec[1]) + (uVec[2] * uVec[2]));
	uNorm[0] = uVec[0] / uMag;
	uNorm[1] = uVec[1] / uMag;
	uNorm[2] = uVec[2] / uMag;

	/*s is used to determine the x*/
	s[0] = (fNorm[1] * uNorm[2]) - (fNorm[2] * uNorm[1]);
	s[1] = (fNorm[2] * uNorm[0]) - (fNorm[0] * uNorm[2]);
	s[2] = (fNorm[0] * uNorm[1]) - (fNorm[1] * uNorm[0]);
	sMag = sqrt((s[0] * s[0]) + (s[1] * s[1]) + (s[2] * s[2]));
	sNorm[0] = s[0] / sMag;
	sNorm[1] = s[1] / sMag;
	sNorm[2] = s[2] / sMag;

	/*u is used to determine the y*/
	u[0] = (sNorm[1] * fNorm[2]) - (sNorm[2] * fNorm[1]);
	u[1] = (sNorm[2] * fNorm[0]) - (sNorm[0] * fNorm[2]);
	u[2] = (sNorm[0] * fNorm[1]) - (sNorm[1] * fNorm[0]);

	/*lookat matrix in column-major format*/
	GLdouble m[16] = { s[0], u[0], (-1 * fNorm[0]), 0,
					   s[1], u[1], (-1 * fNorm[1]), 0,
					   s[2], u[2], (-1 * fNorm[2]), 0,
						  0,    0,				 0, 1 };

	glMultMatrixd(m);
	my_translate(-1 * eX, -1 * eY, -1 * eZ);

	return;
}

void my_ortho(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f)
{
	/*standard ortho matrix*/
	GLfloat m[16] = { (2 / (r - l)),						0,						  0, 0,
								  0,		    (2 / (t - b)),						  0, 0,
								  0,						0,			 (-2 / (f - n)), 0,
		   (-1 * (r + l) / (r - l)), (-1 * (t + b) / (t - b)), (-1 * (f + n) / (f - n)), 1 };
	glMultMatrixf(m);
	return;
}

void my_frustum(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f)
{
	/*standard frustum matrix*/
	GLfloat m[16] = { (2 * n / (r - l)),				   0,						 0,  0,
									  0,   (2 * n / (t - b)),						 0,  0,
					((r + l) / (r - l)), ((t + b) / (t - b)), (-1 * (f + n) / (f - n)), -1,
									  0,				   0,	(-2 * f * n / (f - n)),  0 };
	glMultMatrixf(m);
	return;
}

void my_perspective(GLfloat fov, GLfloat asp, GLfloat n, GLfloat f)
{
	/*cotangent of half the fov give the y distance in the view*/
	GLfloat cof = cosf(fov / 2) / sinf(fov / 2);
	/*standard perspective matrix*/
	GLfloat a[16] = { (cof / asp), 0, 0, 0,
		0, cof, 0, 0,
		0, 0, ((f + n) / (n - f)), -1,
		0, 0, (2 * f * n / (n - f)), 0 };
	glMultMatrixf(a);
	return;
}