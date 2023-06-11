#ifndef __COMMON_HLSLI__
#define __COMMON_HLSLI__

// ���̴����� ���������� ����ϴ� ������ Ÿ���� ����.

#define MAX_LIGHTS 5

// ���÷����� ��� ���̴����� �������� ���
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
    float3 posModel : POSITION; //�� ��ǥ���� ��ġ position
    float3 normalModel : NORMAL0; // �� ��ǥ���� normal    
    float2 texcoord : TEXCOORD0;
    float3 tangentModel : TANGENT0;
    float3 color : COLOR0;
};

struct PixelShaderInput
{
    float4 posProj : SV_POSITION; // Screen position
    float3 posWorld : POSITION; // World position (���� ��꿡 ���)
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
    matrix invProj;      // �� projection ���. view �������� �ٲ� �� ���.
    matrix invViewProj;  // �� view-projection ���. world �������� �ٲ� �� ���.
    float3 eyeWorld;     // ����(ī�޶�) ���� ��ġ.
}

cbuffer GlobalSceneDataCostant : register(b2) // �ȼ� ���̴����� ���.
{
    Light lights[MAX_LIGHTS];
}
/* Constant Buffers */

/* Functions */
float3 BlinnPhong(float3 lightStrength, float3 vertexToLight, float3 vertexNormal, float3 vertexToEye, Material mat)
{
    float3 halfwayVec = normalize(vertexToEye + vertexToLight); // ������ ���ϴ� ���Ϳ�, ������ ���ϴ� ������ �߰� ���� ���͸� ���.
    float hdotn = dot(halfwayVec, vertexNormal);
    float3 specular = mat.specular * pow(max(hdotn, 0.0f), mat.shininess);
    
    return mat.ambient + (mat.diffuse + specular) * lightStrength;
}

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
    // �Է����� �־��� �Ÿ��� falloffStart, falloffEnd ������ ��� ������ �ִ��� [0,1] ������ ��Ÿ��.
    // falloffStart�� ����� ���� 1�� �������.
    // Linear falloff
    return saturate((falloffEnd - d) / (falloffEnd - falloffStart));
}

float3 ComputeDirectionalLight(Light light, Material mat, float3 vertexNormal, float3 vertexToEye)
{
    float3 vertexToLight = -light.direction; // �������� ������ ���ϴ� ����.
    
    float ndotl = max(dot(vertexToLight, vertexNormal), 0.0f);
    float3 lightStrength = light.intensity * ndotl;
    
    return BlinnPhong(lightStrength, vertexToLight, vertexNormal, vertexToEye, mat);
}

float3 ComputePointLight(Light light, Material mat, float3 vertexPos, float3 vertexNormal, float3 vertexToEye)
{
    float3 vertexToLight = light.position - vertexPos;

    // ���̵��� �������� ��������� �Ÿ� ���
    float d = length(vertexToLight);

    // �ʹ� �ָ� ������ ������� ����
    if (d > light.fallOffEnd)
    {
        return float3(0.0, 0.0, 0.0);
    }
    else
    {
        vertexToLight /= d;

        float ndotl = max(dot(vertexToLight, vertexNormal), 0.0f);
        float3 lightStrength = light.intensity * ndotl;

        float att = CalcAttenuation(d, light.fallOffStart, light.fallOffEnd); // �Ÿ��� ���� ���� ���踦 �����.
        lightStrength *= att;

        return BlinnPhong(lightStrength, vertexToLight, vertexNormal, vertexToEye, mat);
    }
}

/* Functions */


#endif // __COMMON_HLSLI__