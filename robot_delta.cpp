#include <cstdio>
#include <unistd.h>
#include <GL/glut.h>
#include <pthread.h>
#include "delta_cau.h"


Pos pos(0.0,0.0,-1000);//work pos
Pos pos_t(100.0,100.0,-900.0);//target pos in moudle coord
GLfloat pos_w[3];//OpenGL world pos
Delta delta(400,200,350,1000);

Dfloat x_offset = 0.0f;
Dfloat y_offset = 0.0f;
Dfloat z_offset = 0.0f;


static GLfloat RATE = 300.0;
static void key(unsigned char key, int x, int y);
void coordTrans(const Pos &moudle, GLfloat *world);

void init(void);
void display(void);
void reshape (int w, int h);

void draw_triangle(GLfloat *p1,GLfloat *p2,GLfloat *p3);
void draw_l1(GLfloat angle,GLfloat theta);
void draw_l2();

void timerFunction(int value);

/*void *move_pthread(void *p);
void move_func(Pos *p);


void move_func(Pos *p)
{
  pthread_t id;
  void *thread_result;
  int retv,res;
  retv = pthread_create(&id,nullptr,*move_pthread,p);
  if(retv != 0)
    printf("Creat the move pthread failed. Code is %d\n",retv);
  // res = pthread_join(id,&thread_result);
  // if(res != 0)
  //   printf("Join the move pthread failed .Code is %d\n",res);
  // printf("move func is successed, and %s\n",(char *)thread_result);
}*/

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
  // move_func(&pos_t);//new thread.
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
    case 'a':
        pos.y = (pos.y + 5.0f);
        delta.inKin(pos);
        glutPostRedisplay();
        break;
    case 'd':
        pos.y = (pos.y - 5.0f);
        delta.inKin(pos);
        glutPostRedisplay();
        break;
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
    case 'g':
        x_offset =( pos_t.x - pos.x )/10.0;
        y_offset =( pos_t.y - pos.y )/10.0;
        z_offset =( pos_t.z - pos.z )/10.0;
        glutTimerFunc(100, timerFunction, 1);
        // move_func(&pos_t);
        // delta.inKin(pos);
        glutPostRedisplay();
        break;
    }
}

void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT);
  glColor3f(1.0,1.0,1.0);

  glLoadIdentity();  //加载单位矩阵

  gluLookAt(0.0,3.5,6.0,  0.0,0.0,0.0,  0.0,1.0,0.0);//camera pos,look at pos,the top of the camera to pos

  glTranslatef(0.0,2.0,0.0);
  


  GLfloat b1[3] = {-0.5f*delta.R/RATE,0.0,-0.866f*delta.R/RATE};
  GLfloat b2[3] = {-0.5f*delta.R/RATE,0.0,0.866f*delta.R/RATE};
  GLfloat b3[3] = {1.0f*delta.R/RATE,0.0,0.0};

  //fixed triangle
  draw_triangle(b1,b2,b3);
  
  //l1_arm
  for(int i = 0;i < 3;i++)
  {
    draw_l1(-60.0f + i*120.0f,delta.in_theta[i]*180/PI);
  }
  //l2_arm
  draw_l2();


  GLfloat p1[3];
  GLfloat p2[3];
  GLfloat p3[3];

  coordTrans(delta.P[0],p1);
  coordTrans(delta.P[1],p2);
  coordTrans(delta.P[2],p3);
  //work triangle
  glColor3f(0.2,0.4,1.0);
  draw_triangle(p1,p2,p3);


  //pos position.
  coordTrans(pos,pos_w);
  glPointSize(2.0f);
  glBegin(GL_POINTS);//必须是加上s，要不然显示不了
  glColor3f(1.0,0.0,0.0);
  glVertex3fv(pos_w);
  glEnd();


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
  gluLookAt (0.0, 3.5, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void draw_triangle(GLfloat *p1,GLfloat *p2,GLfloat *p3)
{
  glBegin(GL_LINE_LOOP);
    glVertex3fv(p1);
    glVertex3fv(p2);
    glVertex3fv(p3);
  glEnd();
}

void draw_l1(GLfloat angle,GLfloat theta)
{
  glPushMatrix();

  glRotatef(angle,0.0,1.0,0.0);
  glTranslatef(-1.0*delta.R/RATE,0.0,0.0);
  glRotatef(-theta,0.0,0.0,1.0);
  glTranslatef(0.0,-delta.l1/RATE/2.0f,0.0);
  glPushMatrix();
  glScalef(0.1f,delta.l1/RATE,0.2f);
  glutWireCube(1.0);
  glPopMatrix();


  glPopMatrix();
}

void draw_l2()
{

  GLfloat l2_len[3];
  delta.len_l2(delta.in_theta,pos,l2_len);

  glColor3f(1.0,1.0,0.0);

  GLfloat P1[3];
  GLfloat P2[3];
  GLfloat P3[3];

  GLfloat E1[3];
  GLfloat E2[3];
  GLfloat E3[3];

  coordTrans(delta.P[0],P1);
  coordTrans(delta.P[1],P2);
  coordTrans(delta.P[2],P3);
  
  coordTrans(delta.E[0],E1);
  coordTrans(delta.E[1],E2);
  coordTrans(delta.E[2],E3);

  
  glPushMatrix();

  glLineWidth(2.0f);
  if(fabs(l2_len[0] - delta.l2) > 5.0 )
    glColor3f(1.0,0.0,0.0);

  glBegin(GL_LINES);
  glVertex3fv(E1);
  glVertex3fv(P1);
  glColor3f(1.0,1.0,0.0);
  glEnd(); 

  glColor3f(1.0,1.0,0.0);
  if(fabs(l2_len[1] - delta.l2) > 5.0 )
    glColor3f(1.0,0.0,0.0);
  glBegin(GL_LINES);
  glVertex3fv(E2);
  glVertex3fv(P2);
  glEnd(); 

  glColor3f(1.0,1.0,0.0);
  if(fabs(l2_len[1] - delta.l2) > 5.0 )
    glColor3f(1.0,0.0,0.0);
  glBegin(GL_LINES);
  glVertex3fv(E3);
  glVertex3fv(P3);
  glEnd(); 

  glPopMatrix();
}

void coordTrans(const Pos &moudle, GLfloat *world){
    world[0] = -moudle.y/RATE;
    world[1] = moudle.z/RATE;
    world[2] = -moudle.x/RATE;
  }
void timerFunction(int value)
{
  if( fabs(pos.x-pos_t.x) < x_offset*0.9 )
    return;
  pos.x = pos.x + x_offset;
  pos.y = pos.y + y_offset;
  pos.z = pos.z + z_offset;
  printf("pos %f, %f, %f",pos.x,pos.y,pos.z);
  delta.inKin(pos);
  glutPostRedisplay();
  glutTimerFunc(100, timerFunction, 1);


}
/*void *move_pthread(void *p)
{
  Dfloat x_offset =( ((Pos *)p)->x - pos.x )/10.0;
  Dfloat y_offset =( ((Pos *)p)->y - pos.y )/10.0;
  Dfloat z_offset =( ((Pos *)p)->z - pos.z )/10.0;
  for(int i = 0;i < 10;i++)
  {
    pos.x = pos.x + x_offset;
    pos.y = pos.y + y_offset;
    pos.z = pos.z + z_offset;
    delta.inKin(pos);
    glutPostRedisplay();
    printf("pos %f, %f, %f",pos.x,pos.y,pos.z);
    usleep(100000);
  }
  pthread_exit(nullptr);
}*/




