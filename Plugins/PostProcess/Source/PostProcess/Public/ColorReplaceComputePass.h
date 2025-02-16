#pragma once
#include "CoreMinimal.h"
#include "DataDrivenShaderPlatformInfo.h"
#include "SceneTexturesConfig.h"
#include "ShaderParameterStruct.h"
#include "PostProcess/PostProcessInputs.h"
#include "ShaderCompilerCore.h"


namespace ColorReplaceCompute
{
	static constexpr int32 THREADS_X = 16;
	static constexpr int32 THREADS_Y = 16;
}

struct FColorReplace
{
	FVector3f TargetColorLab;
	float PerceptionThreshold;
	FVector3f ReplacementColorHSL;
};

// This can be included in your FGlobalShader class
// Handy to keep them separate as you can use the same Params for multiple shaders
BEGIN_SHADER_PARAMETER_STRUCT(FColorReplaceParams,)
	SHADER_PARAMETER(int, ColorCount)
	// The Texture we're going to be reading from and writing to
	SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, SceneColorTexture)
	// Texture type is same as set in shader - for getting the unlit color
	SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
	SHADER_PARAMETER_STRUCT_INCLUDE(FSceneTextureShaderParameters, SceneTextures)
	// The different colors we want to replace
	SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<FColorReplace>, ColorReplacementDataBuffer)
	// How many pixels we've replaced
	SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<int>, ColorReplacementCount)
	// For setting up the indirect dispatch
	SHADER_PARAMETER_RDG_BUFFER_UAV(RWByteAddressBuffer, ExecuteIndirectBuffer)
END_SHADER_PARAMETER_STRUCT()

class FColorReplaceCS : public FGlobalShader
{
	DECLARE_EXPORTED_SHADER_TYPE(FColorReplaceCS, Global, );
	using FParameters = FColorReplaceParams;
	SHADER_USE_PARAMETER_STRUCT(FColorReplaceCS, FGlobalShader);

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters &Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
		OutEnvironment.CompilerFlags.Add(CFLAG_AllowTypedUAVLoads);

		// When changing these, you may need to change something in the shader for it to take effect
		// A simple comment with a bit of gibberish seems to be enough
		SET_SHADER_DEFINE(OutEnvironment, USE_UNLIT_SCENE_COLOR, 1);
		SET_SHADER_DEFINE(OutEnvironment, THREADS_X, ColorReplaceCompute::THREADS_X);
		SET_SHADER_DEFINE(OutEnvironment, THREADS_Y, ColorReplaceCompute::THREADS_Y);
	}
};

BEGIN_SHADER_PARAMETER_STRUCT(FIndirectComputeParams,)
	SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, SceneColorTexture)
END_SHADER_PARAMETER_STRUCT()

class FIndirectComputeCS : public FGlobalShader
{
	DECLARE_EXPORTED_SHADER_TYPE(FIndirectComputeCS, Global, );
	using FParameters = FIndirectComputeParams;
	SHADER_USE_PARAMETER_STRUCT(FIndirectComputeCS, FGlobalShader);

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters &Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
		OutEnvironment.CompilerFlags.Add(CFLAG_AllowTypedUAVLoads);
	}
};

BEGIN_SHADER_PARAMETER_STRUCT(FColorExtractParms,)
	SHADER_PARAMETER(FVector3f, TargetColor)
	SHADER_PARAMETER_RDG_TEXTURE(Texture2D, SceneColorTexture)
	SHADER_PARAMETER_STRUCT_REF(FViewUniformShaderParameters, View)
	SHADER_PARAMETER_STRUCT_INCLUDE(FSceneTextureShaderParameters, SceneTextures)

	RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

class FColorExtractPS : public FGlobalShader
{
	DECLARE_EXPORTED_SHADER_TYPE(FColorExtractPS, Global, );
	using FParameters = FColorExtractParms;
	SHADER_USE_PARAMETER_STRUCT(FColorExtractPS, FGlobalShader);

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters &Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
		SET_SHADER_DEFINE(OutEnvironment, USE_UNLIT_SCENE_COLOR, 0);
	}
};