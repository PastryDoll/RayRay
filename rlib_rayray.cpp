#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <assert.h>

#include "raylib.h"

#include "rlib_rayray.h"
#include "rayray_math.h"

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720

#define RAYCOUNTMAX 8
#define RAYSPERPIXEL 1

global bool UseCosAtten = false;

internal Vector3
CameraControl(Vector3 CameraP)
{

////////////////////////////////////////////////////////////////////////////////////////////////////
    // CAMERA CONTROL
    if (IsKeyDown(KEY_UP))
    {
        CameraP.y += 0.2;
    }

    if (IsKeyDown(KEY_DOWN))
    {
        CameraP.y -= 0.2;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        CameraP.x += 0.2;
    }

    if (IsKeyDown(KEY_LEFT))
    {
        CameraP.x -= 0.2;
    }

    if (IsKeyDown(KEY_W))
    {
        CameraP.z += 0.2;
    }

    if (IsKeyDown(KEY_S))
    {
        CameraP.z -= 0.2;
    }
////////////////////////////////////////////////////////////////////////////////////////////////////

    if (IsKeyPressed(KEY_C))
    {
        UseCosAtten = !UseCosAtten;
    }

    if (IsKeyPressed(KEY_R))
    {
        CameraP = {0, -10, 1};
    }

    return(CameraP);
}

