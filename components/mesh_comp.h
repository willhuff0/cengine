//
// Created by Will on 7/6/24.
//

#ifndef MESH_COMP_H
#define MESH_COMP_H

#include "../node.h"
#include "../simple_mesh.h"
#include "../pbr_mesh.h"

typedef struct {
    enum MeshType {
        MESH_NONE,
        MESH_SIMPLE,
        MESH_PBR,
    } meshType;
    union Mesh {
        SimpleMesh* simpleMesh;
        PbrMesh* pbrMesh;
    } mesh;
} MeshComp;

MeshComp* addMeshComp(Node* node);
void freeMeshComp(MeshComp* comp);

void tickMeshComp(MeshComp* comp);
void drawMeshComp(MeshComp* comp);

#endif //MESH_COMP_H
