//
// Created by Will on 7/6/24.
//

#include "mesh_comp.h"

#include "../component.h"
#include "../draw_queue.h"
#include "../engine.h"

MeshComp* addMeshComp(Node* node) {
    MeshComp* mesh = malloc(sizeof(MeshComp));
    mesh->meshType = MESH_NONE;
    mesh->mesh.simpleMesh = NULL;
    mesh->mesh.pbrMesh = NULL;

    ADD_COMP(node, COMP_MESH, mesh, mesh)
    return mesh;
}

void freeMeshComp(MeshComp* comp) {
    free(comp);
}

void tickMeshComp(MeshComp* comp) {}

void drawMeshComp(MeshComp* comp) {
    switch (comp->meshType) {
        case MESH_SIMPLE:
            DRAW_OP(OP_BIND_SIMPLE_MESH, simpleMesh, comp->mesh.simpleMesh);
            DRAW_OP(OP_DRAW_SIMPLE_MESH, simpleMesh, comp->mesh.simpleMesh);
            break;
        case MESH_PBR:
            DRAW_OP(OP_BIND_PBR_MESH, pbrMesh, comp->mesh.pbrMesh);
            DRAW_OP(OP_DRAW_PBR_MESH, pbrMesh, comp->mesh.pbrMesh);
            break;
        case MESH_NONE:
            break;
    }
}