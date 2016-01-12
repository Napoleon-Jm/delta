#include <GL/glut.h>
#include "delta_cau.h"


Pos pos(0.0,0.0,-600);
GLfloat pos_w[3];
Delta delta(400,400,600,600);
// static const GLfloat PI = 3.1415926;
static int shoulder = 0;
static int elbow = 0;
static int theta1 = 0;
static GLfloat R = 1.0f;
static void key(unsigned char key, int x, int y);
void coordTrans(const Pos &moudle, GLfloat *world);

void init(void);
void display(void);
void reshape (int w, int h);

void draw_l1(GLfloat angle,GLfloat theta);

int main(int argc, char** argv)
{
  
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (500, 500); 
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  glutKeyboardFunc(key);
  init ();
  glutDisplayFunc(display); 
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;
}


void init(void) 
{
  
  glClearColor (0.0, 0.0, 0.0, 0.0); //背景黑色
  glShadeModel(GL_FLAT);
  delta.inKin(pos);
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break; //按ESC键(ASCII码为27)和q键为退出
    case 'w':
        pos.x = (pos.x + 5.0f);
        delta.inKin(pos);
        glutPostRedisplay();
        break;
    case 's':
        pos.x = (pos.x - 5.0f);
        delta.inKin(pos);
        glutPostRedisplay();
        break;
    case 'e':
        elbow = (elbow + 5)%360;
        glutPostRedisplay();
        break;
    case 'E':
        elbow = (elbow - 5)%360;
        glutPostRedisplay();
        break;
    case 'b':
        theta1 = (theta1+5)%360;
        glutPostRedisplay();
        break;
    }
}

void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT);
  glColor3f(1.0,1.0,1.0);

  glLoadIdentity();  //加载单位矩阵

  gluLookAt(0.0,5.0,5.0,  0.0,0.0,0.0,  0.0,1.0,0.0);

  glTranslatef(0.0,3.0,0.0);
  


  GLfloat b1[3] = {-0.5f*R,0.0,-0.866f*R};
  GLfloat b2[3] = {-0.5f*R,0.0,0.866f*R};
  GLfloat b3[3] = {1.0f*R,0.0,0.0};

  glBegin(GL_LINE_LOOP);
    glVertex3fv(b1);
    glVertex3fv(b2);
    glVertex3fv(b3);
  glEnd();


  //l1_arm
  for(int i = 0;i < 3;i++)
  {
    draw_l1(-60.0f + i*120.0f,delta.in_theta[i]*180/PI);
  }



  coordTrans(pos,pos_w);
  glPointSize(2.0f);
  glBegin(GL_POINTS);//必须是加上s，要不然显示不了
  glColor3f(1.0,0.0,0.0);
  glVertex3fv(pos_w);
  glEnd();
/*
  //shoulder

  glPushMatrix();
  glTranslatef(-1.0,0.0,0.0);
  glRotatef(shoulder,0.0,0.0,1.0);
  glTranslatef(1.0,0.0,0.0);

  glPushMatrix();
  glScalef(2.0,0.4,1.0);
  glutWireCube(1.0);
  glPopMatrix();

  //arm
  glTranslatef(1.0,0.0,0.0);
  glRotatef((GLfloat)elbow,0.0,0.0,1.0);
  glTranslatef(1.0,0.0,0.0);

  glPushMatrix();
  glScalef(2.0,0.4,1.0);
  glutWireCube(1.0);
  glPopMatrix();



  glPopMatrix();
*/

  glutSwapBuffers();


}

void reshape (int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glTranslatef(0.0,0.0,-5.0);
}

void draw_l1(GLfloat angle,GLfloat theta)
{
  glPushMatrix();

  glRotatef(angle,0.0,1.0,0.0);
  glTranslatef(-1.0,0.0,0.0);
  glRotatef(-theta,0.0,0.0,1.0);
  glTranslatef(0.0,-0.75,0.0);
  glPushMatrix();
  glScalef(0.1f,1.5f,0.2f);
  glutWireCube(1.0);
  glPopMatrix();


  glPopMatrix();
}

void coordTrans(const Pos &moudle, GLfloat *world){
    world[0] = -moudle.y/delta.R;
    world[1] = moudle.z/delta.R;
    world[2] = -moudle.x/delta.R;
  }


