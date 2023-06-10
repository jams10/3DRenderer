#ifndef __COMMON_HLSLI__
#define __COMMON_HLSLI__

// 쉐이더에서 공통적으로 사용하는 데이터 타입을 정의.

#define MAX_LIGHTS 5

// 샘플러들을 모든 쉐이더에서 공통으로 사용
SamplerState linearWrapSampler : register(s0);
SamplerState linearClampSampler : register(s1);

struct Material
{
    float3 ambient;
    float shininess;
    float3 diffuse;
    uint bUseTexture;
    float3 specular;
    float dummy2;
};

struct Light
{
    float3 intensity;
    float fallOffStart;
    float3 direction;
    float fallOffEnd;
    float3 position;
    float spotPower;
    float3 color;
    uint type;
};

cbuffer GlobalCameraTransformConstant : register(b1)
{
    matrix view;
    matrix proj;
    matrix viewProj;
    matrix invProj;      // 역 projection 행렬. view 공간으로 바꿀 때 사용.
    matrix invViewProj;  // 역 view-projection 행렬. world 공간으로 바꿀 때 사용.
    float3 eyeWorld;     // 시점(카메라) 월드 위치.
}

cbuffer GlobalSceneDataCostant : register(b2) // 픽셀 쉐이더에서 사용.
{
    Light lights[MAX_LIGHTS];
}

struct VertexShaderInput
{
    float3 posModel : POSITION;   //모델 좌표계의 위치 position
    float3 normalModel : NORMAL0; // 모델 좌표계의 normal    
    float2 texcoord : TEXCOORD0;
    float3 tangentModel : TANGENT0;
    float3 color : COLOR0;
};

struct PixelShaderInput
{
    float4 posProj : SV_POSITION; // Screen position
    float3 posWorld : POSITION;   // World position (조명 계산에 사용)
    float3 normalWorld : NORMAL0;
    float2 texcoord : TEXCOORD0;
    float3 tangentWorld : TANGENT0;
    float3 color : COLOR0;
};

#endif // __COMMON_HLSLI__