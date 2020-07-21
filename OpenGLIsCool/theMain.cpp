//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include "globalOpenGLStuff.h"
#include "globalThings.h"

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
#include <fstream>
#include <sstream>
#include "cShaderManager.h"
#include "cVAOManager.h"
#include <string>

#include "cMeshObject.h"
#include <vector>
#include "LightManager/cLightManager.h"

// Function signature for DrawObject()
void DrawObject(cMeshObject* pCurMesh,
    GLuint program,             
    glm::mat4& matView,         
    glm::mat4& matProjection);

// Camera stuff
//glm::vec3 g_cameraEye = glm::vec3(0.0, 0.0, +100.0f);
glm::vec3 g_cameraEye = glm::vec3(-37.0f, -27.0f, +89.0f);
glm::vec3 g_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_upVector = glm::vec3(0.0f, 1.0f, 0.0f);

// The objects we are drawing go in here! Hazzah!
std::vector< cMeshObject* > g_pVecObjects;

int selectedObject = 0;
int selectedLight = 0;

cLightManager* g_pTheLightManager = 0;

// this is a light structure to match what's in the shader
//struct sLight
//{
//    sLight() 
//    {
//        this->position.x = 50.0f;
//        this->position.y = 100.0f;
//        this->position.z = 100.0f;
//        this->position.w = 1.0f;
//        this->atten.y = 0.01f;
//
//        this->diffuse.r = 1.0f;
//        this->diffuse.g = 1.0f;
//        this->diffuse.b = 1.0f;
//    }
//    glm::vec4 position;
//    glm::vec4 diffuse;
//    glm::vec4 specular;	// rgb = highlight colour, w = power
//    glm::vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
//    glm::vec4 direction;	// Spot, directional lights
//    glm::vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
//                    // 0 = pointlight
//                    // 1 = spot light
//                    // 2 = directional light
//    glm::vec4 param2;	// x = 0 for off, 1 for on
//};

//sLight* g_theLights = 0;

//struct sVertex
//{
//    float x, y, z;      // NEW! With Zs
//    float r, g, b;
//};
//
//int g_numberOfVerts = 0;
//sVertex* g_pVertexBuffer = 0;     // or NULL or nullptr



//sVertex myArray[100];                       // STACK  
//sVertex* pMyArray = new sVertex[100];       // HEAP 
//delete [] pMyArray;
//
//sVertex vertices[6] =
//{
//    { -0.6f, -0.4f, 0.0f /*z*/, 1.0f, 0.0f, 0.0f },         // 0
//    {  0.6f, -0.4f, 0.0f /*z*/, 0.0f, 1.0f, 0.0f },         // 1 
//    {  0.0f,  0.6f, 0.0f /*z*/, 0.0f, 0.0f, 1.0f },         // 2
//    { -0.6f,  0.4f, 0.0f /*z*/, 1.0f, 0.0f, 0.0f },         // 3
//    {  0.6f,  0.4f, 0.0f /*z*/, 0.0f, 1.0f, 0.0f },
//    {  0.0f,  1.6f, 0.0f /*z*/, 0.0f, 0.0f, 1.0f }          // 5
//};


// Yes, it's global. Just be calm, for now.
cShaderManager* g_pShaderManager = 0;       // NULL
cVAOManager* g_pTheVAOManager = 0;          // NULL or nullptr

bool g_isWireFrame = false;


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
//
//static const char* fragment_shader_text =
//"#version 110\n"
//"varying vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_FragColor = vec4(color, 1.0);\n"
//"}\n";

