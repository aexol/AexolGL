#include <aex/MeshImporter.hpp>
#include <aex/Mesh.hpp>
#include "gtest/gtest.h"

TEST(Assimp, mesh_dxf)
{
    aex::MeshImporter mi;
    std::vector<aex::Mesh> meshes(mi.GetNumMeshes("samolot.dxf"));
    int n = mi.importAll(meshes);
    EXPECT_EQ(meshes.size(), mi.GetNumMeshes());
    EXPECT_EQ(meshes.size(), n);
    EXPECT_THROW(mi.GetNumMeshes("BadName.model"), std::runtime_error);
}
TEST(Assimp, mesh_fbx)
{
    aex::MeshImporter mi;
    std::vector<aex::Mesh> meshes(mi.GetNumMeshes("samolot2.fbx"));
    int n = mi.importAll(meshes);
    EXPECT_EQ(meshes.size(), mi.GetNumMeshes());
    EXPECT_EQ(meshes.size(), n);
    EXPECT_THROW(mi.GetNumMeshes("BadName.model"), std::runtime_error);
}
