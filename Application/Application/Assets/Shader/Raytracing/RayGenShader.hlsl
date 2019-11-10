#ifndef SHADER_RAYTRACING_RAYGENSHADER_HLSL
#define SHADER_RAYTRACING_RAYGENSHADER_HLSL

#define HLSL
#include "Util/Global.hlsli"
#include "Util/Helper.hlsli"

[shader("raygeneration")]
void RayGenShader() {
    Ray ray = generateCameraRay(DispatchRaysIndex().xy, g_sceneCB.cameraPosition.xyz, g_sceneCB.projectionToWorld);

    float4 color = traceRadianceRay(ray, 0);
    g_renderTarget[DispatchRaysIndex().xy] = color;
}

#endif //! SHADER_RAYTRACING_RAYGENSHADER_HLSL