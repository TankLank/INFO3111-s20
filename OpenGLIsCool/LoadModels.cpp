#include "cVAOManager.h"
#include "cMeshObject.h"
#include "globalThings.h"
//heres where all the models
//are loaded into the VAO and scene

void LoadAllTheModels(unsigned int shaderProgramID, cVAOManager* pTheVAOManager) {

    {// Load the bunny, too
        sModelDrawInfo mdiRabbit;
        pTheVAOManager->LoadModelIntoVAO("assets/models/bun_zipper_res4_xyz_n_rgba_uv.ply",
            mdiRabbit, shaderProgramID);
    }
    {// Load the space shuttle, too
        sModelDrawInfo mdiSpaceShuttle;
        pTheVAOManager->LoadModelIntoVAO("assets/models/SpaceShuttleOrbiter_xyz_n_rgba_uv.ply",
            mdiSpaceShuttle, shaderProgramID);
    }
    {
        sModelDrawInfo mdiSphere;
        pTheVAOManager->LoadModelIntoVAO("assets/models/ISO_Shphere_flat_4div_xyz_n_rgba_uv.ply",
            mdiSphere, shaderProgramID);
    }
    {
        sModelDrawInfo mdiTerrain;
        pTheVAOManager->LoadModelIntoVAO("assets/models/Mountain_Terrain_xyz_n_rgba_uv.ply",
            mdiTerrain, shaderProgramID);
    }
    //Checkpoint 7
    {
        sModelDrawInfo mdiATAT;
        pTheVAOManager->LoadModelIntoVAO("assets/models/atat_xyz_n_rgba_uv.ply",
            mdiATAT, shaderProgramID);
    }
    {
        sModelDrawInfo mdiSnowSpeeder;
        pTheVAOManager->LoadModelIntoVAO("assets/models/StarWarsSnowspeeder_xyz_n_rgba_uv.ply",
            mdiSnowSpeeder, shaderProgramID);
    }

    // ENDOF: Loading the models

    // Add to the list of things to draw
    cMeshObject* pATAT01 = new cMeshObject();
    pATAT01->meshName = "assets/models/atat_xyz_n_rgba_uv.ply";
    pATAT01->position.x = -28.0f;
    pATAT01->position.y = -27.0f;
    pATAT01->position.z = 78.0f;
    pATAT01->orientation = glm::vec3(0.0f, -1.01229f, 0.0f);
    pATAT01->scale = 1.0f;    // 100th of it's normal size
    pATAT01->colourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //pShuttle01->orientation
    ::g_pVecObjects.push_back(pATAT01);

    cMeshObject* pSnowSpeeder01 = new cMeshObject();
    pSnowSpeeder01->meshName = "assets/models/StarWarsSnowspeeder_xyz_n_rgba_uv.ply";
    pSnowSpeeder01->position.x = -29.0f;
    pSnowSpeeder01->position.y = -16.0f;
    pSnowSpeeder01->position.z = +66.0f;
    pSnowSpeeder01->orientation = glm::vec3(-0.802852f, 3.38593f, 1.22173f);
    pSnowSpeeder01->scale = 1.0f / 100.0f;
    pSnowSpeeder01->colourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pVecObjects.push_back(pSnowSpeeder01);

    cMeshObject* pSnowSpeeder02 = new cMeshObject();
    pSnowSpeeder02->meshName = "assets/models/StarWarsSnowspeeder_xyz_n_rgba_uv.ply";
    pSnowSpeeder02->position.x = -29.0f;
    pSnowSpeeder02->position.y = -20.0f;
    pSnowSpeeder02->position.z = 68.0f;
    pSnowSpeeder02->orientation = glm::vec3(-1.309f, 2.6878f, -0.680679f);
    pSnowSpeeder02->scale = 1.0f / 100.0f;
    pSnowSpeeder02->colourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pVecObjects.push_back(pSnowSpeeder02);

    cMeshObject* pSnowSpeeder03 = new cMeshObject();
    pSnowSpeeder03->meshName = "assets/models/StarWarsSnowspeeder_xyz_n_rgba_uv.ply";
    pSnowSpeeder03->position.x = -33.0f;
    pSnowSpeeder03->position.y = -24.0f;
    pSnowSpeeder03->position.z = +73.0f;
    pSnowSpeeder03->orientation = glm::vec3(-0.872665f, 2.42601f, -0.349066f);
    pSnowSpeeder03->scale = 1.0f / 100.0f;
    pSnowSpeeder03->colourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pVecObjects.push_back(pSnowSpeeder03);

    cMeshObject* pSphere = new cMeshObject();
    pSphere->meshName = "assets/models/ISO_Shphere_flat_4div_xyz_n_rgba_uv.ply";
    pSphere->colourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pSphere->isWireframe = true;
    //If this is false, don't draw it.
    pSphere->isVisible = true;
    pSphere->friendlyName = "DebugSphere";  // We can find it later
    ::g_pVecObjects.push_back(pSphere);

    cMeshObject* pTerrain = new cMeshObject();
    pTerrain->meshName = "assets/models/Mountain_Terrain_xyz_n_rgba_uv.ply";
    pTerrain->position.x = 6.0f;
    pTerrain->position.y = -38.0f;
    pTerrain->position.z = -0.0f;
    pTerrain->orientation = glm::vec3(0.0f, -0.0349066, -0.20944f);
    pTerrain->scale = 1.0f;
    pTerrain->colourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pVecObjects.push_back(pTerrain);

    return;
}

