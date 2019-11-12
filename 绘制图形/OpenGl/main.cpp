#include <iostream>
#include <GLUT/GLUT.h>
#include "math3d.h"

void draw() {
    
//    /**
//     正方形
//     */
//    glClearColor(0.5, 0.5, 0, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
//    glColor3f(0.3, 0.3, 0.3);
//    glOrtho(0, 1, 0, 1, -1.0, 1.0);
//    glBegin(GL_POLYGON);
//
//    //设置多边形的4个顶点
//    glVertex3f(0.25f, 0.25f, 0.0f);
//    glVertex3f(0.75f, 0.25f, 0.0f);
//    glVertex3f(0.75f, 0.75f, 0.0f);
//    glVertex3f(0.25f, 0.75f, 0.0f);
//
//    glEnd();
//    glFlush();
    

//    /**
//     圆
//     */
//    glClearColor(0.3, 0.7, 0.2, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);
//    glColor3f(0.1, 0.2, 0.2);
//    glBegin(GL_POLYGON);
//    const int n = 6;
//    const GLfloat R = 0.3f;
//    const GLfloat pi = 3.141592653f;
//
//    for (int i=0; i<n; ++i) {
//        glVertex2f(R * cos(2 * pi / n * i), R * sin(2 * pi / n * i));
//    }
//    glEnd();
//    glFlush();
    
    //    正弦函数
    const GLfloat factor = 0.1f;
    GLfloat x;
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);        // 以上两个点可以画x轴
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);        // 以上两个点可以画y轴
    glEnd();
    glBegin(GL_LINE_STRIP);
    for(x=-1.0f/factor; x<1.0f/factor; x+=0.01f)
    {
        glVertex2f(x*factor, sin(x)*factor);
    }
    glEnd();
    glFlush();
}

int main(int argc,char* argv[])

{

    glutInit(&argc, argv);
    
    glutCreateWindow("绘图");
    glutDisplayFunc(draw);
    glutMainLoop();
    return 0;
    
}

