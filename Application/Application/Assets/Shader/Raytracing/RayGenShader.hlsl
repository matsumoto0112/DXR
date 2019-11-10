#ifndef SHADER_RAYTRACING_RAYGENSHADER_HLSL
#define SHADER_RAYTRACING_RAYGENSHADER_HLSL

#define HLSL
#include "Util/Global.hlsli"
#include "Util/Helper.hlsli"

[shader("raygeneration")]
void RayGenShader() {
    //ƒJƒƒ‰‚©‚ç‚ÌƒŒƒC‚ğ¶¬
    Ray ray = GenerateCameraRay(DispatchRaysIndex().xy, g_sceneCB.cameraPosition.xyz, g_sceneCB.projectionToWorld);

    //”ò‚Î‚µ‚½æ‚ÌF‚ğæ“¾
    float4 color = RayCast(ray, 0);
    g_renderTarget[DispatchRaysIndex().xy] = color;
}

#endif //! SHADER_RAYTRACING_RAYGENSHADER_HLSL