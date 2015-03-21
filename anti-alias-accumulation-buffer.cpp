/************************************************************************		
\link	www.twinklingstar.cn
\author Twinkling Star
\date	2013/11/26
****************************************************************************/

#include <stdlib.h>
#include <GL/glut.h> 
#include <math.h>
#include <stdio.h>

#define PI 3.14159265358979323846
typedef struct  
{
	float x;
	float y;
}Vector2;

Vector2 jit2[2];
Vector2 jit4[4];
Vector2 jit6[6];
Vector2 jit8[8];
Vector2 jit12[12];
Vector2 jit16[16];

#define  JITTER_NUMBER 6

Vector2*	ptrJitter[JITTER_NUMBER];

double fovy			= 45.0;
double nearPlane	= 1.0;
double farPlane		= 1000.0;
int		jitterType	= 0;


void accFrustum(GLdouble left, GLdouble right, GLdouble bottom,
				GLdouble top, GLdouble near, GLdouble far, GLdouble pixdx, 
				GLdouble pixdy, GLdouble eyedx, GLdouble eyedy, 
				GLdouble focus)
{
	GLdouble xwsize, ywsize; 
	GLdouble dx, dy;
	GLint viewport[4];

	glGetIntegerv (GL_VIEWPORT, viewport);

	xwsize = right - left;
	ywsize = top - bottom;

	//xwsize/viewport[2]相当于一个像素在视锥的近平面上的水平尺寸，
	//累积缓冲区在抗锯齿操作中，eyedx为0，pixdx是随机数；
	//累积缓冲区在深度场的应用中，pixdx为0，eyedx是随机数。
	dx = -(pixdx*xwsize/(GLdouble) viewport[2] + eyedx*near/focus);
	//ywsize/viewport[3]相当于一个像素在视锥的近平面上的垂直尺寸
	//累积缓冲区在抗锯齿操作中，eyedy为0，pixdy是随机数；
	//累积缓冲区在深度场的应用中，pixdy为0，eyedy是随机数。
	dy = -(pixdy*ywsize/(GLdouble) viewport[3] + eyedy*near/focus);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum (left + dx, right + dx, bottom + dy, top + dy,near, far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef (-eyedx, -eyedy, 0.0);
}

void accPerspective(GLdouble fovy, GLdouble aspect,GLdouble near, GLdouble far, 
					GLdouble pixdx, GLdouble pixdy, 
					GLdouble eyedx, GLdouble eyedy, 
					GLdouble focus)
{
	//gluPerpective与glFrustum两个函数实现的功能是相同的，
	//不同的是这两个函数的参数，所以两个函数的参数有一定的对应关系。
	GLdouble fov2,left,right,bottom,top;
	fov2 = ((fovy*PI) / 180.0) / 2.0;

	top		= near / (cos(fov2) / sin(fov2));
	bottom	= -top;
	right	= top * aspect;
	left	= -right;

	accFrustum (left, right, bottom, top, near, far,pixdx, pixdy, eyedx, eyedy, focus);
}

/*
\brief 得到随机数，这些随机数从《OpenGL Programming Guide-Sixth Edition-The Official 
		Guide Learning OpenGL, Version 2.1》书中第10.3.5节
*/
void initJitter()
{
	jit2[0].x = 0.25;jit2[0].y=0.75;
	jit2[1].x = 0.75;jit2[1].y=0.25;

	jit4[0].x = 0.375;jit4[0].y=0.25;
	jit4[1].x = 0.125;jit4[1].y=0.75;
	jit4[2].x = 0.875;jit4[2].y=0.25;
	jit4[3].x = 0.625;jit4[3].y=0.75;
	
	jit6[0].x = 0.4646464646;jit6[0].y=0.4646464646;
	jit6[1].x = 0.1313131313;jit6[1].y=0.7979797979;
	jit6[2].x = 0.5353535353;jit6[2].y=0.8686868686;
	jit6[3].x = 0.8686868686;jit6[3].y=0.5353535353;
	jit6[4].x = 0.7979797979;jit6[4].y=0.1313131313;
	jit6[5].x = 0.2020202020;jit6[5].y=0.2020202020;

	jit8[0].x = 0.5624;jit8[0].y=0.4375;
	jit8[1].x = 0.0625;jit8[1].y=0.9375;
	jit8[2].x = 0.3125;jit8[2].y=0.6875;
	jit8[3].x = 0.6875;jit8[3].y=0.8125;
	jit8[4].x = 0.8125;jit8[4].y=0.1875;
	jit8[5].x = 0.9375;jit8[5].y=0.5625;
	jit8[6].x = 0.4375;jit8[6].y=0.0625;
	jit8[7].x = 0.1875;jit8[7].y=0.3125;

	jit12[0].x = 0.4166666666;jit12[0].y=0.625;
	jit12[1].x = 0.9166666666;jit12[1].y=0.875;
	jit12[2].x = 0.25;jit12[2].y=0.375;
	jit12[3].x = 0.4166666666;jit12[3].y=0.125;
	jit12[4].x = 0.75;jit12[4].y=0.125;
	jit12[5].x = 0.0833333333;jit12[5].y=0.125;
	jit12[6].x = 0.75;jit12[6].y=0.625;
	jit12[7].x = 0.25;jit12[7].y=0.875;
	jit12[8].x = 0.5833333333;jit12[8].y=0.375;
	jit12[9].x = 0.9166666666;jit12[9].y=0.375;
	jit12[10].x = 0.0833333333;jit12[10].y=0.625;
	jit12[11].x = 0.583333333;jit12[11].y=0.875;


	jit16[0].x = 0.375;jit16[0].y=0.4375;
	jit16[1].x = 0.625;jit16[1].y=0.0625;
	jit16[2].x = 0.875;jit16[2].y=0.1875;
	jit16[3].x = 0.125;jit16[3].y=0.0625;
	jit16[4].x = 0.375;jit16[4].y=0.6875;
	jit16[5].x = 0.875;jit16[5].y=0.4375;
	jit16[6].x = 0.625;jit16[6].y=0.5625;
	jit16[7].x = 0.375;jit16[7].y=0.9375;
	jit16[8].x = 0.625;jit16[8].y=0.3125;
	jit16[9].x = 0.125;jit16[9].y=0.5625;
	jit16[10].x = 0.125;jit16[10].y=0.8125;
	jit16[11].x = 0.375;jit16[11].y=0.1875;
	jit16[12].x = 0.875;jit16[12].y=0.9375;
	jit16[13].x = 0.875;jit16[13].y=0.6875;
	jit16[14].x = 0.125;jit16[14].y=0.3125;
	jit16[15].x = 0.625;jit16[15].y=0.8125;

	ptrJitter[0] = jit2;
	ptrJitter[1] = jit4;
	ptrJitter[2] = jit6;
	ptrJitter[3] = jit8;
	ptrJitter[4] = jit12;
	ptrJitter[5] = jit16;
}

/*
\brief 绘制一个棋盘网格
*/
void drawPlane( Vector2 minPoint,Vector2 maxPoint,float yLoc)
{
	int x , z;
	float xLen = maxPoint.x - minPoint.x;
	float zLen = maxPoint.y - minPoint.y;
	float step = 0.1f;
	int xEnd = xLen / step;
	int zEnd = zLen / step;
	glBegin(GL_QUADS);
	for( x=0 ; x<=xEnd ; x++ )
		for( z=0 ; z<=zEnd ; z++ )
		{
			if( (x+z)%2==0 )
				glColor3f(0,0,0);
			else
				glColor3f(1.0f,1.0f,1.0f);
			glVertex3f(minPoint.x + step*x , yLoc , minPoint.y + step*z);
			glVertex3f(minPoint.x + step*(x+1) , yLoc , minPoint.y + step*z);
			glVertex3f(minPoint.x + step*(x+1) , yLoc , minPoint.y + step*(z+1));
			glVertex3f(minPoint.x + step*x , yLoc , minPoint.y + step*(z+1));
		}
		glEnd();
}

/*
\brief 绘制一个场景
*/
void drawScene()
{
	Vector2 minPoint , maxPoint;
	float size = 10.0f;
	minPoint.x = -size;minPoint.y=0;minPoint.y=-size;
	maxPoint.x = size;maxPoint.y=0;maxPoint.y=size;

	drawPlane(minPoint,maxPoint,0.0f);
}



/*
\brief 使用累积缓冲区绘制一张图像，有抗锯齿的效果
*/
void drawAccum()
{
	GLint viewport[4];
	glGetIntegerv (GL_VIEWPORT, viewport);
	int jitterNum , accumSize = 8;

	glClear(GL_ACCUM_BUFFER_BIT);
	for (jitterNum = 0; jitterNum < accumSize; jitterNum++) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		accPerspective (fovy,(GLdouble) viewport[2]/(GLdouble)viewport[3],nearPlane, farPlane, 
						ptrJitter[jitterType][jitterNum].x, ptrJitter[jitterType][jitterNum].y, 
						0.0, 0.0,
						1.0);

		glTranslatef(0,-1.0f,0);
		drawScene();
		glAccum(GL_ACCUM, 1.0/accumSize);
	}
	glAccum (GL_RETURN, 1.0);
}