//void LoadPlyFile( std::string fileName )
//{
//    std::ifstream theFile( fileName.c_str() );
//
//    if (!theFile.is_open())
//    {
//        std::cout << "Oh NO!!" << std::endl;
//        return;
//    }
//
//    // Look for the work "vertex"
//    std::string temp;
//
//    bool bKeepReading = true;
//
//    while (bKeepReading)
//    {
//        theFile >> temp;    // Read next string
//        if (temp == "vertex")
//        {
//            bKeepReading = false;
//        }
//    }//while
//
////    int numberOfVerts = 0;
//    theFile >> ::g_numberOfVerts;
//    std::cout << ::g_numberOfVerts << std::endl;
//
//    // Make the vertex buffer the size we need
//    // Allocate "this" number of vertices
//    ::g_pVertexBuffer = new sVertex[::g_numberOfVerts];
//
//    while (true)
//    {
//        theFile >> temp;    // Read next string
//        if (temp == "face")
//        {
//            break;
//        }
//    }//while
//
//    int numberOfTriangles = 0;
//    theFile >> numberOfTriangles;
//    std::cout << numberOfTriangles << std::endl;
//
//
//    while (true)
//    {
//        theFile >> temp;    // Read next string
//        if (temp == "end_header")
//        {
//            break;
//        }
//    }//while
//
//    for (int index = 0; index != ::g_numberOfVerts; index++)
//    {
//        float x,y,z,r,g,b,a;
//        theFile >> x >> y >> z >> r >> g >> b >> a;
//
//        ::g_pVertexBuffer[index].x = x;
//        ::g_pVertexBuffer[index].y = y;
//        ::g_pVertexBuffer[index].z = z;
//        ::g_pVertexBuffer[index].r = r / 255.0f;
//        ::g_pVertexBuffer[index].g = g / 255.0f;
//        ::g_pVertexBuffer[index].b = b / 255.0f;
//    }
//
//    return;
//}

