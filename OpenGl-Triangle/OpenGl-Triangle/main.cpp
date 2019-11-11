#include "GLShaderManager.h"
#include "GLTools.h"
#include <GLUT/GLUT.h>


// 着色管理器
GLShaderManager shaderManager;

// 批次容器
GLBatch triangleBatch;

/**
 窗口大小改变时调用的函数
 */
void windowSizeChange(int width, int height) {
    // 设置视口
    glViewport(0, 0, width, height);
}

/**
 渲染函数
 */
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    GLfloat vRed[] = {1.0,0.0,0.0,1.0f};
    // 将颜色传递到存储着色器
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vRed);
    // 提交着色器
    triangleBatch.Draw();
    // 交换后台缓冲区到前台
    glutSwapBuffers();
}
void setupRC() {
    // 设置清屏颜色
    glClearColor(0.45f, 0.85f, 0.3f, 1.0f);
    // 初始化一个着色管理器
    shaderManager.InitializeStockShaders();
    // 指定顶点
    GLfloat vVerts[] = {
        -0.5f,0.0f,0.0f,
        0.5f,0,0,
        0,0.5,0
    };
    triangleBatch.Begin(GL_TRIANGLES, 3);
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
    
}
int main(int argc,char* argv[])

{
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("triangle");
    // 绑定重塑函数
    glutReshapeFunc(windowSizeChange);
    // 绑定显示函数
    glutDisplayFunc(renderScene);
    
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

