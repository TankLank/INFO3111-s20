//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include "globalOpenGLStuff.h"

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
#include "cVAOManager.h"            // NEW!
#include <string>

#include "cMeshObject.h"
#include <vector>

// Camera stuff
glm::vec3 g_cameraEye = glm::vec3(0.0, 0.0, +100.0f);
glm::vec3 g_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_upVector = glm::vec3(0.0f, 1.0f, 0.0f);

// The objects we are drawing go in here! Hazzah!
std::vector< cMeshObject* > g_pVecObjects;

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

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int selectedObject = 0;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    const float CAMERASPEED = 1.0f;
    // WSAD - AD are "right and left"
    //      - SW are "forward and back"
    //      - QE are "up and down"

    if (key == GLFW_KEY_U && action == GLFW_PRESS)
    {
        selectedObject++;
        if (selectedObject == ::g_pVecObjects.size())
        {
            selectedObject = 0;
        }
    }

    if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {
        selectedObject--;
        if (selectedObject < 0)
        {
            selectedObject = (::g_pVecObjects.size() - 1);
        }
    }


    if (key == GLFW_KEY_O)
    {
        ::g_pVecObjects[selectedObject]->scale *= 1.5f;
    }
    if (key == GLFW_KEY_P)
    {
        ::g_pVecObjects[selectedObject]->scale /= 1.5f;
    }

    if (key == GLFW_KEY_K)
    {
        ::g_pVecObjects[selectedObject]->orientation.z -= glm::radians(CAMERASPEED);
    }

    //MOVE SELECTED MODEL
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

    if (key == GLFW_KEY_L) 
    {
        ::g_pVecObjects[selectedObject]->orientation.z += glm::radians(CAMERASPEED);
    }

    if (key == GLFW_KEY_K)
    {
        ::g_pVecObjects[selectedObject]->orientation.z -= glm::radians(CAMERASPEED);
    }

    if (key == GLFW_KEY_A) // go "left"
    {   
        ::g_cameraEye.x -= CAMERASPEED;
    }

    if (key == GLFW_KEY_D) // go "right"
    {
        ::g_cameraEye.x += CAMERASPEED;
    }

    if (key == GLFW_KEY_S) // go "back" (-ve Z)
    {
        ::g_cameraEye.z -= CAMERASPEED;
    }

    if (key == GLFW_KEY_W) // go "forward" (+ve Z)
    {
        ::g_cameraEye.z += CAMERASPEED;
    }

    if (key == GLFW_KEY_Q) // go "down" (-ve Y)
    {
        ::g_cameraEye.y -= CAMERASPEED;
    }

    if (key == GLFW_KEY_E) // go "up" (-ve Y)
    {
        ::g_cameraEye.y += CAMERASPEED;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // Switches from wireframe to solid (glPolygonMode)
    if (key == GLFW_KEY_9 && action == GLFW_PRESS ) {::g_isWireFrame = true; }
    if (key == GLFW_KEY_0 && action == GLFW_PRESS ) {::g_isWireFrame = false; }


    //save
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        std::string fileName = "assets/models/modelVector.txt";
        std::ofstream thePlyFile(fileName.c_str());
        if (!thePlyFile.is_open())
        {	// Something is wrong...
            std::stringstream ssError;
            ssError << "Can't open >" << fileName << "< file." << std::endl;
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
            thePlyFile << pCurMesh->colourRGBA.r;
            thePlyFile << " ";
            thePlyFile << pCurMesh->colourRGBA.g;
            thePlyFile << " ";
            thePlyFile << pCurMesh->colourRGBA.b;
            thePlyFile << " "; thePlyFile << " ";
            thePlyFile << pCurMesh->colourRGBA.a;
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

        }
        thePlyFile.close();
    }

    //load
    if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS)
    {
        ::g_pVecObjects.clear();

        std::string fileName = "assets/models/modelVector.txt";
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
                //basically create a cmeshobject and laod it ino the vector
                cMeshObject* newMesh = new cMeshObject();
                newMesh->meshName = temp;
                thePlyFile >> newMesh->position.x;
                thePlyFile >> newMesh->position.y;
                thePlyFile >> newMesh->position.z;
                thePlyFile >> newMesh->colourRGBA.r;
                thePlyFile >> newMesh->colourRGBA.g;
                thePlyFile >> newMesh->colourRGBA.b;
                thePlyFile >> newMesh->colourRGBA.a;
                thePlyFile >> newMesh->orientation.x;
                thePlyFile >> newMesh->orientation.y;
                thePlyFile >> newMesh->orientation.z;
                thePlyFile >> newMesh->scale;
                thePlyFile >> newMesh->isWireframe;

                ::g_pVecObjects.push_back(newMesh);
            }
            else
            {
                break;
            }
        }
        thePlyFile.close();
    }

    // Print out camera location:
    std::cout << "cam: "
        << ::g_cameraEye.x << ", "
        << ::g_cameraEye.y << ", "
        << ::g_cameraEye.z << std::endl;

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
    window = glfwCreateWindow(1024, 700, "9 is wireframe, 0 is solid", NULL, NULL);
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
    if ( ! ::g_pTheVAOManager->LoadModelIntoVAO( "assets/models/free_arena_ASCII_xyz_rgba.ply",
                                                 mdiArena, program) )
    {
        std::cout << "Error: " << ::g_pTheVAOManager->getLastError() << std::endl;
    }

    {// Load the bunny, too
        sModelDrawInfo mdiRabbit;
        ::g_pTheVAOManager->LoadModelIntoVAO( "assets/models/bun_zipper_res4_xyz_colour.ply", 
                                              mdiRabbit, program );
    }
    {// Load the space shuttle, too
        sModelDrawInfo mdiSpaceShuttle;
        ::g_pTheVAOManager->LoadModelIntoVAO( "assets/models/SpaceShuttleOrbiter_xyz_rgba.ply", 
                                              mdiSpaceShuttle, program );
    }
    {
        sModelDrawInfo mdiCow;
        ::g_pTheVAOManager->LoadModelIntoVAO("assets/models/cow_xyz_rgba.ply",
            mdiCow, program);
    }
    {
        sModelDrawInfo mdiPalmTree;
        ::g_pTheVAOManager->LoadModelIntoVAO("assets/models/palm-realviz_xyz_rgba.ply",
            mdiPalmTree, program);
    }
    {
        sModelDrawInfo mdiJet;
        ::g_pTheVAOManager->LoadModelIntoVAO("assets/models/ssj100_xyz_rgba.ply", 
            mdiJet, program);
    }
    {
        sModelDrawInfo mdiDolphin;
        ::g_pTheVAOManager->LoadModelIntoVAO("assets/models/dolphin_xyz_rgba.ply",
            mdiDolphin, program);
    }
    {
        sModelDrawInfo mdiHomer;
        ::g_pTheVAOManager->LoadModelIntoVAO("assets/models/homer_xyz_rgba.ply",
            mdiHomer, program);

    }
    // ENDOF: Loading the models

    // Add to the list of things to draw
    cMeshObject* pShuttle01 = new cMeshObject();
    pShuttle01->meshName = "assets/models/SpaceShuttleOrbiter_xyz_rgba.ply";
    pShuttle01->position.x = -10.0f;
    pShuttle01->scale = 1.0f/100.0f;    // 100th of it's normal size
    //pShuttle01->orientation
    ::g_pVecObjects.push_back( pShuttle01 );

    cMeshObject* pShuttle02 = new cMeshObject();
    pShuttle02->meshName = "assets/models/SpaceShuttleOrbiter_xyz_rgba.ply";
    pShuttle02->position.x = +10.0f;
    pShuttle02->scale = 1.0f/100.0f;    // 100th of it's normal size
    ::g_pVecObjects.push_back( pShuttle02 );

    cMeshObject* pBunny = new cMeshObject();
    pBunny->meshName = "assets/models/bun_zipper_res4_xyz_colour.ply";
    pBunny->position.y = +10.0f;
    pBunny->scale = 25.0f;    // 25x bigger
    ::g_pVecObjects.push_back(pBunny);

    cMeshObject* pArena = new cMeshObject();
    pArena->meshName = "assets/models/free_arena_ASCII_xyz_rgba.ply";
    pArena->position.y = -20.0f;
    pArena->scale = 1.0f;
    ::g_pVecObjects.push_back(pArena);

    cMeshObject* pCow = new cMeshObject();
    pCow->meshName = "assets/models/cow_xyz_rgba.ply";
    pCow->position.y = +20.0f;
    pCow->scale = 1.0f;
    ::g_pVecObjects.push_back(pCow);

    cMeshObject* pHomer1 = new cMeshObject();
    pHomer1->meshName = "assets/models/homer_xyz_rgba.ply";
    pHomer1->position.x = +20.0f;
    pHomer1->scale = 10.0f;
    ::g_pVecObjects.push_back(pHomer1);

    cMeshObject* pHomer2 = new cMeshObject();
    pHomer2->meshName = "assets/models/homer_xyz_rgba.ply";
    pHomer2->position.x = -20.0f;
    pHomer2->scale = 10.0f;
    ::g_pVecObjects.push_back(pHomer2);

    cMeshObject* pJet1 = new cMeshObject();
    pJet1->meshName = "assets/models/ssj100_xyz_rgba.ply";
    pJet1->position.x = -30.0f;
    pJet1->scale = 10.0f;
    ::g_pVecObjects.push_back(pJet1);

    cMeshObject* pJet2 = new cMeshObject();
    pJet2->meshName = "assets/models/ssj100_xyz_rgba.ply";
    pJet2->position.x = 30.0f;
    pJet2->scale = 10.0f;
    ::g_pVecObjects.push_back(pJet2);

    cMeshObject* pDolphin = new cMeshObject();
    pDolphin->meshName = "assets/models/dolphin_xyz_rgba.ply";
    pDolphin->position.x = 20.0f;
    pDolphin->position.y = 25.0f;
    pDolphin->scale = 0.01f;
    ::g_pVecObjects.push_back(pDolphin);


    std::cout << "We're all set! Buckle up!" << std::endl;

    while ( ! glfwWindowShouldClose(window) )
    {
        float ratio;
        int width, height;
        //       mat4x4 m, p, mvp;
        glm::mat4 matModel, p, v, mvp;

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
        p = glm::perspective(0.6f,
                             ratio,
                             0.1f,          // Near plane
                             10000.0f);      // Far plane

        v = glm::mat4(1.0f);

        //glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
        //glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        //glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

         v = glm::lookAt( ::g_cameraEye,     // "position" "eye"
                         ::g_cameraTarget,  // "at"  (looking "at")
                         ::g_upVector );    


        // *******************************************
        // **** Draw the entire scene of objects *****
        // *******************************************
        for ( std::vector< cMeshObject* >::iterator it_pCurMesh = ::g_pVecObjects.begin();
              it_pCurMesh != ::g_pVecObjects.end(); it_pCurMesh++ )
        {
            
            // For ease, copy the pointer to a sensible variable name
            cMeshObject* pCurMesh = *it_pCurMesh;

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
            mvp = p * v * matModel;


            glUseProgram(program);


            // This will change the model to "wireframe" and "solid"
            // In this example, it's changed by pressing "9" and "0" keys
            if ( pCurMesh->isWireframe )
            {  
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Wireframe
            }
            else                        
            {  
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Solid
            }


            //glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
            glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

    //        glDrawArrays(GL_TRIANGLES, 0, 3);
     //       glDrawArrays(GL_TRIANGLES, 0, ::g_numberOfVerts);

            sModelDrawInfo mdoModelToDraw;
            if (::g_pTheVAOManager->FindDrawInfoByModelName( pCurMesh->meshName,
                                                             mdoModelToDraw))
            {
                glBindVertexArray(mdoModelToDraw.VAO_ID);

                glDrawElements( GL_TRIANGLES, 
                                mdoModelToDraw.numberOfIndices, 
                                GL_UNSIGNED_INT,     // How big the index values are 
                                0 );        // Starting index for this model

                glBindVertexArray(0);
            }

        }//for ( std::vector< cMeshObject* >
        // ****************************
        // **** END OF: Draw scene ****
        // ****************************



        glfwSwapBuffers(window);
        glfwPollEvents();

    }//while ( ! glfwWindowShouldClose(window) )


    delete ::g_pShaderManager;


    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