cMeshObject* findObjectByName(std::string friendlyNameToFind)
{
    //Search by name until we find the object
    for (std::vector<cMeshObject*>::iterator it_pCO = ::g_pVecObjects.begin();
        it_pCO != ::g_pVecObjects.end(); it_pCO++)
    {
        // Is this the one?
        if ((*it_pCO)->friendlyName == friendlyNameToFind)
        {
            // Found it!
            return (*it_pCO);
        }
    }
    //Didn't find it, so return 0 or NULL;
    return NULL;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    const float CAMERASPEED = 1.0f;
    // WSAD - AD are "right and left"
    //      - SW are "forward and back"
    //      - QE are "up and down"

    if (key == GLFW_KEY_O)
    {
        ::g_pVecObjects[selectedObject]->scale *= 1.5f;
    }
    if (key == GLFW_KEY_P)
    {
        ::g_pVecObjects[selectedObject]->scale /= 1.5f;
    }

    // camera + light controls -----------------------------------------
    if (key == GLFW_KEY_A && (mods & GLFW_MOD_SHIFT)) // go "left" (+ve x) light
    {
        ::g_pTheLightManager->vecLights[selectedLight].position.x -= CAMERASPEED;
    }
    else if (key == GLFW_KEY_A) // go "left" camera
    {
        ::g_cameraEye.x -= CAMERASPEED;
    }

    if (key == GLFW_KEY_D && (mods & GLFW_MOD_SHIFT)) // go "right"
    {
        ::g_pTheLightManager->vecLights[selectedLight].position.x += CAMERASPEED;
    }
    else if (key == GLFW_KEY_D) // go "right"
    {
        ::g_cameraEye.x += CAMERASPEED;
    }

    if (key == GLFW_KEY_S && (mods & GLFW_MOD_SHIFT)) // go "back" (-ve y) light
    {
        ::g_pTheLightManager->vecLights[selectedLight].position.z -= CAMERASPEED;
    }
    else if (key == GLFW_KEY_S) // go "back" (-ve Z) camera
    {
        ::g_cameraEye.z -= CAMERASPEED;
    }

    if (key == GLFW_KEY_W && (mods & GLFW_MOD_SHIFT)) // go "forward" (+ve y) light
    {
        ::g_pTheLightManager->vecLights[selectedLight].position.z += CAMERASPEED;
    }
    else if (key == GLFW_KEY_W) // go "forward" (+ve Z) camera
    {
        ::g_cameraEye.z += CAMERASPEED;
    }

    if (key == GLFW_KEY_Q && (mods & GLFW_MOD_SHIFT)) //go "down" (-ve y) light
    {
        ::g_pTheLightManager->vecLights[selectedLight].position.y -= CAMERASPEED;
    }
    else if (key == GLFW_KEY_Q) // go "down" (-ve Y)
    {
        ::g_cameraEye.y -= CAMERASPEED;
    }

    if (key == GLFW_KEY_E && (mods & GLFW_MOD_SHIFT)) //go "up" (-ve y) light
    {
        ::g_pTheLightManager->vecLights[selectedLight].position.y += CAMERASPEED;
    }
    else if (key == GLFW_KEY_E) // go "up" (-ve Y)
    {
        ::g_cameraEye.y += CAMERASPEED;
    }

    // end camera + light control ==================================

    //attenuation and color of light -------------------------------
    if (key == GLFW_KEY_EQUAL)
    {
        ::g_pTheLightManager->vecLights[selectedLight].atten.y *= 1.01f;
    }
    if (key == GLFW_KEY_MINUS)
    {
        ::g_pTheLightManager->vecLights[selectedLight].atten.y *= 0.99f;
    }
    if (key == GLFW_KEY_1 && (mods & GLFW_MOD_SHIFT))
    {
        if (::g_pTheLightManager->vecLights[selectedLight].diffuse.r < 1.0f)
            ::g_pTheLightManager->vecLights[selectedLight].diffuse.r += 0.01f;
    }
    else if (key == GLFW_KEY_1)
    {
        if (::g_pTheLightManager->vecLights[selectedLight].diffuse.r > 0.0f)
            ::g_pTheLightManager->vecLights[selectedLight].diffuse.r -= 0.01f;
    }
    if (key == GLFW_KEY_2 && (mods & GLFW_MOD_SHIFT))
    {
        if (::g_pTheLightManager->vecLights[selectedLight].diffuse.g < 1.0f)
            ::g_pTheLightManager->vecLights[selectedLight].diffuse.g += 0.01f;
    }
    else if (key == GLFW_KEY_2)
    {
        if (::g_pTheLightManager->vecLights[selectedLight].diffuse.g > 0.0f)
            ::g_pTheLightManager->vecLights[selectedLight].diffuse.g -= 0.01f;
    }
    if (key == GLFW_KEY_3 && (mods & GLFW_MOD_SHIFT))
    {
        if (::g_pTheLightManager->vecLights[selectedLight].diffuse.b < 1.0f)
            ::g_pTheLightManager->vecLights[selectedLight].diffuse.b += 0.01f;
    }
    else if (key == GLFW_KEY_3)
    {
        if (::g_pTheLightManager->vecLights[selectedLight].diffuse.b > 0.0f)
            ::g_pTheLightManager->vecLights[selectedLight].diffuse.b -= 0.01f;
    }

    //-------------------------------------------------------------

    //SELECTED MODEL ----------------------------------------------
    if (key == GLFW_KEY_LEFT) // go "left"
    {
        ::g_pVecObjects[selectedObject]->position.x -= CAMERASPEED;
    }

    if (key == GLFW_KEY_RIGHT) // go "right"
    {
        ::g_pVecObjects[selectedObject]->position.x += CAMERASPEED;
    }

    if (key == GLFW_KEY_RIGHT_CONTROL) // go "back" (-ve Z)
    {
        ::g_pVecObjects[selectedObject]->position.z -= CAMERASPEED;
    }

    if (key == GLFW_KEY_RIGHT_SHIFT) // go "forward" (+ve Z)
    {
        ::g_pVecObjects[selectedObject]->position.z += CAMERASPEED;
    }

    if (key == GLFW_KEY_DOWN) // go "down" (-ve Y)
    {
        ::g_pVecObjects[selectedObject]->position.y -= CAMERASPEED;
    }

    if (key == GLFW_KEY_UP) // go "up" (-ve Y)
    {
        ::g_pVecObjects[selectedObject]->position.y += CAMERASPEED;
    }

    if (key == GLFW_KEY_L && (mods & GLFW_MOD_SHIFT))
    {
        ::g_pVecObjects[selectedObject]->orientation.z += glm::radians(CAMERASPEED);
    }
    else if (key == GLFW_KEY_L && (mods & GLFW_MOD_CONTROL))
    {
        ::g_pVecObjects[selectedObject]->orientation.x += glm::radians(CAMERASPEED);
    }
    else if (key == GLFW_KEY_L)
    {
        ::g_pVecObjects[selectedObject]->orientation.y += glm::radians(CAMERASPEED);
    }

    if (key == GLFW_KEY_K &&(mods & GLFW_MOD_SHIFT))
    {
        ::g_pVecObjects[selectedObject]->orientation.z -= glm::radians(CAMERASPEED);
    }
    else if (key == GLFW_KEY_K && (mods & GLFW_MOD_CONTROL))
    {
        ::g_pVecObjects[selectedObject]->orientation.x -= glm::radians(CAMERASPEED);
    }
    else if (key == GLFW_KEY_K)
    {
        ::g_pVecObjects[selectedObject]->orientation.y -= glm::radians(CAMERASPEED);
    }
    
    // Model/light selection
    if (key == GLFW_KEY_U && action == GLFW_PRESS && (mods & GLFW_MOD_SHIFT)) {
        selectedLight++;
        if (selectedLight == ::g_pTheLightManager->vecLights.size())
        {
            selectedLight = 0;
        }
    }
    else if (key == GLFW_KEY_U && action == GLFW_PRESS)
    {
        selectedObject++;
        if (selectedObject == ::g_pVecObjects.size())
        {
            selectedObject = 0;
        }
    }

    if (key == GLFW_KEY_I && action == GLFW_PRESS && (mods & GLFW_MOD_SHIFT)) {
        selectedLight--;
        if (selectedLight < 0)
        {
            selectedLight = (::g_pTheLightManager->vecLights.size() - 1);
        }
    }
    else if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {
        selectedObject--;
        if (selectedObject < 0)
        {
            selectedObject = (::g_pVecObjects.size() - 1);
        }
    }

    // endof selected object ================================

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // Switches from wireframe to solid (glPolygonMode)
    if (key == GLFW_KEY_9 && action == GLFW_PRESS ) { ::g_pVecObjects[selectedObject]->isWireframe = true; }
    if (key == GLFW_KEY_0 && action == GLFW_PRESS ) { ::g_pVecObjects[selectedObject]->isWireframe = false; }


    //saves the vector info
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        std::string fileName = "assets/models/model.txt";
        std::ofstream thePlyFile(fileName.c_str());
        if (!thePlyFile.is_open())
        {	// Something is wrong...
            std::cout << "Can't open >" << fileName << "< file." << std::endl;
        }

        for (std::vector<cMeshObject*>::iterator it_pCurMesh = ::g_pVecObjects.begin();
            it_pCurMesh != ::g_pVecObjects.end(); it_pCurMesh++)
        {
            cMeshObject* pCurMesh = *it_pCurMesh;
            thePlyFile << " ";
            thePlyFile << pCurMesh->meshName;
            thePlyFile << " ";
            thePlyFile << pCurMesh->position.x;
            thePlyFile << " ";
            thePlyFile << pCurMesh->position.y;
            thePlyFile << " ";
            thePlyFile << pCurMesh->position.z;
            thePlyFile << " ";
            thePlyFile << pCurMesh->position.w;
            thePlyFile << " ";
            thePlyFile << pCurMesh->normal.x;
            thePlyFile << " ";
            thePlyFile << pCurMesh->normal.y;
            thePlyFile << " ";
            thePlyFile << pCurMesh->normal.z;
            thePlyFile << " ";
            thePlyFile << pCurMesh->normal.w;
            thePlyFile << " ";
            thePlyFile << pCurMesh->colourRGBA.r;
            thePlyFile << " ";
            thePlyFile << pCurMesh->colourRGBA.g;
            thePlyFile << " ";
            thePlyFile << pCurMesh->colourRGBA.b;
            thePlyFile << " ";
            thePlyFile << pCurMesh->colourRGBA.a;
            thePlyFile << " ";
            thePlyFile << pCurMesh->textUV.x;
            thePlyFile << " ";
            thePlyFile << pCurMesh->textUV.y;
            thePlyFile << " ";
            thePlyFile << pCurMesh->orientation.x;
            thePlyFile << " ";
            thePlyFile << pCurMesh->orientation.y;
            thePlyFile << " ";
            thePlyFile << pCurMesh->orientation.z;
            thePlyFile << " ";
            thePlyFile << pCurMesh->scale;
            thePlyFile << " ";
            thePlyFile << pCurMesh->isWireframe;
            thePlyFile << " ";
            thePlyFile << pCurMesh->isVisible;
            thePlyFile << " ";
            thePlyFile << pCurMesh->friendlyName;
            thePlyFile << std::endl;
        }
        std::cout << "File written successfully to " << fileName << std::endl;
        thePlyFile.close();
    }

    //loads the vector info
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        ::g_pVecObjects.clear();

        std::string fileName = "assets/models/model.txt";
        std::ifstream thePlyFile(fileName.c_str());
        if (!thePlyFile.is_open())
        {	// Something is wrong...
            std::stringstream ssError;
            ssError << "Can't open >" << fileName << "< file." << std::endl;
        }

        while (true)
        {
            std::string temp;
            if (thePlyFile >> temp)
            {
                //creates a cMeshObject and loads it ino the vector
                cMeshObject* newMesh = new cMeshObject();
                newMesh->meshName = temp;
                thePlyFile >> newMesh->position.x;
                thePlyFile >> newMesh->position.y;
                thePlyFile >> newMesh->position.z;
                thePlyFile >> newMesh->position.w;
                thePlyFile >> newMesh->normal.x;
                thePlyFile >> newMesh->normal.y;
                thePlyFile >> newMesh->normal.z;
                thePlyFile >> newMesh->normal.w;
                thePlyFile >> newMesh->colourRGBA.r;
                thePlyFile >> newMesh->colourRGBA.g;
                thePlyFile >> newMesh->colourRGBA.b;
                thePlyFile >> newMesh->colourRGBA.a;
                thePlyFile >> newMesh->textUV.x;
                thePlyFile >> newMesh->textUV.y;
                thePlyFile >> newMesh->orientation.x;
                thePlyFile >> newMesh->orientation.y;
                thePlyFile >> newMesh->orientation.z;
                thePlyFile >> newMesh->scale;
                thePlyFile >> newMesh->isWireframe;
                thePlyFile >> newMesh->isVisible;
                thePlyFile >> newMesh->friendlyName;

                ::g_pVecObjects.push_back(newMesh);
            }
            else
            {
                break;
            }
        }
        std::cout << "File loaded successfully from " << fileName << std::endl;
        thePlyFile.close();
    }

    // Print out camera location:
    std::cout << "cam: "
        << ::g_cameraEye.x << ", "
        << ::g_cameraEye.y << ", "
        << ::g_cameraEye.z << std::endl;

    // Print out light location:
    //glfwSetWindowTitle(window, ::g_theLights->position.x); //doesn't exist?
    std::cout << "light: "
        << ::g_pTheLightManager->vecLights[selectedLight].position.x << ", "
        << ::g_pTheLightManager->vecLights[selectedLight].position.y << ", "
        << ::g_pTheLightManager->vecLights[selectedLight].position.z << std::endl;

    return;
}



