#include "GLShaderManager.h"
#include "GLTools.h"
#include <GLUT/GLUT.h>


// 着色管理器
GLShaderManager shaderManager;

// 批次容器
GLBatch triangleBatch;

// 边长
GLfloat blockSize = 0.1f;
// 正方形4个点坐标
GLfloat vVerts[] = {
    -blockSize,-blockSize,0.0f,
    blockSize,-blockSize,0.0f,
    blockSize,blockSize,0.0f,
    -blockSize,blockSize,0.0f
};
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
//    GLfloat vVerts[] = {
//        -0.5f,0.0f,0.0f,
//        0.5f,0,0,
//        0,0.5,0
//    };
    triangleBatch.Begin(GL_TRIANGLE_FAN, 4);
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
    
}

void SpecialKeys(int key, int x, int y){
    
    GLfloat stepSize = 0.025f;
    
    GLfloat blockX = vVerts[0];
    GLfloat blockY = vVerts[10];
    
    printf("v[0] = %f\n",blockX);
    printf("v[10] = %f\n",blockY);
    
    
    if (key == GLUT_KEY_UP) {
        
        blockY += stepSize;
    }
    
    if (key == GLUT_KEY_DOWN) {
        
        blockY -= stepSize;
    }
    
    if (key == GLUT_KEY_LEFT) {
        blockX -= stepSize;
    }
    
    if (key == GLUT_KEY_RIGHT) {
        blockX += stepSize;
    }
    
    //触碰到边界（4个边界）的处理
    
    //当正方形移动超过最左边的时候
    if (blockX < -1.0f) {
        blockX = -1.0f;
    }
    
    //当正方形移动到最右边时
    //1.0 - blockSize * 2 = 总边长 - 正方形的边长 = 最左边点的位置
    if (blockX > (1.0 - blockSize * 2)) {
        blockX = 1.0f - blockSize * 2;
    }
    
    //当正方形移动到最下面时
    //-1.0 - blockSize * 2 = Y（负轴边界） - 正方形边长 = 最下面点的位置
    if (blockY < -1.0f + blockSize * 2 ) {
        
        blockY = -1.0f + blockSize * 2;
    }
    
    //当正方形移动到最上面时
    if (blockY > 1.0f) {
        
        blockY = 1.0f;
        
    }
    
    printf("blockX = %f\n",blockX);
    printf("blockY = %f\n",blockY);
    
    // Recalculate vertex positions
    vVerts[0] = blockX;
    vVerts[1] = blockY - blockSize*2;
    
    printf("(%f,%f)\n",vVerts[0],vVerts[1]);
    
    
    vVerts[3] = blockX + blockSize*2;
    vVerts[4] = blockY - blockSize*2;
    printf("(%f,%f)\n",vVerts[3],vVerts[4]);
    
    vVerts[6] = blockX + blockSize*2;
    vVerts[7] = blockY;
    printf("(%f,%f)\n",vVerts[6],vVerts[7]);
    
    vVerts[9] = blockX;
    vVerts[10] = blockY;
    printf("(%f,%f)\n",vVerts[9],vVerts[10]);
    
    triangleBatch.CopyVertexData3f(vVerts);
    
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
    glutSpecialFunc(SpecialKeys);
    
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

