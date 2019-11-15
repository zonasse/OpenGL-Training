#include "GLTools.h"
#include "GLShaderManager.h"
#include "GLFrustum.h"
#include "GLBatch.h"
#include "GLMatrixStack.h"
#include "GLGeometryTransform.h"
#include "StopWatch.h"

#include <math.h>
#include <stdio.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

GLShaderManager     shaderManager;
GLMatrixStack       modelViewStack;
GLMatrixStack       projectionStack;
GLFrustum           viewFrustum;
GLGeometryTransform transformPipeline;
// 大球
GLTriangleBatch     torusBatch;
// 小球
GLTriangleBatch     sphereBatch;
// 地板
GLBatch             floorBatch;
// 相机帧
GLFrame             cameraFrame;
// 角色帧
GLFrame             objectFrame;

// 随机球
#define SPHERES_NUM 50
GLFrame spheres[SPHERES_NUM];

void changeSize(int w,int h) {
    glViewport(0, 0, w, h);
    viewFrustum.SetPerspective(35.0, float(w)/float(h), 1.0, 100.0f);
    projectionStack.LoadMatrix(viewFrustum.GetProjectionMatrix());
    transformPipeline.SetMatrixStacks(modelViewStack, projectionStack);
    
}

void renderScene() {
    static GLfloat vFloorColor[] = {0,0,1,1.0};
    static GLfloat vTorusColor[] = {1,0,0,1.0};
    static GLfloat vSphereColor[] = {0,1,0,1.0};
    
    // 基于时间动画
    static CStopWatch rotTimer;
    float yRot = rotTimer.GetElapsedSeconds() * 60.0f;
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    modelViewStack.PushMatrix(objectFrame);
    
    M3DVector4f vLightPos = {0,10,5,1.0f};
    
    shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),vFloorColor);
    floorBatch.Draw();
    
    modelViewStack.Translate(0, 0, -3.0f);
    modelViewStack.PushMatrix();
    modelViewStack.Rotate(yRot, 0, 1, 0);
    
    shaderManager.UseStockShader(GLT_SHADER_POINT_LIGHT_DIFF,transformPipeline.GetModelViewMatrix(),transformPipeline.GetProjectionMatrix(), vLightPos, vTorusColor);
    
    torusBatch.Draw();
    modelViewStack.PopMatrix();
    
    for (int i=0; i<SPHERES_NUM; ++i) {
        modelViewStack.PushMatrix();
        modelViewStack.MultMatrix(spheres[i]);
        shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),vSphereColor);
        sphereBatch.Draw();
        modelViewStack.PopMatrix();
    }
    
    modelViewStack.Rotate(yRot * -2.0, 0, 1, 0);
    modelViewStack.Translate(0.8f, 0, 0);
    shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(), vSphereColor);
    sphereBatch.Draw();
    modelViewStack.PopMatrix();
    
    glutSwapBuffers();
    glutPostRedisplay();
}

void specialKeys(int key,int x,int y) {
    // 移动步长
    float stepLen = 0.1f;
    float angular = float(m3dDegToRad(5.0f));
    if (key == GLUT_KEY_UP) {
        
        //MoveForward 平移
        objectFrame.MoveForward(-stepLen);
    }
    if (key == GLUT_KEY_DOWN) {
        objectFrame.MoveForward(stepLen);
    }
    
    if (key == GLUT_KEY_LEFT) {
        //RotateWorld 旋转
        objectFrame.RotateWorld(angular, 0, 1, 0);
    }
    
    if (key == GLUT_KEY_RIGHT) {
        objectFrame.RotateWorld(-angular, 0, 1, 0);
    }
}

void setupRC() {
    glClearColor(0, 0, 0, 1.0f);
    shaderManager.InitializeStockShaders();
    
    objectFrame.MoveForward(5.0f);
    
    glEnable(GL_DEPTH_TEST);
    // 设置地板数据
    floorBatch.Begin(GL_LINES, 324);
    for(GLfloat i = -20.0; i <= 20.0f; i+= 0.5) {
        floorBatch.Vertex3f(i, -0.55f, 20.0f);
        floorBatch.Vertex3f(i, -0.55f, -20.0f);
        
        floorBatch.Vertex3f(20.0f, -0.55f, i);
        floorBatch.Vertex3f(-20.0f, -0.55f, i);
    }
    floorBatch.End();
    
    // 设置大球数据
    gltMakeSphere(torusBatch, 0.4, 40, 80);
    // 设置小球数据
    gltMakeSphere(sphereBatch, 0.1, 26, 13);
    // 随机位置放置小球
    for (int i = 0; i < SPHERES_NUM; i++) {
        
        //y轴不变，X,Z产生随机值
        GLfloat x = ((GLfloat)((rand() % 400) - 200 ) * 0.1f);
        GLfloat z = ((GLfloat)((rand() % 400) - 200 ) * 0.1f);
        
        //在y方向，将球体设置为0.0的位置，这使得它们看起来是飘浮在眼睛的高度
        //对spheres数组中的每一个顶点，设置顶点数据
        spheres[i].SetOrigin(x, 0.0f, z);
    }
}


int main(int argc, char *argv[]) {
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("地球系");
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    glutSpecialFunc(specialKeys);
    
    GLenum status = glewInit();
    if (status != GLEW_OK) {
        printf("glew error %s \n", glewGetErrorString(status));
        return 1;
    }
    setupRC();
    glutMainLoop();
    
    return 0;
}
