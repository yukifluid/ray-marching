#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>
#include <string>
#include <memory>

#include "shader.hpp"

// OpenGL version
const int opengl_version_mejor = 3;
const int opengl_version_minor = 3;

// window settings
const char* window_title = "ray-marching";
const int window_width   = 800;
const int window_height  = 800;
const glm::vec4 background_color = glm::vec4(0.2, 0.2, 0.2, 1.0);

void SaveImage(int windowWidth, int windowHeight, std::string imgName);

int main(int argc, char** argv)
{
    if(argc != 2) {
        std::cerr << "Specify fragment shader" << std::endl;
        exit(1);
    }

    // initialize OpenGL
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_version_mejor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_version_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    atexit(glfwTerminate);

    // create window
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);

    if(window == NULL)
    {
        std::cerr << "Falied to create GLFW window" << std::endl;
        exit(1);
    }

    glfwMakeContextCurrent(window);

    // initialize GLEW
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        std::cerr << "Falied to initialize GLEW" << std::endl;
        exit(1);
    }

    glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
    glViewport(0, 0, 2*window_width, 2*window_height); // times 2 for retina display

    float vertices[] = {
         1.0,  1.0, 0.0,
        -1.0,  1.0, 0.0,
        -1.0, -1.0, 0.0,
         1.0, -1.0, 0.0
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int vao;
    unsigned int vbo;
    unsigned int ibo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)(0*sizeof(float)));
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW);

    std::unique_ptr<Shader> shader = std::make_unique<Shader>("./shaders/vertex.glsl", "./shaders/" + std::string(argv[1]) + ".glsl");

    // glfwSetTime(0);
    // while(!glfwWindowShouldClose(window))
    // {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->Use();
    shader->SetFloat("time", glfwGetTime());
    shader->SetVec2("resolution", glm::vec2(2.0*window_width, 2.0*window_height));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);

    SaveImage(window_width, window_height, "./result/" + std::string(argv[1]) + ".jpg");

    // }
}

void SaveImage(int windowWidth, int windowHeight, std::string imgName)
{
    const int imageWidth    = windowWidth*2; // twice the size for Mac Retina display
    const int imageHeight   = windowHeight*2; // twice the size for Mac Retina display
    const unsigned int channelNum = 4; // RGBなら3, RGBAなら4
    unsigned char* dataBuffer = NULL;
    dataBuffer = (unsigned char*)malloc(imageWidth * imageHeight * channelNum);

    // 読み取るOpneGLのバッファを指定 GL_FRONT:フロントバッファ　GL_BACK:バックバッファ
    glReadBuffer(GL_FRONT);

    // OpenGLで画面に描画されている内容をバッファに格納
    glReadPixels(
        0,                  //読み取る領域の左下隅のx座標
        0,                  //読み取る領域の左下隅のy座標 //0 or getCurrentWidth() - 1
        imageWidth,         //読み取る領域の幅
        imageHeight,        //読み取る領域の高さ
        GL_BGRA,            //取得したい色情報の形式
        GL_UNSIGNED_BYTE,   //読み取ったデータを保存する配列の型
        dataBuffer          //ビットマップのピクセルデータ（実際にはバイト配列）へのポインタ
    );

    GLubyte* p = static_cast<GLubyte*>(dataBuffer);
    cv::Mat data(imageWidth, imageHeight, CV_8UC4, p);
    cv::flip(data, data, 0);
    cv::imwrite(imgName, data);
}