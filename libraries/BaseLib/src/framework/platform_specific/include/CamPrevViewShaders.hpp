#ifndef CAM_PREV_VIEW_SHADERS_HPP
#define CAM_PREV_VIEW_SHADERS_HPP

// a - attribute
// u - uniform
// v - varying



const char* SimplePreviewViewVS =
        "uniform mat4 uTextureTransform;\n"
        "varying vec2 vTexCoord;\n"
        "void main() {\n"
        "   vTexCoord = (uTextureTransform * vec4(aTexCoord, 0.0, 1.0)).xy;\n"
        "   gl_Position = aPosition;\n"
        "}\n";

const char* SimplePreviewViewFS =
        #ifdef BUILD_FOR_ANDROID 
        "//#extension GL_OES_EGL_image_external : require\n"
        #endif
        #ifdef BUILD_FOR_ANDROID 
        "uniform samplerExternalOES textureOES;\n"
        #endif
        #ifdef BUILD_FOR_IOS
        "uniform sampler2D textureY;\n"
        "uniform sampler2D textureUV;\n"
        #endif
        "varying vec2 vTexCoord;\n"
        "\n"
        "void main () { \n"
        // get color from texture/s
        #ifdef BUILD_FOR_ANDROID 
        "    vec4 color = texture2D(textureOES, vTexCoord);\n"
        #endif
        #ifdef BUILD_FOR_IOS
        "   vec3 yuv; \n"
        "   yuv.x  = texture2D(textureY, vTexCoord).r; \n"
        "   yuv.yz = texture2D(textureUV, vTexCoord).rg - vec2(0.5, 0.5); \n"
        "   vec3 rgb = mat3(1,1,1,  0,-.18732,1.8556,   1.57481,-.46813,0) * yuv; \n" // SDTV = rgb = mat3(1,1,1, 0, -.34413, 1.772, 1.402, -.71414, 0) * yuv;\n
        "   vec4 color = vec4(rgb, 1); \n"
        #endif
        "    gl_FragColor = color;\n"
        "}\n";

/* ++++++++++++ Noctovision +++++++++++ */
const char* NoctovisionPreviewViewFS =
        #ifdef BUILD_FOR_ANDROID 
        "//#extension GL_OES_EGL_image_external : require\n"
        #endif
        #ifdef BUILD_FOR_ANDROID 
        "uniform samplerExternalOES textureOES;\n"
        #endif
        #ifdef BUILD_FOR_IOS
        "uniform sampler2D textureY;\n"
        "uniform sampler2D textureUV;\n"
        #endif
        "varying vec2 vTexCoord;\n"
        "\n"
        "void main () { \n"
        // get color from texture/s
        #ifdef BUILD_FOR_ANDROID 
        "    vec4 color = texture2D(textureOES, vTexCoord);\n"
        #endif
        #ifdef BUILD_FOR_IOS
        "   vec3 yuv; \n"
        "   yuv.x  = texture2D(textureY, vTexCoord).r; \n"
        "   yuv.yz = texture2D(textureUV, vTexCoord).rg - vec2(0.5, 0.5); \n"
        "   vec3 rgb = mat3(1,1,1,  0,-.18732,1.8556,   1.57481,-.46813,0) * yuv; \n"
        "   vec4 color = vec4(rgb, 1); \n"
        #endif
        "   color.x = color.x / 0.2; \n" // stretching the histogram 0.0 - 0.2 area to full 0.0 - 1.0
        "   color.y = color.y / 0.2; \n"
        "   color.z = color.z / 0.2; \n"
        "   float green = ((color.x + color.y + color.z)*1.0)/3.0; \n"
        "   if(green > 1.0) green = 1.0; \n"
        "   float wspol = 1.0-green; \n"
        "   gl_FragColor = vec4(1.0 - (wspol*2.0), green, 1.0 - (wspol*2.0), 1.0);\n"
        "}\n";

/* +++++++++++++ HRM ++++++++++++++++*/
const char* HRMPreviewViewFS =
        #ifdef BUILD_FOR_ANDROID 
        "//#extension GL_OES_EGL_image_external : require\n"
        #endif
        #ifdef BUILD_FOR_ANDROID 
        "uniform samplerExternalOES textureOES;\n"
        #endif
        #ifdef BUILD_FOR_IOS
        "uniform sampler2D textureY;\n"
        "uniform sampler2D textureUV;\n"
        #endif
        "varying vec2 vTexCoord;\n"
        "\n"
        "void main () { \n"
        // get color from texture/s
        #ifdef BUILD_FOR_ANDROID 
        "    vec4 color = texture2D(textureOES, vTexCoord);\n"
        #endif
        #ifdef BUILD_FOR_IOS
        "   vec3 yuv; \n"
        "   yuv.x  = texture2D(textureY, vTexCoord).r; \n"
        "   yuv.yz = texture2D(textureUV, vTexCoord).rg - vec2(0.5, 0.5); \n"
        "   vec3 rgb = mat3(1,1,1,  0,-.18732,1.8556,   1.57481,-.46813,0) * yuv; \n"
        "   vec4 color = vec4(rgb, 1); \n"
        #endif
        "   gl_FragColor = vec4(color.xyz, 1.0);\n"
        "}\n";

