#ifndef __COMMON_HLSLI__
#define __COMMON_HLSLI__

// 쉐이더에서 공통적으로 사용하는 데이터 타입을 정의.

#define MAX_LIGHTS 5

// 샘플러들을 모든 쉐이더에서 공통으로 사용
SamplerState linearWrapSampler : register(s0);
SamplerState linearClampSampler : register(s1);

/* Structs */
struct Material
{
    float3 ambient;
    float shininess;
    float3 diffuse;
    int bUseTexture;
    float3 specular;
    float dummy1;
    float3 color;
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
    int type;
    float3 dummy;
    int turnOn;
};

struct VertexShaderInput
{
    float3 posModel : POSITION; //모델 좌표계의 위치 position
    float3 normalModel : NORMAL0; // 모델 좌표계의 normal    
    float2 texcoord : TEXCOORD0;
    float3 tangentModel : TANGENT0;
    float3 color : COLOR0;
};

struct PixelShaderInput
{
    float4 posProj : SV_POSITION; // Screen position
    float3 posWorld : POSITION; // World position (조명 계산에 사용)
    float3 normalWorld : NORMAL0;
    float2 texcoord : TEXCOORD0;
    float3 tangentWorld : TANGENT0;
    float3 color : COLOR0;
};
/* Structs */

/* Constant Buffers */
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
/* Constant Buffers */

/* Functions */
float3 BlinnPhong(float3 lightStrength, float3 vertexToLight, float3 vertexNormal, float3 vertexToEye, Material mat)
{
    float3 halfwayVec = normalize(vertexToEye + vertexToLight); // 시점을 향하는 벡터와, 광원을 향하는 벡터의 중간 지점 벡터를 계산.
    float hdotn = dot(halfwayVec, vertexNormal);
    float3 specular = mat.specular * pow(max(hdotn, 0.0f), mat.shininess);
    
    return mat.ambient + (mat.diffuse + specular) * lightStrength;
}

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
    // 입력으로 주어진 거리가 falloffStart, falloffEnd 사이의 어느 지점에 있는지 [0,1] 범위로 나타냄.
    // falloffStart에 가까울 수록 1에 가까워짐.
    // Linear falloff
    return saturate((falloffEnd - d) / (falloffEnd - falloffStart));
}

float3 ComputeDirectionalLight(Light light, Material mat, float3 vertexNormal, float3 vertexToEye)
{
    float3 vertexToLight = -light.direction; // 정점에서 광원을 향하는 벡터.
    
    float ndotl = max(dot(vertexToLight, vertexNormal), 0.0f);
    float3 lightStrength = light.intensity * ndotl;
    
    return BlinnPhong(lightStrength, vertexToLight, vertexNormal, vertexToEye, mat);
}

float3 ComputePointLight(Light light, Material mat, float3 vertexPos, float3 vertexNormal, float3 vertexToEye)
{
    float3 vertexToLight = light.position - vertexPos;

    // 쉐이딩할 지점부터 조명까지의 거리 계산
    float d = length(vertexToLight);

    // 너무 멀면 조명이 적용되지 않음
    if (d > light.fallOffEnd)
    {
        return float3(0.0, 0.0, 0.0);
    }
    else
    {
        vertexToLight /= d;

        float ndotl = max(dot(vertexToLight, vertexNormal), 0.0f);
        float3 lightStrength = light.intensity * ndotl;

        float att = CalcAttenuation(d, light.fallOffStart, light.fallOffEnd); // 거리에 따른 빛의 감쇠를 계산함.
        lightStrength *= att;

        return BlinnPhong(lightStrength, vertexToLight, vertexNormal, vertexToEye, mat);
    }
}

/* Functions */


#endif // __COMMON_HLSLI__