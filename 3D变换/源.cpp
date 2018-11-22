//������ �϶��ɿ���������ת��x,y,z����������ֱ���x�ᣬy�ᣬz����ת
//���ܼ�PageUp ��������Ŵ�PageDown����������С
//�������������������������ƶ�
//


#include <GL/glut.h>
#include <iostream>

const GLint winWidth = 500, winHeight = 500;

typedef GLfloat Matrix4x4[4][4];

Matrix4x4 eyeMatrix4x4 = { 1,0,0,0,
						  0,1,0,0,
						  0,0,1,5,
						  0,0,0,1 };	//�����λ�õ�4x4����
Matrix4x4 centerMatrix4x4 = { 1,0,0,0,
							  0,1,0,0,
							  0,0,1,0,
							  0,0,0,1 };	//�۲����ĵ�4x4����

double eye[] = { eyeMatrix4x4[0][3], eyeMatrix4x4[1][3], eyeMatrix4x4[2][3] };	//�۾���λ��
double center[] = { centerMatrix4x4[0][3], centerMatrix4x4[1][3], centerMatrix4x4[2][3] };//�۲����ĵ�λ��
double place[] = { 0, 1, 0 };//�Դ��ĳ���Ĭ�ϳ���y��������

double angle_x = 0;	//x����ת�ǶȲ���
double angle_y = 0;	//y����ת�ǶȲ���
double angle_z = 0;	//z����ת�ǶȲ���

double times = 1.0;	//���ŵı���
const double move_x = 0;	//x��Ĭ���ƶ�����0
const double move_y = 0;	//y��Ĭ���ƶ�����0
const double move_z = 0;	//z��Ĭ���ƶ�����0

GLint oldx = 0;	//��������ά�����ƶ������У�ÿһ�ζ�Ҫ����λ��������ʼ���������
GLint oldy = 0;

int project_mode = 0;	//ͶӰģʽ��0����͸��ͶӰ��1������ͶӰ

void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);	//set color to white
}
/*ƽ��һ����ά���壬ͨ������任ʵ����һ���Ѷȣ����ǿ���ͨ������任�ı�������͹۲����ĵ�λ�ã�
 *ֻҪ����ͬ���ƶ���ͬ������ʵ����ά�����ƶ���Ч��
 */
void updateEyeAndCenter()	//������͹۲�����λ�ø���
{
	for (int i = 0; i < 3; i++)
	{
		eye[i] = eyeMatrix4x4[i][3];
		center[i] = centerMatrix4x4[i][3];
	}
}

void matrix4x4SetIdentity(Matrix4x4 matIdent4x4)	//����һ��4x4��λ����
{
	GLint row, col;

	for (row = 0; row < 4; row++)
		for (col = 0; col < 4; col++)
			matIdent4x4[row][col] = (row == col);
}

void matrix4x4PreMultiply(Matrix4x4 m1, Matrix4x4 m2)	//m2 = m1*m2
{
	GLint row, col;
	Matrix4x4 matTemp;

	for (row = 0; row < 4; row++)
		for (col = 0; col < 4; col++)
			matTemp[row][col] = m1[row][0] * m2[0][col] + m1[row][1] * m2[1][col] + m1[row][2] * m2[2][col] + m1[row][3] * m2[3][col];
	for (row = 0; row < 4; row++)
		for (col = 0; col < 4; col++)
			m2[row][col] = matTemp[row][col];
}

void translate3D(GLfloat tx, GLfloat ty, GLfloat tz)	//3άƽ��
{
	Matrix4x4 matTransl3D;

	matrix4x4SetIdentity(matTransl3D);

	matTransl3D[0][3] = tx;
	matTransl3D[1][3] = ty;
	matTransl3D[2][3] = tz;

	matrix4x4PreMultiply(matTransl3D, eyeMatrix4x4);
	matrix4x4PreMultiply(matTransl3D, centerMatrix4x4);
	updateEyeAndCenter();
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�����ɫ�������Ȼ���
	glColor3f(1.0, 0.0, 0.0);	//������ɫΪ��ɫ
	glLoadIdentity();	//��ʼ����λ����
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], place[0], place[1], place[2]);

	glEnable(GL_DEPTH_TEST);  //������Ȳ���
	glEnable(GL_LIGHTING); //��������ģʽ
	GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 }; // ������ɫ
	GLfloat light_pos[] = { 5,5,5,1 };  //�����Դλ��

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos); //���õ�0�Ź�Դ�Ĺ���λ��
	glLightfv(GL_LIGHT0, GL_AMBIENT, red); //���õ�0�Ź�Դ��η����Ĺ�����ɫ����������ɫ��
	glEnable(GL_LIGHT0);					//������0�Ź�Դ

	glScalef(times, times, times);
	glRotated(angle_x, 1, 0, 0);	//x����ת�Ƕ�
	glRotated(angle_y, 0, 1, 0);	//y����ת�Ƕ�
	glRotated(angle_z, 0, 0, 1);	//z����ת�Ƕ�
	glutSolidTeapot(2);

	glutSwapBuffers();
}

void keyboard(GLubyte key, GLint xMouse, GLint yMouse)
{
	switch (key)
	{
	case 'x':
		angle_x = (int)(angle_x + 1) % 360;
		break;
	case 'y':
		angle_y = (int)(angle_y + 1) % 360;
		break;
	case 'z':
		angle_z = (int)(angle_z + 1) % 360;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void scaleFcn(GLint specialKey, GLint xMouse, GLint yMOuse)
{
	switch (specialKey)
	{
	case GLUT_KEY_PAGE_UP:
		times += 0.1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		if (times <= 0.1)	times = 0.1;
		else	times -= 0.1;
		break;
	case GLUT_KEY_UP:
		translate3D(move_x, -0.1, move_z);
		break;
	case GLUT_KEY_DOWN:
		translate3D(move_x, 0.1, move_z);
		break;
	case GLUT_KEY_RIGHT:
		translate3D(-0.1, move_y, move_z);
		break;
	case GLUT_KEY_LEFT:
		translate3D(0.1, move_y, move_z);
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			oldx = x;//���������ʱ��¼�������  
			oldy = y;
		}
	}
}

void motion(GLint xMouse, GLint yMouse)
{
	GLint delta_x = xMouse - oldx;
	GLint delta_y = yMouse - oldy;

	angle_y += (360 * delta_x / winWidth);
	angle_x += (360 * delta_y / winHeight);
	//angle_z += (360 * delta_y / winHeight);
	oldx = xMouse;
	oldy = yMouse;
	glutPostRedisplay();
}

void reshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);	//��ͼ���ڴ�С
	glMatrixMode(GL_PROJECTION);	//����ΪͶӰģʽ
	glLoadIdentity();
	if (project_mode == 0)
		gluPerspective(90, (GLdouble)newWidth / (GLdouble)newHeight, 1, 100);	//͸��ͶӰ
	else
		glOrtho(-10, 10, -10, 10, -100, 100);	//ƽ��ͶӰ
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], place[0], place[1], place[2]);	//�����λ��
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);//��ʼ����ʾģʽ:RGB��ɫģ�ͣ���Ȳ��ԣ�˫����
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("��ά����");
	init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(reshapeFcn);

	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(scaleFcn);
	//glutIdleFunc(myDisplay);
	glutMainLoop();
	return 0;
}