internal Vector3
RayCast(world *World, Vector3 RayOrigin, Vector3 RayDirection)
{
    // Vector3 Result = World->Materials[0].Color;
    Vector3 Result = {};
    Vector3 Attenuation = {1.0f,1.0f,1.0f};

    f32 Tolerance = 0.0001f;
    f32 MinHitDistance = 0.0001f;

    for (u32 RayCount = 0; RayCount < RAYCOUNTMAX; ++RayCount)
    {
        f32 HitDistance = F32Max;
        u32 HitMatIndex = 0;
        Vector3 NextOrigin;
        Vector3 NextNormal;

        for (u32 PlaneIndex = 0; PlaneIndex < World->PlaneCount; ++PlaneIndex)
        {
            plane Plane = World->Planes[PlaneIndex];
            
            f32 Denom = Inner(Plane.Normal, RayDirection);
            if ((Denom < -Tolerance) || (Denom > Tolerance))
            {
                f32 t = (-Plane.d - Inner(Plane.Normal, RayOrigin))/Denom;

                if ((t > MinHitDistance) && (t < HitDistance))
                {   
                    HitDistance = t;
                    HitMatIndex = Plane.MatIndex;

                    NextOrigin = RayOrigin + t*RayDirection;
                    NextNormal = Plane.Normal;
                }
            }
        }

        for (u32 SphereIndex = 0; SphereIndex < World->SphereCount; ++SphereIndex)
        {
            sphere Sphere = World->Spheres[SphereIndex];
            
            Vector3 SphereRelativeRayOrigin = RayOrigin - Sphere.Center;
            f32 a = Inner(RayDirection, RayDirection);
            f32 b = 2.0f*Inner(SphereRelativeRayOrigin, RayDirection);
            f32 c = Inner(SphereRelativeRayOrigin,SphereRelativeRayOrigin) - Sphere.Radius*Sphere.Radius;

            f32 Dem = 2.0f*a;
            f32 RootTerm = SquareRoot(b*b - 4*a*c);
            if (RootTerm > Tolerance)
            {
                f32 tplus = (-b + RootTerm)/Dem;
                f32 tneg = (-b - RootTerm)/Dem;

                f32 t = tplus;

                if ((tneg > MinHitDistance) && (tneg < tplus))
                {
                    t = tneg;
                }

                if ((t > MinHitDistance) && (t < HitDistance))
                {   
                    HitDistance = t;
                    HitMatIndex = Sphere.MatIndex;

                    NextOrigin = RayOrigin + t*RayDirection;
                    NextNormal = NOZ(NextOrigin - Sphere.Center);

                }
            }
        }

        material Mat = World->Materials[HitMatIndex];
        f32 CosAtten = 1.0f;

        if (UseCosAtten)
        {
            CosAtten = Inner(-RayDirection, NextNormal);
            if (CosAtten < 0)
            {
                CosAtten = 0;
            }
        }


        if (HitMatIndex)
        {
            Result += Hadamard(CosAtten*Mat.EmitColor,Attenuation);
            Attenuation = Hadamard(CosAtten*Mat.RefColor,Attenuation);
            RayOrigin = NextOrigin;
            // RayOrigin += HitDistance*RayDirection;
            Vector3 PureBounce = RayDirection - 2.0f*Inner(RayDirection, NextNormal)*NextNormal;
            Vector3 RandomVector = {RandomBilateral(),RandomBilateral(),RandomBilateral()};
            Vector3 RandomBounce = NOZ(NextNormal +  RandomVector);
            RayDirection = NOZ(Lerp(RandomBounce, Mat.Specular, PureBounce));
            // RayDirection = NOZ(PureBounce);
            // printf("Attenuation: %f, %f,%f\n", RayDirection.x,RayDirection.y,RayDirection.z);
        }
        else 
        {
            Result += Hadamard(Mat.EmitColor,Attenuation);
            break;
        }
    }

    return(Result);
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RayRay");

    SetTargetFPS(60);        

    material Materials[6] = {}; 
    Materials[0].EmitColor = {0.5f,0.5f,0.5f};
    Materials[1].EmitColor = {0.0f,1.0f,1.0f};
    Materials[5].EmitColor = {1.0f,0.0f,0.0f};
    Materials[1].Specular = 0.0f;
    Materials[2].RefColor = {0.7f,0.5f,0.3f};
    Materials[3].EmitColor = {1.0f,1.0f,1.0f};
    Materials[4].RefColor = {0.2f,0.7f,0.2f};
    Materials[4].Specular = 1.0f;

    plane Planes[2] = {}; 
    Planes[0].Normal = {0,0,1};
    Planes[0].d = 1.0;
    Planes[0].MatIndex = 1;
    Planes[1].Normal = {0,0,-1};
    Planes[1].d = 2.0;
    Planes[1].MatIndex = 5;

    sphere Spheres[2] = {}; 
    Spheres[0].Center = {0,0,0};
    Spheres[0].Radius = 1.0f;
    Spheres[0].MatIndex = 2;
    Spheres[1].Center = {3,-1,0};
    Spheres[1].Radius = 1.0f;
    Spheres[1].MatIndex = 3;
    // Spheres[2].Center = {-2,-1,2};
    // Spheres[2].Radius = 1.0f;
    // Spheres[2].MatIndex = 4;

    world World = {};
    World.MaterialCount = 3;
    World.Materials = Materials;
    World.PlaneCount = ArrayCount(Planes);
    World.Planes = Planes;
    World.SphereCount = ArrayCount(Spheres);
    World.Spheres = Spheres;

    u32 Height = GetScreenHeight();
    u32 Width = GetScreenWidth();
    Image TextureImage = {};
    TextureImage.height = Height;
    TextureImage.width = Width;
    TextureImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    TextureImage.mipmaps = 1;
    u32 *Pixels = (u32*)malloc(Height*Width*4);
    TextureImage.data = Pixels;
    Texture2D texture = LoadTextureFromImage(TextureImage);    

    // The scene can be tought normally as Z up, Y forward and X right 
    // Vector3 CameraP = {0, -10, 1};

    // // -Z to scene, X right, Y Down
    // Vector3 CameraZ = NOZ(CameraP);
    // Vector3 CameraX = NOZ(Cross((Vector3){0,0,1},CameraZ));
    // Vector3 CameraY = NOZ(Cross(CameraX,CameraZ));

    f32 FilmDist = 1.0f;
    f32 FilmW = 1.0f;
    f32 FilmH = 1.0f;

    if (Width > Height)
    {
        FilmH = FilmW*((f32)Height/(f32)Width);
    }
    else if (Height > Width)
    {
        FilmW = FilmH*((f32)Width/(f32)Height);
    }

    f32 HalfFilmW = 0.5f*FilmW;
    f32 HalfFilmH = 0.5f*FilmH;
    // Vector3 FilmCenter = CameraP - FilmDist*CameraZ;

    Vector3 CameraP = {0, -10, 1};
    while(!WindowShouldClose())
    {
        CameraP = CameraControl(CameraP);

        // -Z to scene, X right, Y Down
        Vector3 CameraZ = NOZ(CameraP);
        Vector3 CameraX = NOZ(Cross((Vector3){0,0,1},CameraZ));
        Vector3 CameraY = NOZ(Cross(CameraX,CameraZ));
        Vector3 FilmCenter = CameraP - FilmDist*CameraZ;

        BeginDrawing();
            ClearBackground(GREEN);
            u32 RaysPerPixel = RAYSPERPIXEL;
            u32 *Out = (u32*)Pixels;
            for (u32 Y = 0; Y < Height; ++Y)
            {
                // RayLib paints up->down, Square starts at (-1,-1) on top left.
                f32 FilmY = -1.0f + 2.0f*((f32)Y / (f32)Height);
                for (u32 X = 0; X < Width; ++X)
                {
                    f32 FilmX = -1.0f + 2.0f*((f32)X / (f32)Width);
                    Vector3 FilmP = FilmCenter + HalfFilmW*FilmX*CameraX + HalfFilmH*FilmY*CameraY;

                    Vector3 RayOrigin = CameraP;
                    Vector3 RayDirection = NOZ(FilmP - CameraP);

                    Vector3 ColorRay = {};
                    f32 Contrib = 1.0f/RaysPerPixel;
                    for (u32 RayIndex = 0; RayIndex < RaysPerPixel; ++RayIndex)
                    {
                        ColorRay += Contrib*RayCast(&World, RayOrigin, RayDirection);
                    }

                    Color ColorFilm =  ColorFromNormalized((Vector4){ColorRay.x,ColorRay.y,ColorRay.z,1.0f});

                    // PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
                    *Out++ = ((u32)ColorFilm.a << 24) | ((u32)ColorFilm.b << 16) | ((u32)ColorFilm.g << 8) | ColorFilm.r;
                }
            }
            UpdateTexture(texture, Pixels); 
            DrawTexture(texture, 0, 0, WHITE);
            DrawText(TextFormat("Cosine Attenunation: %s", UseCosAtten ? "yes" : "no"),10,30,4,RED);
            DrawFPS(10,10);
        EndDrawing();
    }

}
