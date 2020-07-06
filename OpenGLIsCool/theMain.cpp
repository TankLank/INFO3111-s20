#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#include "cShaderManager.h"

//fileio
#include <fstream>
#include <string>

struct sVertex
{
    float x, y, z;      // NEW! With Zs
    float r, g, b;
};

sVertex vertices[6] =
{
    { -0.6f, -0.4f, 0.0f /*z*/, 1.0f, 0.0f, 0.0f },         // 0
    {  0.6f, -0.4f, 0.0f /*z*/, 0.0f, 1.0f, 0.0f },         // 1 
    {  0.0f,  0.6f, 0.0f /*z*/, 0.0f, 0.0f, 1.0f },         // 2
    { -0.6f,  0.4f, 0.0f /*z*/, 1.0f, 0.0f, 0.0f },         // 3
    {  0.6f,  0.4f, 0.0f /*z*/, 0.0f, 1.0f, 0.0f },
    {  0.0f,  1.6f, 0.0f /*z*/, 0.0f, 0.0f, 1.0f }          // 5
};


// Yes, it's global. Just be calm, for now.
cShaderManager* g_pShaderManager = 0;       // NULL


//static const char* vertex_shader_text =
//"#version 110\n"
//"uniform mat4 MVP;\n"
//"attribute vec3 vCol;\n"        
//"attribute vec3 vPos;\n"        
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_Position = MVP * vec4(vPos, 1.0);\n"
//"    color = vCol;\n"
//"}\n";

//static const char* fragment_shader_text =
//"#version 110\n"
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_FragColor = vec4(color, 1.0);\n"
//"}\n";

void DoTheFileStuff() {
    std::ifstream theFile("assets/models/bun_zipper_res4_xyz_colour.ply");

    if (!theFile.is_open()) {
        std::cout << "Oh no!" << std::endl;
        return;
    }

    //look for the word "Vertex"
    std::string temp;
    while (true) 
    {
        theFile >> temp;

        if (temp == "vertex")
        {
            break;
        }
    }//while

    int numVerts = 0;
    theFile >> numVerts;
    std::cout << "Number of vertices: " << numVerts << std::endl;

    while (true)
    {
        theFile >> temp;

        if (temp == "face")
        {
            break;
        }
    }//while

    int numTriangles = 0;
    theFile >> numTriangles;
    std::cout << "Number of faces: " << numTriangles << std::endl;

    while (true)
    {
        theFile >> temp;

        if (temp == "end_header")
        {
            break;
        }
    }//while

    float hr, hg, hb, lr, lg, lb = 0;

    for (int index = 0; index != numVerts; index++)
    {
        float x, y, z, r, g, b, a;
        theFile >> x >> y >> z >> r >> g >> b >> a;

        if (index == 0) {
            hr = r / 255.0f;
            hg = g / 255.0f;
            hb = b / 255.0f;
            lr = r / 255.0f;
            lg = g / 255.0f;
            lb = b / 255.0f;
        }
        
        if (hr < (r / 255.0f) && (hg < (g / 255.0f)) && (hb < (b / 255.0f)))
        {
            hr = r / 255.0f;
            hg = g / 255.0f;
            hb = b / 255.0f;
        }
        if (lr > (r / 255.0f) && (lg > (g / 255.0f)) && (lb > (b / 255.0f)))
        {
            lr = r / 255.0f;
            lg = g / 255.0f;
            lb = b / 255.0f;
        }


       // std::cout << "g = " << g / 255.0f << std::endl;
    }

    std::cout << "hr = " << hr << std::endl;
    std::cout << "hg = " << hg << std::endl;
    std::cout << "hb = " << hb << std::endl;
    std::cout << "lr = " << lr << std::endl;
    std::cout << "lg = " << lg << std::endl;
    std::cout << "lb = " << lb << std::endl;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
    DoTheFileStuff();

    std::cout << "About to start..." << std::endl;

    GLFWwindow* window;
    GLuint vertex_buffer;
    GLuint vertex_shader; 
    GLuint fragment_shader;
    GLuint program;

    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        std::cout << "Can't init GLFW. Exiting" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    std::cout << "About to create window..." << std::endl;
    window = glfwCreateWindow(1024, 700, "Long Live COBOL", NULL, NULL);
    if (!window)
    {
        // Can't init openGL. Oh no. 
        std::cout << "ERROR: Can't create window." << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //cShaderManager* g_pShaderManager = 0;
    ::g_pShaderManager = new cShaderManager();          // HEAP  

    cShaderManager::cShader vertShader;
    cShaderManager::cShader fragShader;

    vertShader.fileName = "simpleVertex.glsl";
    fragShader.fileName = "simpleFragment.glsl";

    if ( ! ::g_pShaderManager->createProgramFromFile( "SimpleShaderProg",
                                                      vertShader, fragShader) )
    {
        // There was problem. 
        std::cout << "ERROR: can't make shader program because: " << std::endl;
        std::cout << ::g_pShaderManager->getLastError() << std::endl;
        
        // Exit program (maybe add some cleanup code)
        return -1;
    }//createProgramFromFile

    //vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    //glCompileShader(vertex_shader);
//
    //fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    //glCompileShader(fragment_shader);
//
    //program = glCreateProgram();
    //glAttachShader(program, vertex_shader);
    //glAttachShader(program, fragment_shader);
    //glLinkProgram(program);

    program = ::g_pShaderManager->getIDFromFriendlyName("SimpleShaderProg");

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*)0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*)(sizeof(float) * 2));

    std::cout << "We're all set! Buckle up!" << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        //       mat4x4 m, p, mvp;
        glm::mat4 m, p, v, mvp;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        //         mat4x4_identity(m);
        m = glm::mat4(1.0f);

        //mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
                                        (float)glfwGetTime(),
                                        glm::vec3(0.0f, 0.0, 1.0f));

       m = m * rotateZ;

        //mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        p = glm::perspective(0.6f,
                             ratio,
                             0.1f,
                             1000.0f);

        v = glm::mat4(1.0f);

        glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        v = glm::lookAt(cameraEye,
                        cameraTarget,
                        upVector);

        //mat4x4_mul(mvp, p, m);
        mvp = p * v * m;


        glUseProgram(program);


        //glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    delete ::g_pShaderManager;


    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
