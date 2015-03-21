/************************************************************************		
\link	www.twinklingstar.cn
\author Twinkling Star
\date	2013/11/26
****************************************************************************/
#include <stdlib.h>
#include <GL/glut.h> 


typedef struct 
{
	float x;
	float y;
	float z;
}Vector3;

double fovy			= 45.0;
double nearPlane	= 1.0;
double farPlane		= 1000.0;


void drawScene(Vector3 pos)
{
	glColor3f(1.0f,1.0f,0);
	glPushMatrix();
		glTranslatef(pos.x,pos.y,pos.z);
		glutSolidSphere(1.0f,32,32);
	glPopMatrix();
}

void animation(Vector3 from,Vector3 to)
{
	int i , frameNum = 10;
	float dx = (from.x - to.x) / (float)frameNum;
	float dy = (from.y - to.y) / (float)frameNum;
	float dz = (from.z - to.z) / (float)frameNum;
	drawScene(to);
	//为了保证累积系数的和是1.0
	glAccum(GL_LOAD, 0.5);
	for( i=1 ; i<frameNum ; i++ )
	{
		glAccum(GL_ACCUM,0.05f);
		Vector3 pos = to;
		pos.x += dx*i; pos.y += dy*i; pos.z += dz*i;
		drawScene(pos);
	}
	glAccum(GL_RETURN,1.0f);
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Vector3 from , to;
	from.x = 0; from.y = 7.0f ; from.z = -30.0f;
	to.x = 0; to.y = 1.0f; to.z = -30.0f;

	animation(from,to);
	glFlush();
}

void init()
{
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearAccum(0.0f,0.0f,0.0f,0.0f);

	glEnable(GL_DEPTH_TEST);
}

void myReshape(GLsizei w,GLsizei h)
{
	glViewport(0,0,w,h);
	GLint viewport[4];
	glGetIntegerv (GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy,(GLdouble) viewport[2]/(GLdouble)viewport[3],nearPlane,farPlane);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc,char ** argv)
{

	glutInit(&argc,argv);
	//加入GLUT_ACCUM
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_ACCUM);
	glutInitWindowSize(400,400);
	glutCreateWindow("Accumulation Buffer:Motion Blur");
	init();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);

	glutMainLoop();
	return(0);
}