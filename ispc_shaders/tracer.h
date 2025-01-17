//
// C:/Users/alper.bile/Projects/ps_ray_trace/ispc_shaders/tracer.h
// (Header automatically generated by the ispc compiler.)
// DO NOT EDIT THIS FILE.
//

#pragma once
#include <stdint.h>

#if !defined(__cplusplus)
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#include <stdbool.h>
#else
typedef int bool;
#endif
#endif



#ifdef __cplusplus
namespace ispc { /* namespace */
#endif // __cplusplus
///////////////////////////////////////////////////////////////////////////
// Vector types with external visibility from ispc code
///////////////////////////////////////////////////////////////////////////

#ifndef __ISPC_VECTOR_float3__
#define __ISPC_VECTOR_float3__
#ifdef _MSC_VER
__declspec( align(16) ) struct float3 { float v[3]; };
#else
struct float3 { float v[3]; } __attribute__ ((aligned(16)));
#endif
#endif


///////////////////////////////////////////////////////////////////////////
// Enumerator types with external visibility from ispc code
///////////////////////////////////////////////////////////////////////////

#ifndef __ISPC_ENUM_eMaterialType__
#define __ISPC_ENUM_eMaterialType__
enum eMaterialType {
    DIFFUSE = 0,
    METAL = 1 
};
#endif


#ifndef __ISPC_ALIGN__
#if defined(__clang__) || !defined(_MSC_VER)
// Clang, GCC, ICC
#define __ISPC_ALIGN__(s) __attribute__((aligned(s)))
#define __ISPC_ALIGNED_STRUCT__(s) struct __ISPC_ALIGN__(s)
#else
// Visual Studio
#define __ISPC_ALIGN__(s) __declspec(align(s))
#define __ISPC_ALIGNED_STRUCT__(s) __ISPC_ALIGN__(s) struct
#endif
#endif

#ifndef __ISPC_STRUCT_CameraVariables__
#define __ISPC_STRUCT_CameraVariables__
struct CameraVariables {
    float focal_length;
    struct float3  origin;
    float viewport_width;
    float viewport_height;
    int32_t samples_per_pixel;
    int32_t ray_max_depth;
};
#endif

#ifndef __ISPC_STRUCT_Material__
#define __ISPC_STRUCT_Material__
struct Material {
    struct float3  albedo;
    float fuzz;
    enum eMaterialType material_type;
};
#endif

#ifndef __ISPC_STRUCT_Sphere__
#define __ISPC_STRUCT_Sphere__
struct Sphere {
    struct float3  center;
    float radius;
    struct Material material;
};
#endif


///////////////////////////////////////////////////////////////////////////
// Functions exported from ispc code
///////////////////////////////////////////////////////////////////////////
#if defined(__cplusplus) && (! defined(__ISPC_NO_EXTERN_C) || !__ISPC_NO_EXTERN_C )
extern "C" {
#endif // __cplusplus
#if defined(__cplusplus)
    extern void trace(uint8_t * pixels, struct CameraVariables &camera_vars, struct Sphere * spheres, int32_t sphere_count, uint32_t image_width, uint32_t image_height);
#else
    extern void trace(uint8_t * pixels, struct CameraVariables *camera_vars, struct Sphere * spheres, int32_t sphere_count, uint32_t image_width, uint32_t image_height);
#endif // trace function declaraion
#if defined(__cplusplus) && (! defined(__ISPC_NO_EXTERN_C) || !__ISPC_NO_EXTERN_C )
} /* end extern C */
#endif // __cplusplus


#ifdef __cplusplus
} /* namespace */
#endif // __cplusplus
