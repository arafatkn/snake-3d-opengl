#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include <windows.h>
#include <GL/glut.h>
#include "draw.h"
using namespace std;
#define pi (2*acos(0.0))

double cameraHeight, cameraAngle;
double angle, map_angle;
double move_pos=2;
double l=140,w=140,h=20;
bool eaten, started=false, pause=false;
int sg=5,ar=0, score=0, highestScore;

point o(0,0,0);
point pos, fruit;
class segment
{
public:
    int ang,x,y,z;
    segment()
    {
        ang=0; x=0; y=0; z=0;
    }
};
point head;
segment sgs[1000];
void keyListener(unsigned char key, int x, int y);
void specialKeyListener(int key, int x,int y);
void drawBoard();
void write(string str)
{
    for(int i=0;i<str.length();i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    }
}
void saveHighestScore(int s=score)
{
    ofstream file;
    file.open ("F:\CodeBlocksProjects\Snake 3D\highest_score.txt");
    file << s;
    file.close();
}
int getHighestScore(int s=score)
{
    ifstream file;
    int sc=0;
    file.open ("F:\CodeBlocksProjects\Snake 3D\highest_score.txt");
    if(file.is_open())
    {
        file>>sc;
        file.close();
    }
    return sc;
}
void drawSnake()
{
    int i;
    point c1(0.6,0.1,0.1), c2(0,0,0), c3(1,1,1);

    for(i=1;i<sg;i++)
    {
        glPushMatrix();
            glTranslated(sgs[i].x*10, sgs[i].y*10, 0);
            draw::solid(o,10,10,8,c1,c2,c3);
        glPopMatrix();
    }
    point p=o;
    p.x -=coss(sgs[0].ang); p.y -=sinn(sgs[0].ang);
    c1.set(0,0,1);
    glPushMatrix();
        glTranslated(sgs[0].x*10, sgs[0].y*10, 0);
        draw::solid(p,12,12,10,c1,c2,c3);
    glPopMatrix();
}
bool isDeath()
{
    if(abs(head.x)>=l || abs(head.y)>=w)
    {
        return true;
    }
    //cout<<head.x<<" "<<head.y<<endl;
    for(int i=1;i<sg;i++)
    {
        if(sgs[0].x==sgs[i].x && sgs[0].y==sgs[i].y)
            return true;
    }
    return false;
}
bool isEaten()
{
    if(head.x==fruit.x && head.y==fruit.y)
        return true;
    else
        return false;
}
void drawFruit()
{
    if(eaten)
    {
        int ll = floor(l/10), ww=floor(w/10);
        fruit.x = (rand()%(2*ll)-ll)*10; fruit.y = (rand()%(2*ww)-ww)*10;
        if(fruit.x<=-l) fruit.x= -l+10;
        if(fruit.y<=-w) fruit.y= -w+10;
        if(fruit.x>=l) fruit.x= l-10;
        if(fruit.y>=w) fruit.y= w-10;
        eaten = false;
    }
    point f=fruit;
    f.x -= 2; f.y -= 2;
    point c1(0.6,0.1,0.1), c2(0,0,0), c3(1,1,1);
    draw::solid(f,6,6,8,c1,c2,c3);
}
void pregame()
{
	// Score
	glColor3f(1,1,1);
	glRasterPos3f(20 , 130, 10);
	stringstream ss;
	ss << highestScore;
	write("Highest Score: " + ss.str());

	glColor3f(1,0,0);
	glRasterPos3f(-100 , -50, 70);
	write("Press Key F1 for Starting the Game");

	glColor3f(0.9,0.9,0.9);
	glRasterPos3f(-100 , -50, 0);
	write("Snake Movements");
	glRasterPos3f(-100 , -65, 0);
	write("4 - Move Left");
	glRasterPos3f(-100 , -80, 0);
	write("6 - Move Right");
	glRasterPos3f(-100 , -95, 0);
	write("F1 - Start/Restart Game");
	glRasterPos3f(-100 , -110, 0);
	write("F2 - Pause/Resume Game");
//draw::axes();
    glColor3f(0,0,1);
	glRasterPos3f(-140 , -100, -20);
	write("Die if touch the border or itself");

	//Double Buffer
	glutSwapBuffers();
}
void display()
{
	//set the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/******** set-up camera here ********/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//1. where is the camera (viewer)? 2. where is the camera looking? 3. Which direction is the camera's UP direction?
    gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,-80,		0,0,1);
	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	if(!started)
    {
        pregame();
        return;
    }
	// Score
	glColor3f(1,1,1);
	glRasterPos3f(100 , 100, 70);
	stringstream ss,ss1;
	ss << score;
	write("SCORE: " + ss.str());
	glRasterPos3f(100 , 100, 40);
	ss1 << highestScore;
	write("Highest Score: " + ss1.str());

	// Add objects from here
	//draw::axes();
	drawBoard();
    drawFruit();
    if(isEaten())
    {
        eaten = true;
        drawFruit();
        score++;
        sg++;
    }

    if(isDeath())
    {
        if(score>highestScore) saveHighestScore(score);
        sgs[0]=sgs[1];
        drawSnake();
        glColor3f(1,1,1);
        glRasterPos3f(-50 , 0, 20);
        write("GAME OVER");
    }
    else
    {
        for(int i=sg-1;i>0;i--)
        {
            sgs[i] = sgs[i-1];
        }
        sgs[0].ang = ar;
        sgs[0].x += coss(ar);
        sgs[0].y += sinn(ar);
        //cout<<sgs[0].x<<" "<<sgs[0].y<<" "<<sgs[0].ang<<" "<<ar<<" "<<coss(ar)<<" "<<sinn(ar)<<endl<<endl;
        head.x += 10*coss(ar);
        head.y += 10*sinn(ar);
        if(!isDeath()) drawSnake();
    }

	//Double Buffer
	glutSwapBuffers();
}

