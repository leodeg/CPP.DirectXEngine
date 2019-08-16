cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;

    float3 dynamicLightColor;
    float dynamicLightStrength;

    float3 dynamicLightPos;

    float dynamicLightAttenuationA;
    float dynamicLightAttenuationB;
    float dynamicLightAttenuationC;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main (PS_INPUT input) : SV_TARGET
{
    float3 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    //sampleColor *= ambientLightColor * ambientLightStrength;
    //sampleColor *= input.inNormal;

	// Base light and color
    float3 ambientLight = ambientLightColor * ambientLightStrength;
    float3 appliedLight = ambientLight;

	// Calculate light intensity
    float3 vectorToLight = normalize(dynamicLightPos - input.inWorldPos);
    float3 diffuseLightIntensity = max(dot(vectorToLight, input.inNormal), 0);

	// Light attenuation
    float distanceToLight = distance(dynamicLightPos, input.inWorldPos);
    float attenuationFactor = 1 / (dynamicLightAttenuationA 
							+ (dynamicLightAttenuationB * distanceToLight) 
							+ (dynamicLightAttenuationC * pow(distanceToLight, 2)));

    diffuseLightIntensity *= attenuationFactor;


	// Final colors
    float3 diffuseLight = diffuseLightIntensity * dynamicLightStrength * dynamicLightColor;
    appliedLight += diffuseLight;
    float3 finalColor = sampleColor * appliedLight; //* saturate(appliedLight);

    return float4(finalColor, 1.0f);
}