#include "ColorReplaceComputePass.h"

IMPLEMENT_SHADER_TYPE(, FColorReplaceCS, TEXT("/CustomShaders/private/ComputeShader.usf"), TEXT("ColorChangeMaskCS"), SF_Compute);
IMPLEMENT_SHADER_TYPE(, FIndirectComputeCS, TEXT("/CustomShaders/private/ComputeShader.usf"), TEXT("IndirectDispatchCS"), SF_Compute);
IMPLEMENT_SHADER_TYPE(, FColorExtractPS, TEXT("/CustomShaders/private/PixelShader.usf"), TEXT("MainPS"), SF_Pixel);
