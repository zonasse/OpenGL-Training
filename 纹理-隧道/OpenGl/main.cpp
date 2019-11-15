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

// 批量容器类
GLBatch floorBatch;
GLBatch ceilingBatch;
GLBatch leftWallBatch;
GLBatch rightWallBatch;

// 深度初始值
GLfloat viewZ = -65.0f;

#define TEXTURE_BRICK 0
#define TEXTURE_FLOOR 1
#define TEXTURE_CEILING 2
#define TEXTURE_COUNT 3

// 纹理数组
GLuint textures[TEXTURE_COUNT];

// tga数组
const char *textureFiles[TEXTURE_COUNT] = {"brick.tga","floor.tga","floor.tga"};



void changeSize(int w,int h) {
    glViewport(0, 0, w, h);
    viewFrustum.SetPerspective(35.0, float(w)/float(h), 1.0, 100.0f);
    projectionStack.LoadMatrix(viewFrustum.GetProjectionMatrix());
    transformPipeline.SetMatrixStacks(modelViewStack, projectionStack);
    
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    modelViewStack.PushMatrix();
    modelViewStack.Translate(0, 0, viewZ);
    shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(),0);
    
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_FLOOR]);
    floorBatch.Draw();
    
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_CEILING]);
    ceilingBatch.Draw();
    
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_BRICK]);
    leftWallBatch.Draw();
    rightWallBatch.Draw();
    
    modelViewStack.PopMatrix();
    glutSwapBuffers();

}

void specialKeys(int key,int x,int y) {

    if(key == GLUT_KEY_UP)
        //移动的是深度值，Z
        viewZ += 0.5f;
    
    if(key == GLUT_KEY_DOWN)
        viewZ -= 0.5f;
    
    //更新窗口，即可回调到RenderScene函数里
    glutPostRedisplay();
}

void setupRC() {
    glClearColor(0, 0, 0, 1.0f);
    
    shaderManager.InitializeStockShaders();
    
    
    // 设置纹理数据
    GLbyte *pBytes;
    GLint iWidth,iHeight,iComponents;
    GLenum eFormat;
    GLint iLoop;
    
    glGenTextures(TEXTURE_COUNT, textures);
    
    for (iLoop = 0; iLoop<TEXTURE_COUNT; ++iLoop) {
        glBindTexture(GL_TEXTURE_2D, textures[iLoop]);
        pBytes = gltReadTGABits(textureFiles[iLoop], &iWidth, &iHeight, &iComponents, &eFormat);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
        glGenerateMipmap(GL_TEXTURE_2D);
        free(pBytes);
    }
    // 设置顶点数据
    GLfloat z;
    floorBatch.Begin(GL_TRIANGLE_STRIP, 28,1);
    for (z = 60.0f; z >= 0.0f; z -= 10.0f) {
        floorBatch.MultiTexCoord2f(0, 0, 0);
        floorBatch.Vertex3f(-10, -10, z);
        
        floorBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
        floorBatch.Vertex3f(10.0f, -10.0f, z);
        
        floorBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
        floorBatch.Vertex3f(-10.0f, -10.0f, z - 10.0f);
        
        floorBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
        floorBatch.Vertex3f(10.0f, -10.0f, z - 10.0f);
    }
    floorBatch.End();
    
    ceilingBatch.Begin(GL_TRIANGLE_STRIP, 28, 1);
    for(z = 60.0f; z >= 0.0f; z -=10.0f)
    {
        ceilingBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
        ceilingBatch.Vertex3f(-10.0f, 10.0f, z - 10.0f);
        
        ceilingBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
        ceilingBatch.Vertex3f(10.0f, 10.0f, z - 10.0f);
        
        ceilingBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
        ceilingBatch.Vertex3f(-10.0f, 10.0f, z);
        
        ceilingBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
        ceilingBatch.Vertex3f(10.0f, 10.0f, z);
    }
    ceilingBatch.End();
    

    leftWallBatch.Begin(GL_TRIANGLE_STRIP, 28, 1);
    for(z = 60.0f; z >= 0.0f; z -=10.0f)
    {
        leftWallBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
        leftWallBatch.Vertex3f(-10.0f, -10.0f, z);
        
        leftWallBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
        leftWallBatch.Vertex3f(-10.0f, 10.0f, z);
        
        leftWallBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
        leftWallBatch.Vertex3f(-10.0f, -10.0f, z - 10.0f);
        
        leftWallBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
        leftWallBatch.Vertex3f(-10.0f, 10.0f, z - 10.0f);
    }
    leftWallBatch.End();
    

    rightWallBatch.Begin(GL_TRIANGLE_STRIP, 28, 1);
    for(z = 60.0f; z >= 0.0f; z -=10.0f)
    {
        rightWallBatch.MultiTexCoord2f(0, 0.0f, 0.0f);
        rightWallBatch.Vertex3f(10.0f, -10.0f, z);
        
        rightWallBatch.MultiTexCoord2f(0, 0.0f, 1.0f);
        rightWallBatch.Vertex3f(10.0f, 10.0f, z);
        
        rightWallBatch.MultiTexCoord2f(0, 1.0f, 0.0f);
        rightWallBatch.Vertex3f(10.0f, -10.0f, z - 10.0f);
        
        rightWallBatch.MultiTexCoord2f(0, 1.0f, 1.0f);
        rightWallBatch.Vertex3f(10.0f, 10.0f, z - 10.0f);
    }
    rightWallBatch.End();
    
}