/*
\brief 绘制一张没有抗锯齿效果的图像
*/
void drawNormal()
{
	GLint viewport[4];
	glGetIntegerv (GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy,(GLdouble) viewport[2]/(GLdouble)viewport[3],nearPlane,farPlane);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,1.0f,0);
	drawScene();
}

void myDisplay(void)
{
	drawAccum();

	drawNormal();

	glFlush();
}


/*
\brief 键盘消息函数，按空格键，在调用drawAccum()绘制图像时，会改变累积缓冲区中的累积次数
*/
void myKeys(unsigned char key, int x, int y) 
{
	int jiterNum[JITTER_NUMBER] = {2,4,6,8,12,16};
	switch(key) 
	{
	case ' ':
		jitterType = (jitterType + 1)%JITTER_NUMBER;
		printf("Number of Jitterring:%d\n",jiterNum[jitterType]);
		break;
	}
	myDisplay();
}

void init()
{
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearAccum(0.0f,0.0f,0.0f,0.0f);

	glEnable(GL_DEPTH_TEST);
	initJitter();
}

void myReshape(GLsizei w,GLsizei h)
{
	glViewport(0,0,w,h);
}

int main(int argc,char ** argv)
{
	glutInit(&argc,argv);
	//加入GLUT_ACCUM
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_ACCUM);
	glutInitWindowSize(400,400);

	glutCreateWindow("Accumulation Buffer:Anti-alias");

	init();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeys);

	glutMainLoop();
	return(0);
}