/* ++++++++++++ Filter ++++++++++++++*/
const char* FilterPreviewViewFS =
        #ifdef BUILD_FOR_ANDROID 
        "//#extension GL_OES_EGL_image_external : require\n"
        #endif
        #ifdef BUILD_FOR_ANDROID 
        "uniform samplerExternalOES textureOES;\n"
        #endif
        #ifdef BUILD_FOR_IOS
        "uniform sampler2D textureY;\n"
        "uniform sampler2D textureUV;\n"
        #endif

        "#define KERNEL_SIZE 9 \n" // this can be dinamically included before compiling (parametrize in Filter)

        "uniform float uWidth; \n"
        "uniform float uHeight; \n"
        "uniform mat3 uKernel; \n"
        "uniform float uKernelDivisor; \n"
        "// uniform vec2 uOffsets[KERNEL_SIZE]; // not used, is slower when providing it from cpu \n"
        "varying vec2 vTexCoord;\n"

        "\n"
        "void main () { \n"
        "   float kernel[KERNEL_SIZE]; \n"
        "   float step_w = (1.0/uWidth); \n"
        "   float step_h = (1.0/uHeight); \n"
        "   vec2  offset[KERNEL_SIZE]; \n"
        "   int i = 0; \n"
        "   vec4 sum = vec4(0.0); \n"

        "   offset[0] = vec2(-step_w, step_h);   // north west \n"
        "   offset[1] = vec2(0.0, step_h);       // north \n"
        "   offset[2] = vec2(step_w, step_h);    // north east \n"

        "   offset[3] = vec2(-step_w, 0.0);      // west \n"
        "   offset[4] = vec2(0.0, 0.0);          // center \n"
        "   offset[5] = vec2(step_w, 0.0);       // east \n"

        "   offset[6] = vec2(-step_w, -step_h);  // south west  \n"
        "   offset[7] = vec2(0.0, -step_h);      // south \n"
        "   offset[8] = vec2(step_w, -step_h);   // south east \n"

        "   kernel[0] =  uKernel[0][0];     kernel[1] =  uKernel[1][0];    kernel[2] = uKernel[2][0]; \n"
        "   kernel[3] =  uKernel[0][1];     kernel[4] =  uKernel[1][1];    kernel[5] = uKernel[2][1]; \n"
        "   kernel[6] =  uKernel[0][2];     kernel[7] =  uKernel[1][2];    kernel[8] = uKernel[2][2]; \n"

        "   for( i=0; i < KERNEL_SIZE; i++ ) \n"
        "   { \n"
        // get color from texture/s
        #ifdef BUILD_FOR_ANDROID 
        "       vec4 color = texture2D(textureOES, vTexCoord + offset[i]);\n"
        #endif
        #ifdef BUILD_FOR_IOS
        "       vec3 yuv; \n"
        "       yuv.x  = texture2D(textureY, vTexCoord + offset[i]).r; \n"
        "       yuv.yz = texture2D(textureUV, vTexCoord + offset[i]).rg - vec2(0.5, 0.5); \n"
        "       vec3 rgb = mat3(1,1,1,  0,-.18732,1.8556,   1.57481,-.46813,0) * yuv; \n"
        "       vec4 color = vec4(rgb, 1); \n"
        #endif
        "       sum += color * kernel[i]; \n"
        "   } \n"
        "    sum /= uKernelDivisor; \n"
        "   sum = vec4(sum.rgb, 1.0); // to reset alpha damaged by filters \n"
        "   gl_FragColor = sum;\n"
        "}\n";

