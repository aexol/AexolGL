                                             //
//  AexolGL.h
//  AexolGL
//
//  Created by Kamil Matysiewicz on 19.08.2013.
//
//

#ifndef AexolGL_AexolGL_h
#define AexolGL_AexolGL_h

#include <aex/Math.hpp>
#include <aex/Aex.hpp>
#include <aex/AexInstances.hpp>
#include <aex/ALoop.hpp>
#include <aex/ax_Macros.hpp>
#include <aex/Icosahedron.hpp>
#include <aex/Instance.hpp>
#include <aex/QuaternionCamera.hpp>
#include <aex/Matrix.hpp>
#include <aex/Matrix3.hpp>
#include <aex/Quaternion.hpp>
#include <aex/Vector2.hpp>
#include <aex/Vector3.hpp>
#include <aex/BoundingSphere.hpp>
#include <aex/Indexer.hpp>
#include <aex/Mesh.hpp>
#include <aex/AexRenderer.hpp>
#include <aex/Frustum.hpp>
#include <aex/Material.hpp>
#include <aex/Plane.hpp>
#include <aex/LinesDrw.hpp>
#include <aex/Texture.hpp>
#include <aex/TextureIncludes.hpp>
#include <aex/Uniform.hpp>
#include <aex/UniformArray.hpp>
#include <aex/UniformBool.hpp>
#include <aex/UniformFloat.hpp>
#include <aex/UniformInt.hpp>
#include <aex/UniformMatrix3fv.hpp>
#include <aex/UniformMatrix4fv.hpp>
#include <aex/UniformPrimitive.hpp>
#include <aex/Uniforms.hpp>
#include <aex/UniformSampler.hpp>
#include <aex/UniformStruct.hpp>
#include <aex/UniformVector1fv.hpp>
#include <aex/UniformVector2fv.hpp>
#include <aex/UniformVector3fv.hpp>
#include <aex/UniformVector4fv.hpp>
#include <aex/api_definitions.hpp>
#include <aex/FileManager.hpp>
#include <aex/platform_definitions.hpp>
#include <aex/ALoopNode.hpp>
#include <aex/Singleton.hpp>
#include <aex/utils.hpp>
#include <aex/AWindow.hpp>
#include <aex/ObjReader.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/DrawNode.hpp>
#include <aex/AexTypes.hpp>
#include <aex/ObjectRenderNode.hpp>
#include <aex/AexRenderTree.hpp>
#include <aex/Sprite.hpp>
#include <aex/PostEffect.hpp>
#include <aex/Logger.hpp>
#include <aex/RenderingBlock.hpp>
#include <aex/CameraRenderBlock.hpp>
#include <aex/RenderingTreeBlock.hpp>
#include <aex/CtxRenderingBlock.hpp>
extern int AEXMain(int argc, char* argv[]);
#define main AEXMain
#endif