void shutdownRC() {
    glDeleteTextures(TEXTURE_COUNT, textures);
}

void processMenu(int value) {
    GLint iLoop;
    
    for(iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
    {
        /**绑定纹理 glBindTexture
         参数1：GL_TEXTURE_2D
         参数2：需要绑定的纹理对象
         */
        glBindTexture(GL_TEXTURE_2D, textures[iLoop]);
        
        /**配置纹理参数 glTexParameteri
         参数1：纹理模式
         参数2：纹理参数
         参数3：特定纹理参数
         
         */
        switch(value)
        {
            case 0:
                //GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER(缩小过滤器)，GL_NEAREST（最邻近过滤）
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                break;
                
            case 1:
                //GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER(缩小过滤器)，GL_LINEAR（线性过滤）
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                break;
                
            case 2:
                //GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER(缩小过滤器)，GL_NEAREST_MIPMAP_NEAREST（选择最邻近的Mip层，并执行最邻近过滤）
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                break;
                
            case 3:
                //GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER(缩小过滤器)，GL_NEAREST_MIPMAP_LINEAR（在Mip层之间执行线性插补，并执行最邻近过滤）
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                break;
                
            case 4:
                //GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER(缩小过滤器)，GL_NEAREST_MIPMAP_LINEAR（选择最邻近Mip层，并执行线性过滤）
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                break;
                
            case 5:
                //GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER(缩小过滤器)，GL_LINEAR_MIPMAP_LINEAR（在Mip层之间执行线性插补，并执行线性过滤，又称为三线性过滤）
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                break;
                
            case 6:
                
                //设置各向异性过滤
                GLfloat fLargest;
                //获取各向异性过滤的最大数量
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
                //设置纹理参数(各向异性采样)
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
                break;
                
            case 7:
                //设置各向同性过滤，数量为1.0表示(各向同性采样)
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
                break;
                
        }
    }
    
    //触发重画
    glutPostRedisplay();
}
int main(int argc, char *argv[]) {
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("隧道");
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    glutSpecialFunc(specialKeys);
    
    // 添加菜单入口，改变过滤器
    glutCreateMenu(processMenu);
    glutAddMenuEntry("GL_NEAREST",0);
    glutAddMenuEntry("GL_LINEAR",1);
    glutAddMenuEntry("GL_NEAREST_MIPMAP_NEAREST",2);
    glutAddMenuEntry("GL_NEAREST_MIPMAP_LINEAR", 3);
    glutAddMenuEntry("GL_LINEAR_MIPMAP_NEAREST", 4);
    glutAddMenuEntry("GL_LINEAR_MIPMAP_LINEAR", 5);
    glutAddMenuEntry("Anisotropic Filter", 6);
    glutAddMenuEntry("Anisotropic Off", 7);
    
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    GLenum status = glewInit();
    if (status != GLEW_OK) {
        printf("glew error %s \n", glewGetErrorString(status));
        return 1;
    }
    setupRC();
    glutMainLoop();
    shutdownRC();
    return 0;
}
