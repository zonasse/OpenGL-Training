#include "GLTools.h"
#include "GLMatrixStack.h"
#include "GLFrame.h"
#include "GLFrustum.h"
#include "GLGeometryTransform.h"

#include <math.h>
#include <GLUT/GLUT.h>


// 着色管理器
GLShaderManager shaderManager;
// 设置角色帧，作为相机
GLFrame viewFrame;

// 透视投影
GLFrustum viewFrustum;

GLTriangleBatch torusBatch;

GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;

GLGeometryTransform transformPipeline;

// 背面剔除、深度测试
bool cullMark = false;
bool depthMark = false;

void mouseRightMenu(int value) {
    switch (value) {
        case 1:
            depthMark = !depthMark;
            break;
        case 2:
            cullMark = !cullMark;
            break;
        case 3:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case 4:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case 5:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}
/**
 窗口大小改变时调用的函数
 */
void windowSizeChange(int width, int height) {
    if (height == 0) {
        height = 1;
    }
    // 设置视口
    glViewport(0, 0, width, height);
    
    viewFrustum.SetPerspective(35, float(width) / float(height), 1.0f, 100.f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

/**
 渲染函数
 */
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    if (cullMark) {
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }
    
    if (depthMark) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    
    modelViewMatrix.PushMatrix(viewFrame);
    
    GLfloat vColor[] = {0.3f,0.5f,0.2f,1.0f};
    
    shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT,transformPipeline.GetModelViewMatrix(),transformPipeline.GetProjectionMatrix(),vColor);
    torusBatch.Draw();
    
    modelViewMatrix.PopMatrix();
    
    glutSwapBuffers();
    
}
void setupRC() {
    // 设置清屏颜色
    glClearColor(0.45f, 0.85f, 0.3f, 1.0f);
    // 初始化一个着色管理器
    shaderManager.InitializeStockShaders();
    
    viewFrame.MoveForward(7.0f);
    
    gltMakeTorus(torusBatch, 1.0f, 0.3f, 100, 26);
    
    glPointSize(4.0f);
    
}

void specialKeys(int key, int x, int y){
    
    if(key == GLUT_KEY_UP)
        viewFrame.RotateWorld(m3dDegToRad(-5.0), 1.0f, 0.0f, 0.0f);
    
    if(key == GLUT_KEY_DOWN)
        viewFrame.RotateWorld(m3dDegToRad(5.0), 1.0f, 0.0f, 0.0f);
    
    if(key == GLUT_KEY_LEFT)
        viewFrame.RotateWorld(m3dDegToRad(-5.0), 0.0f, 1.0f, 0.0f);
    
    if(key == GLUT_KEY_RIGHT)
        viewFrame.RotateWorld(m3dDegToRad(5.0), 0.0f, 1.0f, 0.0f);
    
    //重新刷新window
    glutPostRedisplay();
}

int main(int argc,char* argv[])

{
    gltSetWorkingDirectory(argv[0]);

    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("键盘控制");
    // 绑定重塑函数
    glutReshapeFunc(windowSizeChange);
    // 绑定显示函数
    glutDisplayFunc(renderScene);
    // 绑定特殊键函数
    glutSpecialFunc(specialKeys);
    // 绑定鼠标右键
    glutCreateMenu(mouseRightMenu);
    glutAddMenuEntry("开启深度测试",1);
    glutAddMenuEntry("开启背面剔除",2);
    glutAddMenuEntry("设置模式为填充", 3);
    glutAddMenuEntry("设置模式为线", 4);
    glutAddMenuEntry("设置模式为点", 5);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    GLenum status = glewInit();
    if (GLEW_OK != status) {
        printf("glew error : %s \n", glewGetErrorString(status));
        return 1;
    }
    //设置渲染环境
    setupRC();
    glutMainLoop();
    return 0;
    
}

