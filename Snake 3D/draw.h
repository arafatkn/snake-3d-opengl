#ifndef DRAW_H
#define DRAW_H
#define pi (2*acos(0.0))
double d2r(double d)
{
    return (d)*(double)(pi/180);
}
double r2d(double r)
{
    return (r)*(double)(180/pi);
}
int sinn(int a)
{
    switch(a)
    {
        case 0: return 0;
        case 1: return 1;
        case 2: return 0;
        case 3: return -1;
    }
}
int coss(int a)
{
    switch(a)
    {
        case 0: return 1;
        case 1: return 0;
        case 2: return -1;
        case 3: return 0;
    }
}
class point
{
public:
    double x,y,z;
    point()
    {
        x=0; y=0; z=0;
    }
    point(double a, double b, double c)
    {
        x=a; y=b; z=c;
    }
    void set(double a, double b, double c)
    {
        x=a; y=b; z=c;
    }
    void reset()
    {
        x=0; y=0; z=0;
    }
};
class draw
{
public:
    static void axes()
    {
        glBegin(GL_LINES);{
            glColor3f(0, 1, 0);
            glVertex3f( 100,0,0);
            glVertex3f(-100,0,0);

            glColor3f(1, 0, 0);
            glVertex3f(0,-100,0);
            glVertex3f(0, 100,0);

            glColor3f(0, 0, 1);
            glVertex3f(0,0, 100);
            glVertex3f(0,0,-100);
        }glEnd();
    }
    static void triangle(point p1, point p2, point p3)
    {
        glBegin(GL_TRIANGLES);{
            glVertex3f( p1.x, p1.y, p1.z);
            glVertex3f( p2.x, p2.y, p2.z);
            glVertex3f( p3.x, p3.y, p3.z);
        }glEnd();
    }
    static void rectangle(point p1, point p2, point p3, point p4)
    {
        glBegin(GL_QUADS);{
            glVertex3f( p1.x, p1.y, p1.z);
            glVertex3f( p2.x, p2.y, p2.z);
            glVertex3f( p3.x, p3.y, p3.z);
            glVertex3f( p4.x, p4.y, p4.z);
        }glEnd();
    }
    static void solid(point p1, double l, double w, double h, point c1, point c2, point c3)
    {
        point p2,p3,p4,p5,p6,p7,p8;
        p2 = p3 = p4 = p1;
        p2=p1; p2.x -= l;
        p3=p2; p3.y -= w;
        p4=p1; p4.y -= w;
        p5=p1; p6=p2; p7=p3; p8=p4;
        p5.z += h; p6.z += h; p7.z += h; p8.z += h;

        glColor3f(c1.x,c1.y,c1.z);
        rectangle(p1,p2,p3,p4);
        rectangle(p5,p6,p7,p8);

        glColor3f(c2.x,c2.y,c2.z);
        rectangle(p1,p2,p6,p5);
        rectangle(p1,p4,p8,p5);
        rectangle(p2,p3,p7,p6);
        rectangle(p4,p3,p7,p8);
    }
};
#endif
