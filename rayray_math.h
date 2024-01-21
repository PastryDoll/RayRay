inline Vector3
operator*(f32 A, Vector3 B)
{
    Vector3 Result;

    Result.x = A*B.x;
    Result.y = A*B.y;
    Result.z = A*B.z;
    
    return(Result);
}

inline Vector3
operator*(Vector3 B, f32 A)
{
    Vector3 Result = A*B;

    return(Result);
}

inline Vector3
operator+(Vector3 A, Vector3 B)
{
    Vector3 Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;

    return(Result);
}

inline Vector3 &
operator+=(Vector3 &A, Vector3 B)
{
    A = A + B;

    return(A);
}

inline Vector3
operator-(Vector3 A, Vector3 B)
{
    Vector3 Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    Result.z = A.z - B.z;

    return(Result);
}

inline Vector3
operator-(Vector3 A)
{
    Vector3 Result;

    Result.x = -A.x;
    Result.y = -A.y;
    Result.z = -A.z;

    return(Result);
}

inline f32
Square(f32 A)
{
    f32 Result = A*A;

    return(Result);
}

inline f32 
SquareRoot(f32 A)
{
    // TODO(casey): Replace with SSE
    f32 Result = (f32)sqrt(A);
    return(Result);
}

inline Vector3
Hadamard(Vector3 A, Vector3 B)
{
    Vector3 Result = {A.x*B.x, A.y*B.y, A.z*B.z};

    return(Result);
}
inline f32

Inner(Vector3 A, Vector3 B)
{
    f32 Result = A.x*B.x + A.y*B.y + A.z*B.z;
    return(Result);
}

inline Vector3
Cross(Vector3 A, Vector3 B)
{
    Vector3 Result;
    
    Result.x = A.y*B.z - A.z*B.y;
    Result.y = A.z*B.x - A.x*B.z;
    Result.z = A.x*B.y - A.y*B.x;
    
    return(Result);
}

inline f32
LengthSq(Vector3 A)
{
    f32 Result = Inner(A, A);

    return(Result);
}

inline f32
Length(Vector3 A)
{
    f32 Result = SquareRoot(LengthSq(A));
    return(Result);
}


inline Vector3
Normalize(Vector3 A)
{
    Vector3 Result = A * (1.0f / Length(A));

    return(Result);
}

inline Vector3
NOZ(Vector3 A)
{
    Vector3 Result = {};
    
    f32 LenSq = LengthSq(A);
    if (LenSq > Square(0.0001f))
    {

        Result = A * (1.0f / SquareRoot(LenSq));
        return(Result);
    }
    else 
    {
        Vector3 ZeroVec = {0,0,0};
        return(ZeroVec);
    }
                      
}

inline Vector3
Lerp(Vector3 A, f32 t, Vector3 B)
{
    Vector3 Result = (1.0f - t)*A + t*B;

    return(Result);
}

internal f32
RandomUnilateral(void)
{
    f32 Result = (f32)rand()/(f32)RAND_MAX;
    return(Result);
}

internal f32
RandomBilateral(void)
{
    f32 Result = -1.0 + 2.0f*RandomUnilateral();
    return(Result);
}