void init()
{
	//codes for initialization
	cameraHeight=160.0; cameraAngle=pi+0.9; angle=0; map_angle=90;

	//clear the screen
	glClearColor(0,0,0,0);
	/************************
	/ set-up projection here
	************************/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	10,	1000.0);
	//field of view in the Y (vertically) //aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance //far distance
    sg = 5; ar=0;
    head.set(0,0,0);
	for(int i=0; i<sg; i++)
    {
        sgs[i].x = -i;
        sgs[i].y = 0;
        sgs[i].ang = 0;
    }
    eaten = true; pause = false;
    score = 0;
    highestScore = getHighestScore();
}
void animate(int value)
{
    if(!pause)
        glutPostRedisplay();
    glutTimerFunc(200,animate,1);
}
int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitWindowSize(980, 680);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Snake 3D");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutTimerFunc(200,animate,1);	//call after miliseconds

	glutSpecialFunc(specialKeyListener);
	glutKeyboardFunc(keyListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
void keyListener(unsigned char key, int x, int y)
{
    if(pause || !started)
        return;
    switch (key)
    {
        case 'a' :
        case '4' :
            ar ++;
            break;

        case 'd' :
        case '6' :
            ar --;
            break;

        /*case 'r' :
        case '5' :
            //glutPostRedisplay();
            break;*/
    }
    if(ar<0) ar = 4+ar;
    if(ar>=4) ar = ar%4;
    glutPostRedisplay();
}
void drawBoard()
{
    int d=10;
    glColor3f(0.2, 0.8, 0);
    point p1(l+2,w+2,0), p2(l+2,-w-2,0), p3(-l-2,-w-2,0), p4(-l-2,w+2,0);
    draw::rectangle(p1,p2,p3,p4);

    glBegin(GL_LINES);{
        glColor3f(0, 0, 0);
        for(int i=-l;i<=l;i+=d)
        {
            glVertex3f( l,i,0);
            glVertex3f(-l,i,0);
            glVertex3f(i,-w,0);
            glVertex3f(i, w,0);
        }
    }glEnd();

    glColor3f(1, 0, 0);
    double ll = l+2, ww = w+2;
    p1.set(ll,ww,0); p2.set(ll,ww,h); p3.set(ll,-ww,h); p4.set(ll,-ww,0);
    draw::rectangle(p1,p2,p3,p4);

    p1.set(ll,-ww,0); p2.set(ll,-ww,h); p3.set(-ll,-ww,h); p4.set(-ll,-ww,0);
    draw::rectangle(p1,p2,p3,p4);

    p1.set(-ll,-ww,0); p2.set(-ll,-ww,h); p3.set(-ll,ww,h); p4.set(-ll,ww,0);
    draw::rectangle(p1,p2,p3,p4);

    p1.set(-ll,ww,0); p2.set(-ll,ww,h); p3.set(ll,w,h); p4.set(ll,ww,0);
    draw::rectangle(p1,p2,p3,p4);
}
void specialKeyListener(int key, int x,int y)
{
	switch(key){
		case GLUT_KEY_DOWN:
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:
			cameraHeight += 3.0;
			break;
		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;
        case GLUT_KEY_F1:
            started = true;
			init();
			break;
        case GLUT_KEY_F2:
            pause = !pause;
			break;
		default:
			break;
        glutPostRedisplay();
	}
}