int main(void)
{

 //   LoadPlyFile("assets/models/bun_zipper_res4_xyz_colour.ply");

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
    window = glfwCreateWindow(1024, 700, "Checkpoint #6", NULL, NULL);
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

//    // NOTE: OpenGL error checks have been omitted for brevity
//    glGenBuffers(1, &vertex_buffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//
//    //sVertex vertices[6]
//    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    unsigned int sizeOfVertBufferInBytes = sizeof(sVertex) * ::g_numberOfVerts;
//
//    glBufferData( GL_ARRAY_BUFFER, 
//                  sizeOfVertBufferInBytes,      // Size in bytes
//                  ::g_pVertexBuffer,            // Pointer to START of local array
//                  GL_STATIC_DRAW);

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

//    glEnableVertexAttribArray(vpos_location);
//    glVertexAttribPointer(vpos_location, 
//                          3, GL_FLOAT, 
//                          GL_FALSE,
//                          sizeof(sVertex),      // "stride"
//                          (void*)offsetof(sVertex, x) );
//    //struct sVertex
    //{
    //    float x, y, z;      // NEW! With Zs
    //    float r, g, b;
    //};
//
//    glEnableVertexAttribArray(vcol_location);
//    glVertexAttribPointer(vcol_location, 
//                          3, GL_FLOAT, 
//                          GL_FALSE,
//                          sizeof(sVertex),               // "stride"
//                          (void*)offsetof(sVertex, r) ); // "offset" (how many bytes 'in' is this member?)

    // STARTOF: Loading the models
    ::g_pTheVAOManager = new cVAOManager();

    sModelDrawInfo mdiArena;
    if ( ! ::g_pTheVAOManager->LoadModelIntoVAO( "assets/models/free_arena_ASCII_xyz_n_rgba_uv.ply",
                                                 mdiArena, program) )
    {
        std::cout << "Error: " << ::g_pTheVAOManager->getLastError() << std::endl;
    }
    
    // Load the models
    LoadAllTheModels(program, ::g_pTheVAOManager);


    g_pTheLightManager = new cLightManager();

    ::g_pTheLightManager->LoadUniformLocations(program);

    ::g_pTheLightManager->vecLights[0].position = glm::vec4(-59.0f, 71.0f, 129.0f, 1.0f);
    ::g_pTheLightManager->vecLights[0].param1.x = 0;        //point light
    ::g_pTheLightManager->vecLights[0].atten.x = 0.0f;      //constant
    ::g_pTheLightManager->vecLights[0].atten.y = 0.001f;     //linear
    ::g_pTheLightManager->vecLights[0].atten.z = 0.0001f;     //quadratic
    ::g_pTheLightManager->vecLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pTheLightManager->vecLights[0].specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pTheLightManager->vecLights[0].param2.x = 1.0f;

    ::g_pTheLightManager->vecLights[1].position = glm::vec4(39.0f, 151.0f, 131.0f, 1.0f);
    ::g_pTheLightManager->vecLights[1].param1.x = 0;        //point light
    ::g_pTheLightManager->vecLights[1].atten.x = 0.0f;      //constant
    ::g_pTheLightManager->vecLights[1].atten.y = 0.001f;     //linear
    ::g_pTheLightManager->vecLights[1].atten.z = 0.0001f;     //quadratic
    ::g_pTheLightManager->vecLights[1].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pTheLightManager->vecLights[1].specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pTheLightManager->vecLights[1].param2.x = 1.0f;

    //Get the locations for the "uniform variables"
    // uniform vec4 objectColour;

    // get the uniform locations for the lights
    /*GLint theLights_0_position_LocID = glGetUniformLocation(program, "theLights[0].position");
    GLint theLights_0_diffuse_LocID = glGetUniformLocation(program, "theLights[0].diffuse");
    GLint theLights_0_specular_LocID = glGetUniformLocation(program, "theLights[0].specular");
    GLint theLights_0_atten_LocID = glGetUniformLocation(program, "theLights[0].atten");
    GLint theLights_0_direction_LocID = glGetUniformLocation(program, "theLights[0].direction");
    GLint theLights_0_param1_LocID = glGetUniformLocation(program, "theLights[0].param1");
    GLint theLights_0_param2_LocID = glGetUniformLocation(program, "theLights[0].param2");*/

    //create theLights variable
   // ::g_theLights = new sLight();

    std::cout << "We're all set! Buckle up!" << std::endl;

    while ( ! glfwWindowShouldClose(window) )
    {
        float ratio;
        int width, height;
        //       mat4x4 m, p, mvp;
 //       glm::mat4 matModel;       //moved to DrawObject function
        glm::mat4 matProjection;
        glm::mat4 matView;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_BACK);

       

        //mat4x4_rotate_Z(m, m, (float) glfwGetTime());
//        glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
//                                        (float)glfwGetTime(),
//                                        glm::vec3(0.0f, 0.0, 1.0f));
//
//       m = m * rotateZ;

        //mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        matProjection = glm::perspective( 0.6f,
                                          ratio,
                                          0.1f,          // Near plane
                                          10000.0f);      // Far plane

        matView = glm::mat4(1.0f);

        //glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
        //glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        //glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

         matView = glm::lookAt( ::g_cameraEye,     // "position" "eye"
                         ::g_cameraTarget,  // "at"  (looking "at")
                         ::g_upVector );    

         ::g_pTheLightManager->CopyLightValuesToShader();
         

         // set the lighting for the scene
         //glUniform4f(theLights_0_position_LocID, ::g_theLights->position.x, ::g_theLights->position.y, ::g_theLights->position.z, ::g_theLights->position.w); //theLights[0].position
         //glUniform4f(theLights_0_diffuse_LocID, ::g_theLights->diffuse.r, ::g_theLights->diffuse.g, ::g_theLights->diffuse.b, 1.0f);     //theLights[0].diffuse
         //glUniform4f(theLights_0_specular_LocID, 1.0f, 1.0f, 1.0f, 1.0f);
         //glUniform4f(theLights_0_atten_LocID, 0.0f, ::g_theLights->atten.y, 0.0f, 1.0f); //x = dim, y = atten (how much light drops off), z = quadratic atten
         //glUniform4f(theLights_0_direction_LocID, 0.0f, 0.0f, 0.0f, 0.0f);
         //glUniform4f(theLights_0_param1_LocID, 0.0f, 0.0f, 0.0f, 0.0f);
         //glUniform4f(theLights_0_param2_LocID, 1.0f, 0.0f, 0.0f, 1.0f); //theLights[0].param2       x = 1.0f for on, 0.0f for off

        // *******************************************
        // **** Draw the entire scene of objects *****
        // *******************************************
        for ( std::vector< cMeshObject* >::iterator it_pCurMesh = ::g_pVecObjects.begin();
              it_pCurMesh != ::g_pVecObjects.end(); it_pCurMesh++ )
        {
            cMeshObject* pTheObject = *it_pCurMesh;

            DrawObject(pTheObject, program, matView, matProjection);

        }//for ( std::vector< cMeshObject* >
        // ****************************
        // **** END OF: Draw scene ****
        // ****************************

        cMeshObject* pDebugBall = findObjectByName("DebugSphere");
        pDebugBall->isVisible = true;
        pDebugBall->position = ::g_pTheLightManager->vecLights[selectedLight].position;
        DrawObject(pDebugBall, program, matView, matProjection);
        //pDebugBall->isVisible = false;

        cLightHelper myHelper;
        float dist5 = myHelper.calcApproxDistFromAtten(0.05f, 0.01f, 1000000,
                                                            g_pTheLightManager->vecLights[0].atten.x,
                                                            g_pTheLightManager->vecLights[0].atten.y,
                                                            g_pTheLightManager->vecLights[0].atten.z);
        float dist25 = myHelper.calcApproxDistFromAtten(0.25f, 0.01f, 1000000, 
                                                            g_pTheLightManager->vecLights[0].atten.x,
                                                            g_pTheLightManager->vecLights[0].atten.y,
                                                            g_pTheLightManager->vecLights[0].atten.z);
        float dist50 = myHelper.calcApproxDistFromAtten(0.50f, 0.01f, 1000000,
            g_pTheLightManager->vecLights[0].atten.x,
            g_pTheLightManager->vecLights[0].atten.y,
            g_pTheLightManager->vecLights[0].atten.z);
        float dist75 = myHelper.calcApproxDistFromAtten(0.75f, 0.01f, 1000000,
            g_pTheLightManager->vecLights[0].atten.x,
            g_pTheLightManager->vecLights[0].atten.y,
            g_pTheLightManager->vecLights[0].atten.z);

        //// Draw sphere to match the brightness of the light
        //pDebugBall->isVisible = true;
        //pDebugBall->colourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        //pDebugBall->scale = dist5;
        //DrawObject(pDebugBall, program, matView, matProjection);

        //pDebugBall->colourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        //pDebugBall->scale = dist25;
        //DrawObject(pDebugBall, program, matView, matProjection);

        //pDebugBall->colourRGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        //pDebugBall->scale = dist50;
        //DrawObject(pDebugBall, program, matView, matProjection);

        //pDebugBall->colourRGBA = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        //pDebugBall->scale = dist75;
        //DrawObject(pDebugBall, program, matView, matProjection);
   
        glfwSwapBuffers(window);
        glfwPollEvents();

    }//while ( ! glfwWindowShouldClose(window) )


    delete ::g_pShaderManager;


    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void DrawObject(cMeshObject* pCurMesh, 
                GLuint program,             // Shader program
                glm::mat4 &matView,         // View matrix
                glm::mat4 &matProjection)   // Projection matrix
{
    // For ease, copy the pointer to a sensible variable name
 //   cMeshObject* pCurMesh = *it_pCurMesh;
    if (!pCurMesh->isVisible)
    {
        //Don't draw it
        return;
    }

    // Get the uniform location variables (can do this outside of call for performance)
    GLint diffuseColour_LocID = glGetUniformLocation(program, "diffuseColour");

    GLint matModel_LocID = glGetUniformLocation(program, "matModel");
    GLint matView_LocID = glGetUniformLocation(program, "matView");
    GLint matProj_LocID = glGetUniformLocation(program, "matProj");

    GLint hasNoLighting_LocID = glGetUniformLocation(program, "hasNoLighting");

    glm::mat4 matModel;

    //         mat4x4_identity(m);
    matModel = glm::mat4(1.0f);

    //*****************************
    //*** Model Transformations ***

        // Place the object in the world at 'this' location
    glm::mat4 matTranslation
        = glm::translate(glm::mat4(1.0f),
            glm::vec3(pCurMesh->position.x,
                pCurMesh->position.y,
                pCurMesh->position.z));
    matModel = matModel * matTranslation;


    //mat4x4_rotate_Z(m, m, );
    //*************************************
    // ROTATE around Z
    glm::mat4 matRotateZ = glm::rotate(glm::mat4(1.0f),
        pCurMesh->orientation.z, // (float) glfwGetTime(), 
        glm::vec3(0.0f, 0.0f, 1.0f));
    matModel = matModel * matRotateZ;
    //*************************************

    //*************************************
    // ROTATE around Y
    glm::mat4 matRotateY = glm::rotate(glm::mat4(1.0f),
        pCurMesh->orientation.y, // (float) glfwGetTime(), 
        glm::vec3(0.0f, 1.0f, 0.0f));
    matModel = matModel * matRotateY;
    //*************************************

    //*************************************
    // ROTATE around X
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
        pCurMesh->orientation.x, // (float) glfwGetTime(), 
        glm::vec3(1.0f, 0.0, 0.0f));
    matModel = matModel * rotateX;
    //*************************************


    // Set up a scaling matrix
    glm::mat4 matScale = glm::mat4(1.0f);

    float theScale = pCurMesh->scale;		// 1.0f;		
    matScale = glm::scale(glm::mat4(1.0f),
        glm::vec3(theScale, theScale, theScale));


    // Apply (multiply) the scaling matrix to 
    // the existing "model" (or "world") matrix
    matModel = matModel * matScale;

    //*** End Model Transformations ***
    //*********************************

    //mat4x4_mul(mvp, p, m);
    //mvp = p * v * matModel;
    //glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
    //glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(matModel_LocID, 1, GL_FALSE, glm::value_ptr(matModel));
    glUniformMatrix4fv(matView_LocID, 1, GL_FALSE, glm::value_ptr(matView));
    glUniformMatrix4fv(matProj_LocID, 1, GL_FALSE, glm::value_ptr(matProjection));

    glUseProgram(program);


    // This will change the model to "wireframe" and "solid"
    // In this example, it's changed by pressing "9" and "0" keys
    if (pCurMesh->isWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Wireframe
        //turn off the lighting on THIS object
        glUniform1f(hasNoLighting_LocID, (float)GL_TRUE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Solid
        //turn on the lighting on THIS object
        glUniform1f(hasNoLighting_LocID, (float)GL_FALSE);
    }

    // set the uniform colour info
    glUniform3f(diffuseColour_LocID, pCurMesh->colourRGBA.r, pCurMesh->colourRGBA.g,
        pCurMesh->colourRGBA.b);

    sModelDrawInfo mdoModelToDraw;
    if (::g_pTheVAOManager->FindDrawInfoByModelName(pCurMesh->meshName,
        mdoModelToDraw))
    {
        glBindVertexArray(mdoModelToDraw.VAO_ID);

        glDrawElements(GL_TRIANGLES,
            mdoModelToDraw.numberOfIndices,
            GL_UNSIGNED_INT,     // How big the index values are 
            0);        // Starting index for this model

        glBindVertexArray(0);
    }
    return;
} // DrawObject()