#ifndef _AEX_AEX_FILE_DETAILS_HPP
#define _AEX_AEX_FILE_DETAILS_HPP
#include <fstream>

namespace aex
{
    enum MeshType   {NONE, STATIC_MESH, ANIMATED_MESH, SKINNED_MESH, LOD_MESH};
    enum OffsetType {NONE_TYPE, GL_VERTICES, GL_UV, GL_NORMALS, GL_INDICES, GL_KEYFRAME, GL_KEYFRAME_NORMALS, GL_KEYFRAME_UV, GL_BONEID, GL_BONEWEIGHT, CUSTOM_ATTRIB, SUB_MESH, STATIC_MESH_OFFSET, LOD_MESH_OFFSET};

    struct FileOffsetData
    {
        // For now int64_t should be big enough for most platforms.
        using offset_type = int64_t;
        offset_type headerOffset;
        offset_type attribOffset;
        OffsetType attrT;
    };
}

#endif //  _AEX_AEX_FILE_DETAILS_HPP
