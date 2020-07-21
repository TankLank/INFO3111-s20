#ifndef _globalVariables_HG_
#define _globalVariables_HG_
//globalVariables.h

#include "cMeshObject.h"
#include "cVAOManager.h"
#include <vector>

void LoadAllTheModels(unsigned int shaderProgramID, cVAOManager* pTheVAOManager);

// The objects we are drawing go in here! Hazzah!
extern std::vector< cMeshObject* > g_pVecObjects;

#endif