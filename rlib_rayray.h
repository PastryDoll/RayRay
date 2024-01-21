#define internal static
#define global static
#define Assert assert

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef s32 b32;
typedef s32 b32x;

typedef float f32;
typedef double f64;

#define U32Max ((u32)-1)
#define F32Max FLT_MAX
#define F32Min -FLT_MAX
// #define Tau32 6.28318530717958647692f
#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))


// struct material
// {
//     f32 Specular; // NOTE(case): 0 is pure diffuse ("chalk"), 1 is pure specular ("mirror")
//     v3 RefColor;
//     v3 EmitColor;
    
//     brdf_table BRDF;
// };

struct material
{   
    f32 Specular; // 0 is pure diffuse
    Vector3 EmitColor;
    Vector3 RefColor;
};

struct sphere 
{
    Vector3 Center;
    float Radius;
    u32 MatIndex;
};

struct plane
{
    Vector3 Normal; //, Tangent, Binormal;
    f32 d;
    u32 MatIndex;
};

struct world
{
    u32 MaterialCount;
    material *Materials;
    
    u32 PlaneCount;
    plane *Planes;
    
    u32 SphereCount;
    sphere *Spheres;
};

struct cast_state
{
    world *World;
    u32 RaysPerPixel;
    // u32 MaxBounceCount;
//     random_series Series;
    
    // v3 CameraP;
    // v3 CameraX;
    // v3 CameraY;
    // v3 CameraZ;
    
//     f32 FilmW;
//     f32 FilmH;
//     f32 HalfFilmW;
//     f32 HalfFilmH;
//     v3 FilmCenter;
    
//     f32 HalfPixW;
//     f32 HalfPixH;
    
//     f32 FilmX;
//     f32 FilmY;
    
//     // NOTE(casey): Out
//     v3 FinalColor;
//     u64 BouncesComputed;
//     u64 LoopsComputed;
};