/* +++++++++++ Movement ++++++++++++*/
const char* MovementPreviewViewFS =
        #ifdef BUILD_FOR_ANDROID 
        "//#extension GL_OES_EGL_image_external : require\n"
        #endif
        #ifdef BUILD_FOR_ANDROID 
        "uniform samplerExternalOES textureOES;\n"
        #endif
        #ifdef BUILD_FOR_IOS
        "uniform sampler2D textureY;\n"
        "uniform sampler2D textureUV;\n"
        #endif
        "uniform float uMoveX; \n"
        "uniform float uMoveY; \n"
        "varying vec2 vTexCoord;\n"
        "void main () {\n"
        "   vec2 movedTexCoord = vTexCoord; \n"
        "   movedTexCoord.x += uMoveX; \n"
        "   movedTexCoord.y += uMoveY; \n"
        #ifdef BUILD_FOR_ANDROID 
        "    vec4 color1 = texture2D(textureOES, vTexCoord);\n"
        "    vec4 color2 = texture2D(textureOES, movedTexCoord);\n"
        #endif
        #ifdef BUILD_FOR_IOS
        "   vec3 yuv; \n"
        "   vec3 rgb; \n"
        "   yuv.x  = texture2D(textureY, vTexCoord).r; \n"
        "   yuv.yz = texture2D(textureUV, vTexCoord).rg - vec2(0.5, 0.5); \n"
        "   rgb = mat3(1,1,1,  0,-.18732,1.8556,   1.57481,-.46813,0) * yuv; \n"
        "   vec4 color1 = vec4(rgb, 1); \n"
        "   yuv.x  = texture2D(textureY, movedTexCoord).r; \n"
        "   yuv.yz = texture2D(textureUV, movedTexCoord).rg - vec2(0.5, 0.5); \n"
        "   rgb = mat3(1,1,1,  0,-.18732,1.8556,   1.57481,-.46813,0) * yuv; \n"
        "   vec4 color2 = vec4(rgb, 1); \n"
        #endif
        "   color1 = (color1 + color2) / 2.0; \n"
        "   gl_FragColor = color1;\n"
        "}\n";


const char* SimpleChainElementVS =
        "varying vec2 vTexCoord;\n"
        "void main() {\n"
        "   vTexCoord = aTexCoord;\n"
        "   gl_Position = aPosition;\n"
        "}\n";

const char* SimpleChainElementFS =
        "uniform sampler2D uTexture;\n"
        "varying vec2 vTexCoord;\n"
        "void main () { \n"
        "    vec4 color = texture2D(uTexture, vTexCoord);\n"
        "    color.r += 0.2; \n"
        "    gl_FragColor = color;\n"
        "}\n";


#endif



//const char* FilterPreviewViewFS =
//"#ifdef ANDROID \n"
//"//#extension GL_OES_EGL_image_external : require\n"
//#endif
//"#ifdef ANDROID \n"
//    "uniform samplerExternalOES textureOES;\n"
//#endif
//#ifdef BUILD_FOR_IOS
//    "uniform sampler2D textureY;\n"
//    "uniform sampler2D textureUV;\n"
//#endif
//"uniform float uWidth; \n"
//"uniform float uHeight; \n"
//"varying vec2 vTexCoord;\n"
//
//"#define KERNEL_SIZE 9 \n"
//"float kernel[KERNEL_SIZE]; \n"
//"float step_w = (1.0/uWidth); \n"
//"float step_h = (1.0/uHeight); \n"
//"vec2  offset[KERNEL_SIZE]; \n"
//"\n"
//"void main () { \n"
//"   int i = 0; \n"
//"   vec4 sum = vec4(0.0); \n"
//
//"   offset[0] = vec2(-step_w, step_h);   // north west \n"
//"   offset[1] = vec2(0.0, step_h);       // north \n"
//"   offset[2] = vec2(step_w, step_h);    // north east \n"
//
//"   offset[3] = vec2(-step_w, 0.0);      // west \n"
//"   offset[4] = vec2(0.0, 0.0);          // center \n"
//"   offset[5] = vec2(step_w, 0.0);       // east \n"
//
//"   offset[6] = vec2(-step_w, -step_h);  // south west  \n"
//"   offset[7] = vec2(0.0, -step_h);      // south \n"
//"   offset[8] = vec2(step_w, -step_h);   // south east \n"
//
//"   kernel[0] =  -1.0;     kernel[1] =  -1.0;    kernel[2] = -1.0; \n"
//"   kernel[3] =  -1.0;     kernel[4] =  9.0;    kernel[5] =  -1.0; \n"
//"   kernel[6] =  -1.0;     kernel[7] =  -1.0;    kernel[8] = -1.0; \n"
//
//"   for( i=0; i < KERNEL_SIZE; i++ ) \n"
//"   { \n"
//// get color from texture/s
//"#ifdef ANDROID \n"
//"       vec4 color = texture2D(textureOES, vTexCoord + offset[i]);\n"
//#endif
//#ifdef BUILD_FOR_IOS
//"       vec3 yuv; \n"
//"       yuv.x  = texture2D(textureY, vTexCoord + offset[i]).r; \n"
//"       yuv.yz = texture2D(textureUV, vTexCoord + offset[i]).rg - vec2(0.5, 0.5); \n"
//"       vec3 rgb = mat3(1,1,1,  0,-.18732,1.8556,   1.57481,-.46813,0) * yuv; \n"
//"       vec4 color = vec4(rgb, 1); \n"
//#endif
//"       sum += color * kernel[i]; \n"
//"   } \n"
//"   // sum /= 3.0; \n"
//"   sum = vec4(sum.rgb, 1.0); // to reset alpha damaged by filters \n"
//"   gl_FragColor = sum;\n"
//"}\n";
