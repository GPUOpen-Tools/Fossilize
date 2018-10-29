// C++.
#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <map>

// C.
#include <stdint.h>

// JSON.
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

// Pipeline types.
#include "rga/Vulkan/Include/rgPsoSerializerVulkan.h"

// Local.
#include "rga/Vulkan/Include/Converters/rgFossilizeConverter.h"
#include <stdio.h>

// Namespace.
using namespace rapidjson;

// String constants.
static const char* STR_FOSSILIZE_NODE_VERSION = "version";
static const char* STR_FOSSILIZE_NODE_SAMPLERS = "samplers";
static const char* STR_FOSSILIZE_NODE_SET_LAYOUTS = "setLayouts";
static const char* STR_FOSSILIZE_NODE_BINDINGS = "bindings";
static const char* STR_FOSSILIZE_NODE_PIPELINE_LAYOUTS = "pipelineLayouts";
static const char* STR_FOSSILIZE_NODE_SHADER_MODULES = "shaderModules";
static const char* STR_FOSSILIZE_NODE_RENDER_PASSES = "renderPasses";
static const char* STR_FOSSILIZE_NODE_GRAPHICS_PIPELINES = "graphicsPipelines";
static const char* STR_FOSSILIZE_NODE_COMPUTE_PIPELINES = "computePipelines";
static const char* STR_FOSSILIZE_NODE_HASH = "hash";
static const char* STR_FOSSILIZE_NODE_FLAGS = "flags";

// String constants - samplers.
static const char* STR_FOSSILIZE_NODE_SAMPLER_MIN_FILTER = "minFilter";
static const char* STR_FOSSILIZE_NODE_SAMPLER_MAG_FILTER = "magFilter";
static const char* STR_FOSSILIZE_NODE_SAMPLER_MAX_ANISOTROPY = "maxAnisotropy";
static const char* STR_FOSSILIZE_NODE_SAMPLER_COMPARE_OP = "compareOp";
static const char* STR_FOSSILIZE_NODE_SAMPLER_ANISOTROPY_ENABLE = "anisotropyEnable";
static const char* STR_FOSSILIZE_NODE_SAMPLER_MIPMAP_MODE = "mipmapMode";
static const char* STR_FOSSILIZE_NODE_SAMPLER_ADDRESS_MODE_U = "addressModeU";
static const char* STR_FOSSILIZE_NODE_SAMPLER_ADDRESS_MODE_V = "addressModeV";
static const char* STR_FOSSILIZE_NODE_SAMPLER_ADDRESS_MODE_W = "addressModeW";
static const char* STR_FOSSILIZE_NODE_SAMPLER_BORDER_COLOR = "borderColor";
static const char* STR_FOSSILIZE_NODE_SAMPLER_UNNORMALIZED_COORDINATES = "unnormalizedCoordinates";
static const char* STR_FOSSILIZE_NODE_SAMPLER_COMPARE_ENABLE = "compareEnable";
static const char* STR_FOSSILIZE_NODE_SAMPLER_MIPLOD_BIAS = "mipLodBias";
static const char* STR_FOSSILIZE_NODE_SAMPLER_MIN_LOD = "minLod";
static const char* STR_FOSSILIZE_NODE_SAMPLER_MAX_LOD = "maxLod";

// String constants - descriptor layout binding.
static const char* STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_DESC_TYPE = "descriptorType";
static const char* STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_DESC_COUNT = "descriptorCount";
static const char* STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_STAGE_FLAGS = "stageFlags";
static const char* STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_BINDING = "binding";

// String constants - pipeline layout.
static const char* STR_FOSSILIZE_NODE_PIPELINE_LAYOUT_PUSH_CONSTANT_RANGES = "pushConstantRanges";

// String constants - push constants ranges.
static const char* STR_FOSSILIZE_NODE_PUSH_CONSTANT_RANGES_STAGE_FLAGS = "stageFlags";
static const char* STR_FOSSILIZE_NODE_PUSH_CONSTANT_RANGES_SIZE = "size";
static const char* STR_FOSSILIZE_NODE_PUSH_CONSTANT_RANGES_OFFSET = "offset";

// String constants - shader stages.
static const char* STR_FOSSILIZE_NODE_PIPELINE_STAGES_NAME = "name";
static const char* STR_FOSSILIZE_NODE_PIPELINE_STAGES_MODULE = "module";
static const char* STR_FOSSILIZE_NODE_PIPELINE_STAGES_STAGE = "stage";
static const char* STR_FOSSILIZE_NODE_PIPELINE_STAGES_SPECIALIZATION_INFO = "specializationInfo";

// String constants - specialization info.
static const char* STR_FOSSILIZE_NODE_PIPELINE_STAGES_SPECIALIZATION_INFO_DATA_SIZE = "dataSize";
static const char* STR_FOSSILIZE_NODE_PIPELINE_STAGES_SPECIALIZATION_INFO_DATA = "data";
static const char* STR_FOSSILIZE_NODE_PIPELINE_STAGES_SPECIALIZATION_INFO_MAP_ENTRIES = "mapEntries";

// String constants - map entries.
static const char* STR_FOSSILIZE_NODE_PIPELINE_STAGES_SPECIALIZATION_INFO_MAP_ENTRIES_OFFSET = "offset";
static const char* STR_FOSSILIZE_NODE_PIPELINE_STAGES_SPECIALIZATION_INFO_MAP_ENTRIES_SIZE = "size";
static const char* STR_FOSSILIZE_NODE_PIPELINE_STAGES_SPECIALIZATION_INFO_MAP_ENTRIES_CONSTANT_ID = "constantID";

// String constants - render passes.
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES = "dependencies";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTIONS = "subpasses";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_ATTACHMENTS = "attachments";

// String constants - sub-pass dependencies.
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_FLAGS = "dependencyFlags";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_DST_ACCESS_MASK = "dstAccessMask";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_DST_STAGE_MASK = "dstStageMask";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_DST_SUBPASS = "dstSubpass";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_SRC_ACCESS_MASK = "srcAccessMask";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_SRC_STAGE_MASK = "srcStageMask";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_SRC_SUBPASS = "srcSubpass";

// String constants - sub-pass attachments.
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_FLAGS = "flags";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_FORMAT = "format";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_FINAL_LAYOUT = "finalLayout";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_INITIAL_LAYOUT = "initialLayout";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_LOAD_OP = "loadOp";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_STORE_OP = "storeOp";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_SAMPLES = "samples";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_STENCIL_LOAD_OP = "stencilLoadOp";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_STENCIL_STORE_OP = "stencilStoreOp";

// String constants - sub-pass descriptions.
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTION_PIPELINE_BIND_POINT = "pipelineBindPoint";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTION_COLOR_ATTACHMENTS = "colorAttachments";
static const char* STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTION_DEPTH_STENCIL_ATTACHMENTS = "depthStencilAttachment";

// String constants - attachment reference.
static const char* STR_FOSSILIZE_NODE_ATTACHMENT_REFERENCE_ATTACHMENT = "attachment";
static const char* STR_FOSSILIZE_NODE_ATTACHMENT_REFERENCE_LAYOUT = "layout";

// String constants - graphics and compute pipelines.
static const char* STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_BASE_PIPELINE_HANDLE = "basePipelineHandle";
static const char* STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_BASE_PIPELINE_INDEX = "basePipelineIndex";
static const char* STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_LAYOUT = "layout";

// String constants - graphics pipeline specific.
static const char* STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_RENDER_PASS = "renderPass";
static const char* STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_SUB_PASS = "subpass";
static const char* STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_DYNAMIC_STATE = "dynamicState";
static const char* STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_MULTISAMPLE_STATE = "multisampleState";
static const char* STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_VERTEX_INPUT_STATE = "vertexInputState";
static const char* STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_RASTERIZATION_STATE = "rasterizationState";
static const char* STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_INPUT_ASSEMBLY_STATE = "inputAssemblyState";
static const char* STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_COLOR_BLEND_STATE = "colorBlendState";
static const char* STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_VIEWPORT_STATE = "viewportState";
static const char* STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_DEPTH_STENCIL_STATE = "depthStencilState";
static const char* STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_STAGES = "stages";

// String constants - compute pipeline specific.
static const char* STR_FOSSILIZE_NODE_COMPUTE_PIPELINE_STAGE = "stage";

// String constants - multi sample state.
static const char* STR_FOSSILIZE_NODE_MULTI_SAMPLE_STATE_RASTERIZATION_SAMPLES = "rasterizationSamples";
static const char* STR_FOSSILIZE_NODE_MULTI_SAMPLE_SAMPLE_SHADING_ENABLE = "sampleShadingEnable";
static const char* STR_FOSSILIZE_NODE_MULTI_SAMPLE_MIN_SAMPLE_SHADING = "minSampleShading";
static const char* STR_FOSSILIZE_NODE_MULTI_ALPHA_TO_ONE_ENABLE = "alphaToOneEnable";
static const char* STR_FOSSILIZE_NODE_MULTI_ALPHA_TO_COVERAGE_ENABLE = "alphaToCoverageEnable";

// String constants - vertex input state.
static const char* STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES = "attributes";
static const char* STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDINGS = "bindings";

// String constants - vertex input attribute description.
static const char* STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_LOCATION = "location";
static const char* STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_BINDING = "binding";
static const char* STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_OFFSET = "offset";
static const char* STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_FORMAT = "format";

// String constants - vertex input binding description.
static const char* STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDING_BINDING = "binding";
static const char* STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDING_STRIDE = "stride";
static const char* STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDING_INPUT_RATE = "inputRate";

// String constants - rasterization.
static const char* STR_FOSSILIZE_NODE_MULTI_RASTERIZATION_STATE_DEPTH_BIAS_CONSTANT_FACTOR = "depthBiasConstantFactor";
static const char* STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_BIAS_SLOPE_FACTOR = "depthBiasSlopeFactor";
static const char* STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_BIAS_CLAMP = "depthBiasClamp";
static const char* STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_BIAS_ENABLE = "depthBiasEnable";
static const char* STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_CLAMP_ENABLE = "depthClampEnable";
static const char* STR_FOSSILIZE_NODE_RASTERIZATION_STATE_POLYGON_MODE = "polygonMode";
static const char* STR_FOSSILIZE_NODE_RASTERIZATION_STATE_RASTERIZER_DISCARD_ENABLE = "rasterizerDiscardEnable";
static const char* STR_FOSSILIZE_NODE_RASTERIZATION_STATE_FRONT_FACE = "frontFace";
static const char* STR_FOSSILIZE_NODE_RASTERIZATION_STATE_LINE_WIDTH = "lineWidth";
static const char* STR_FOSSILIZE_NODE_RASTERIZATION_STATE_CULL_MDOE = "cullMode";

// String constants - input assembly state.
static const char* STR_FOSSILIZE_NODE_INPUT_ASSEMBLY_STATE_TOPOLY = "topology";
static const char* STR_FOSSILIZE_NODE_INPUT_ASSEMBLY_STATE_PRIMITIVE_RESTART_ENABLE = "primitiveRestartEnable";

// String constants - color blend state.
static const char* STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_LOGIC_OP = "logicOp";
static const char* STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_LOGIC_OP_ENABLE = "logicOpEnable";
static const char* STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_BLEND_CONSTANTS = "blendConstants";
static const char* STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_ATTACHMENTS = "attachments";

// String constants - color blend attachments.
static const char* STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_DST_ALPHA_BLEND_FACTOR = "dstAlphaBlendFactor";
static const char* STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_SRC_ALPHA_BLEND_FACTOR = "srcAlphaBlendFactor";
static const char* STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_DST_COLOR_BLEND_FACTOR = "dstColorBlendFactor";
static const char* STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_SRC_COLOR_BLEND_FACTOR = "srcColorBlendFactor";
static const char* STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_COLOR_WRITE_MASK = "colorWriteMask";
static const char* STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_ALPHA_BLEND_OP = "alphaBlendOp";
static const char* STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_COLOR_BLEND_OP = "colorBlendOp";
static const char* STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_BLEND_ENABLE = "blendEnable";

// String constants - viewport.
static const char* STR_FOSSILIZE_NODE_VIEWPORT_STATE_VIEWPORT_COUNT = "viewportCount";
static const char* STR_FOSSILIZE_NODE_VIEWPORT_STATE_SCISSOR_COUNT = "scissorCount";

// String constants - depth stencil.
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_ENABLE = "stencilTestEnable";
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_MAX_DEPTH_BOUNDS = "maxDepthBounds";
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_MIN_DEPTH_BOUNDS = "minDepthBounds";
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_BOUNDS_TEST_ENABLE = "depthBoundsTestEnable";
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_WRITE_ENABLE = "depthWriteEnable";
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_TEST_ENABLE = "depthTestEnable";
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_COMPARE_OP = "depthCompareOp";
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_FRONT = "front";
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_BACK = "back";

// String constants - stencil op state stencil.
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_COMPARE_OP = "compareOp";
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_WRITE_MASK = "writeMask";
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_REFERENCE = "reference";
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_COMPARE_MASK = "compareMask";
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_PASS_OP = "passOp";
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_FAIL_OP = "failOp";
static const char* STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_DEPTH_FAIL_OP = "depthFailOp";

// String constants - notifications.
static const char* STR_ERROR_SPIRV_SERIALIZATION_FORMATTED = "Failed to write serialized SPIR-V binary to disk: %s.\n";
static const char* STR_ERROR_SPIRV_SERIALIZATION_WRITE_FORMATTED = "Failed to open file for writing: \"%s\".\n";
static const char* STR_SPIRV_SERIALIZATION_SUCCESS_FORMATTED = "Serialized SPIR-V binary to \"%s\".\n";

// String constants - files.
static const char* STR_FILE_EXTENSION_SPIRV = ".spv";
static const char* STR_FILE_EXTENSION_PSO = ".pso";
static const char* STR_FILE_BASE_FILENAME_PSO = "rgaPso";

static std::vector<uint8_t> LoadBufferFromFile(const char* path)
{
    FILE *pFile = fopen(path, "rb");
    if (!pFile)
        return {};

    fseek(pFile, 0, SEEK_END);
    long len = ftell(pFile);
    rewind(pFile);
    std::vector<uint8_t> ret(len);
    if (fread(ret.data(), 1, len, pFile) != size_t(len))
    {
        fclose(pFile);
        return {};
    }

    fclose(pFile);
    return ret;
}

static bool WriteBinaryFile(const std::string& outputFileName, const void* pCode, size_t codeSize)
{
    bool ret = false;
    FILE *file = fopen(outputFileName.c_str(), "wb");
    if (file)
    {
        if (fwrite(pCode, 1, codeSize, file) != codeSize)
        {
            printf(STR_ERROR_SPIRV_SERIALIZATION_FORMATTED, outputFileName.c_str());
        }
        else
        {
            printf(STR_SPIRV_SERIALIZATION_SUCCESS_FORMATTED, outputFileName.c_str());
            ret = true;
        }
        fclose(file);
    }
    else
    {
        printf(STR_ERROR_SPIRV_SERIALIZATION_WRITE_FORMATTED, outputFileName.c_str());
    }
    return ret;
}

// Convert a Fossilize sampler node to a VkSamplerCreateInfo.
static bool ToVkSamplerCreateInfo(const rapidjson::Value& sampler, VkSamplerCreateInfo& createInfo)
{
    bool ret = true;

    // Reset the output structure.
    memset(&createInfo, 0, sizeof(VkSamplerCreateInfo));

    // Verify that the members exist.
    assert(ret = ret && sampler.HasMember(STR_FOSSILIZE_NODE_SAMPLER_MIN_FILTER));
    assert(ret = ret && sampler.HasMember(STR_FOSSILIZE_NODE_SAMPLER_MAG_FILTER));
    assert(ret = ret && sampler.HasMember(STR_FOSSILIZE_NODE_SAMPLER_MAX_ANISOTROPY));
    assert(ret = ret && sampler.HasMember(STR_FOSSILIZE_NODE_SAMPLER_COMPARE_OP));
    assert(ret = ret && sampler.HasMember(STR_FOSSILIZE_NODE_SAMPLER_ANISOTROPY_ENABLE));
    assert(ret = ret && sampler.HasMember(STR_FOSSILIZE_NODE_SAMPLER_MIPMAP_MODE));
    assert(ret = ret && sampler.HasMember(STR_FOSSILIZE_NODE_SAMPLER_ADDRESS_MODE_U));
    assert(ret = ret && sampler.HasMember(STR_FOSSILIZE_NODE_SAMPLER_ADDRESS_MODE_V));
    assert(ret = ret && sampler.HasMember(STR_FOSSILIZE_NODE_SAMPLER_ADDRESS_MODE_W));
    assert(ret = ret && sampler.HasMember(STR_FOSSILIZE_NODE_SAMPLER_BORDER_COLOR));
    assert(ret = ret && sampler.HasMember(STR_FOSSILIZE_NODE_SAMPLER_UNNORMALIZED_COORDINATES));
    assert(ret = ret && sampler.HasMember(STR_FOSSILIZE_NODE_SAMPLER_COMPARE_ENABLE));
    assert(ret = ret && sampler.HasMember(STR_FOSSILIZE_NODE_SAMPLER_MIPLOD_BIAS));
    assert(ret = ret && sampler.HasMember(STR_FOSSILIZE_NODE_SAMPLER_MIN_LOD));
    assert(ret = ret && sampler.HasMember(STR_FOSSILIZE_NODE_SAMPLER_MAX_LOD));

    // Verify the type.
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_HASH].IsUint64());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_FLAGS].IsUint());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_SAMPLER_MIN_FILTER].IsUint());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_SAMPLER_MAG_FILTER].IsUint());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_SAMPLER_MAX_ANISOTROPY].IsFloat());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_SAMPLER_COMPARE_OP].IsUint());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_SAMPLER_ANISOTROPY_ENABLE].IsUint());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_SAMPLER_MIPMAP_MODE].IsUint());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_SAMPLER_ADDRESS_MODE_U].IsUint());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_SAMPLER_ADDRESS_MODE_V].IsUint());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_SAMPLER_ADDRESS_MODE_W].IsUint());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_SAMPLER_BORDER_COLOR].IsUint());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_SAMPLER_UNNORMALIZED_COORDINATES].IsUint());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_SAMPLER_COMPARE_ENABLE].IsUint());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_SAMPLER_MIPLOD_BIAS].IsFloat());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_SAMPLER_MIN_LOD].IsFloat());
    assert(ret = ret && sampler[STR_FOSSILIZE_NODE_SAMPLER_MAX_LOD].IsFloat());

    if (ret)
    {
        // Fill up the output structure.
        createInfo.flags = sampler[STR_FOSSILIZE_NODE_FLAGS].GetUint();
        createInfo.minFilter = static_cast<VkFilter>(sampler[STR_FOSSILIZE_NODE_SAMPLER_MIN_FILTER].GetUint());
        createInfo.magFilter = static_cast<VkFilter>(sampler[STR_FOSSILIZE_NODE_SAMPLER_MAG_FILTER].GetUint());
        createInfo.maxAnisotropy = sampler[STR_FOSSILIZE_NODE_SAMPLER_MAX_ANISOTROPY].GetFloat();
        createInfo.compareOp = static_cast<VkCompareOp>(sampler[STR_FOSSILIZE_NODE_SAMPLER_COMPARE_OP].GetUint());
        createInfo.anisotropyEnable = (sampler[STR_FOSSILIZE_NODE_SAMPLER_ANISOTROPY_ENABLE].GetUint() != 0);
        createInfo.mipmapMode = static_cast<VkSamplerMipmapMode>(sampler[STR_FOSSILIZE_NODE_SAMPLER_MIPMAP_MODE].GetUint());
        createInfo.addressModeU = static_cast<VkSamplerAddressMode>(sampler[STR_FOSSILIZE_NODE_SAMPLER_ADDRESS_MODE_U].GetUint());
        createInfo.addressModeV = static_cast<VkSamplerAddressMode>(sampler[STR_FOSSILIZE_NODE_SAMPLER_ADDRESS_MODE_V].GetUint());
        createInfo.addressModeW = static_cast<VkSamplerAddressMode>(sampler[STR_FOSSILIZE_NODE_SAMPLER_ADDRESS_MODE_W].GetUint());
        createInfo.borderColor = static_cast<VkBorderColor>(sampler[STR_FOSSILIZE_NODE_SAMPLER_BORDER_COLOR].GetUint());
        createInfo.unnormalizedCoordinates = (sampler[STR_FOSSILIZE_NODE_SAMPLER_UNNORMALIZED_COORDINATES].GetUint() != 0);
        createInfo.compareEnable = (sampler[STR_FOSSILIZE_NODE_SAMPLER_COMPARE_ENABLE].GetUint() != 0);
        createInfo.mipLodBias = sampler[STR_FOSSILIZE_NODE_SAMPLER_MIPLOD_BIAS].GetFloat();
        createInfo.minLod = sampler[STR_FOSSILIZE_NODE_SAMPLER_MIN_LOD].GetFloat();
        createInfo.maxLod = sampler[STR_FOSSILIZE_NODE_SAMPLER_MAX_LOD].GetFloat();
    }

    return ret;
}

bool rgFossilizeConverter::Convert(const rapidjson::Document& doc, const std::string& outputDirectory,
    const std::map<uint32_t, VkShaderModuleCreateInfo>& shaderModules, std::vector<std::string>& rgaPsoFiles)
{
    bool ret = false;

    // Get the version.
    assert(ret = doc.HasMember(STR_FOSSILIZE_NODE_VERSION));
    assert(ret = ret && doc[STR_FOSSILIZE_NODE_VERSION].IsUint());
    std::cout << STR_FOSSILIZE_NODE_VERSION << " " << doc[STR_FOSSILIZE_NODE_VERSION].GetUint() << std::endl;

    // Get the samplers.
    std::vector<VkSamplerCreateInfo*> rgaSamplers;
    assert(ret = ret && doc.HasMember(STR_FOSSILIZE_NODE_SAMPLERS));
    assert(ret = ret && doc[STR_FOSSILIZE_NODE_SAMPLERS].IsArray());
    for (const auto& sampler : doc[STR_FOSSILIZE_NODE_SAMPLERS].GetArray())
    {
        VkSamplerCreateInfo* pSamplerCreateInfo = new VkSamplerCreateInfo{};
        pSamplerCreateInfo->sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        ret = ret && ToVkSamplerCreateInfo(sampler, *pSamplerCreateInfo);
        if (ret)
        {
            // If we got it, add it to our collection of samplers.
            rgaSamplers.push_back(pSamplerCreateInfo);
        }
        else
        {
            // If something went wrong, stop trying.
            break;
        }
    }

    // Descriptor set layout.
    std::vector<VkDescriptorSetLayoutCreateInfo*> rgaDescriptorSetLayouts;
    assert(ret = ret && doc.HasMember(STR_FOSSILIZE_NODE_SET_LAYOUTS));
    assert(ret = ret && doc[STR_FOSSILIZE_NODE_SET_LAYOUTS].IsArray());
    for (const auto& descriptorSetLayout : doc[STR_FOSSILIZE_NODE_SET_LAYOUTS].GetArray())
    {
        // Verify that the members exist.
        assert(ret = ret && descriptorSetLayout.HasMember(STR_FOSSILIZE_NODE_HASH));
        assert(ret = ret && descriptorSetLayout.HasMember(STR_FOSSILIZE_NODE_FLAGS));

        // Verify the type.
        assert(ret = ret && descriptorSetLayout[STR_FOSSILIZE_NODE_HASH].IsUint64());
        assert(ret = ret && descriptorSetLayout[STR_FOSSILIZE_NODE_FLAGS].IsUint());

        // Descriptor set layout bindings.
        std::vector<VkDescriptorSetLayoutBinding> rgaDescriptorSetLayoutBindings;
        assert(ret = ret && descriptorSetLayout.HasMember(STR_FOSSILIZE_NODE_BINDINGS));
        assert(ret = ret && descriptorSetLayout[STR_FOSSILIZE_NODE_BINDINGS].IsArray());
        for (const auto& binding : descriptorSetLayout[STR_FOSSILIZE_NODE_BINDINGS].GetArray())
        {
            // Verify that the members exist.
            assert(ret = ret && binding.HasMember(STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_DESC_TYPE));
            assert(ret = ret && binding.HasMember(STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_DESC_COUNT));
            assert(ret = ret && binding.HasMember(STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_STAGE_FLAGS));
            assert(ret = ret && binding.HasMember(STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_BINDING));

            // Verify the type.
            assert(ret = ret && binding[STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_DESC_TYPE].IsUint());
            assert(ret = ret && binding[STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_DESC_COUNT].IsUint());
            assert(ret = ret && binding[STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_STAGE_FLAGS].IsUint());
            assert(ret = ret && binding[STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_BINDING].IsUint());

            // Allocate and reset.
            VkDescriptorSetLayoutBinding bindingStructure{};

            if (ret)
            {
                // Fill up the structure.
                bindingStructure.descriptorType = static_cast<VkDescriptorType>(binding[STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_DESC_TYPE].GetUint());
                bindingStructure.descriptorCount = binding[STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_DESC_COUNT].GetUint();
                bindingStructure.stageFlags = binding[STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_STAGE_FLAGS].GetUint();
                bindingStructure.binding = binding[STR_FOSSILIZE_NODE_DESCRIPTOR_SET_LAYOUT_BINDING_BINDING].GetUint();

                // Add the binding to our collection.
                rgaDescriptorSetLayoutBindings.push_back(bindingStructure);
            }
        }

        // Fill up the structure.
        VkDescriptorSetLayoutCreateInfo* pDescSetLayoutCreateInfo = new VkDescriptorSetLayoutCreateInfo{};
        pDescSetLayoutCreateInfo->sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        pDescSetLayoutCreateInfo->bindingCount = static_cast<uint32_t>(rgaDescriptorSetLayoutBindings.size());

        // Copy the binding structures.
        if (pDescSetLayoutCreateInfo->bindingCount > 0)
        {
            VkDescriptorSetLayoutBinding* pDescriptorLayoutBindings = new VkDescriptorSetLayoutBinding[pDescSetLayoutCreateInfo->bindingCount]{};
            for (uint32_t i = 0; i < rgaDescriptorSetLayoutBindings.size(); i++)
            {
                pDescriptorLayoutBindings[i] = rgaDescriptorSetLayoutBindings[i];
            }

            // Assign to the create info structure.
            pDescSetLayoutCreateInfo->pBindings = pDescriptorLayoutBindings;
        }

        // Add the descriptor set layout create info to our collection.
        rgaDescriptorSetLayouts.push_back(pDescSetLayoutCreateInfo);
    }

    // Pipeline layouts.
    std::vector<rgVkPipelineLayoutCreateInfo*> rgaPipelineLayouts;
    assert(ret = ret && doc.HasMember(STR_FOSSILIZE_NODE_PIPELINE_LAYOUTS));
    assert(ret = ret && doc[STR_FOSSILIZE_NODE_PIPELINE_LAYOUTS].IsArray());

    for (const auto& pipelineLayout : doc[STR_FOSSILIZE_NODE_PIPELINE_LAYOUTS].GetArray())
    {
        // Verify that the members exist.
        assert(ret = ret && pipelineLayout.HasMember(STR_FOSSILIZE_NODE_HASH));
        assert(ret = ret && pipelineLayout.HasMember(STR_FOSSILIZE_NODE_FLAGS));
        assert(ret = ret && pipelineLayout.HasMember(STR_FOSSILIZE_NODE_PIPELINE_LAYOUT_PUSH_CONSTANT_RANGES));
        assert(ret = ret && pipelineLayout.HasMember(STR_FOSSILIZE_NODE_SET_LAYOUTS));

        // Verify the type.
        assert(ret = ret && pipelineLayout[STR_FOSSILIZE_NODE_HASH].IsUint64());
        assert(ret = ret && pipelineLayout[STR_FOSSILIZE_NODE_FLAGS].IsUint());
        assert(ret = ret && pipelineLayout[STR_FOSSILIZE_NODE_PIPELINE_LAYOUT_PUSH_CONSTANT_RANGES].IsArray());
        assert(ret = ret && pipelineLayout[STR_FOSSILIZE_NODE_SET_LAYOUTS].IsArray());

        if (ret)
        {
            // Push constant ranges.
            auto pushConstantRangesArray = pipelineLayout[STR_FOSSILIZE_NODE_PIPELINE_LAYOUT_PUSH_CONSTANT_RANGES].GetArray();
            uint32_t pushConstantRangesCount = pushConstantRangesArray.Size();

            // Allocate and reset the push constant range array.
            VkPushConstantRange* pRgaPushConstantRanges = nullptr;
            if (pushConstantRangesCount > 0)
            {
                // Allocate and reset our push constant ranges array.
                pRgaPushConstantRanges = new VkPushConstantRange[pushConstantRangesCount]{};

                // Index in our push constant range array.
                size_t i = 0;
                for (const auto& pushConstantRange : pipelineLayout[STR_FOSSILIZE_NODE_PIPELINE_LAYOUT_PUSH_CONSTANT_RANGES].GetArray())
                {
                    // Verify that the members exist.
                    assert(ret = ret && pushConstantRange.HasMember(STR_FOSSILIZE_NODE_PUSH_CONSTANT_RANGES_STAGE_FLAGS));
                    assert(ret = ret && pushConstantRange.HasMember(STR_FOSSILIZE_NODE_PUSH_CONSTANT_RANGES_OFFSET));
                    assert(ret = ret && pushConstantRange.HasMember(STR_FOSSILIZE_NODE_PUSH_CONSTANT_RANGES_SIZE));

                    // Verify the type.
                    assert(ret = ret && pushConstantRange[STR_FOSSILIZE_NODE_PUSH_CONSTANT_RANGES_STAGE_FLAGS].IsUint());
                    assert(ret = ret && pushConstantRange[STR_FOSSILIZE_NODE_PUSH_CONSTANT_RANGES_OFFSET].IsUint());
                    assert(ret = ret && pushConstantRange[STR_FOSSILIZE_NODE_PUSH_CONSTANT_RANGES_SIZE].IsUint());

                    // Fill up the structure.
                    VkPushConstantRange& currRange = pRgaPushConstantRanges[i++];
                    currRange.stageFlags = static_cast<VkShaderStageFlags>(pushConstantRange[STR_FOSSILIZE_NODE_PUSH_CONSTANT_RANGES_STAGE_FLAGS].GetUint());
                    currRange.offset = pushConstantRange[STR_FOSSILIZE_NODE_PUSH_CONSTANT_RANGES_OFFSET].GetUint();
                    currRange.size = pushConstantRange[STR_FOSSILIZE_NODE_PUSH_CONSTANT_RANGES_SIZE].GetUint();
                }
            }

            // Get a reference to the array of descriptor set layout indices.
            const auto& descSetLayoutIndices = pipelineLayout[STR_FOSSILIZE_NODE_SET_LAYOUTS].GetArray();
            uint32_t descSetLayoutIndicesCount = descSetLayoutIndices.Size();

            rgVkPipelineLayoutCreateInfo* pRgaPipelineCreateInfo = new rgVkPipelineLayoutCreateInfo{};
            pRgaPipelineCreateInfo->m_pVkPipelineLayoutCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pRgaPipelineCreateInfo->m_pVkPipelineLayoutCreateInfo->flags = pipelineLayout[STR_FOSSILIZE_NODE_FLAGS].GetUint();
            pRgaPipelineCreateInfo->m_pVkPipelineLayoutCreateInfo->setLayoutCount = descSetLayoutIndicesCount;
            pRgaPipelineCreateInfo->m_pVkPipelineLayoutCreateInfo->pushConstantRangeCount = pushConstantRangesCount;
            pRgaPipelineCreateInfo->m_pVkPipelineLayoutCreateInfo->pPushConstantRanges = pRgaPushConstantRanges;

            // Descriptor set layouts - add the relevant indices.
            for (const auto& setLayout : pipelineLayout[STR_FOSSILIZE_NODE_SET_LAYOUTS].GetArray())
            {
                pRgaPipelineCreateInfo->m_descriptorSetLayoutIndices.push_back(setLayout.GetUint());
            }

            // Add this instance to our collection.
            rgaPipelineLayouts.push_back(pRgaPipelineCreateInfo);
        }
    }

    // Shader modules: we get them as an argument, so we don't need to process that node. Skip to render passes.

    // Render passes.
    std::vector<VkRenderPassCreateInfo*> rgaRenderPasses;
    assert(ret = ret && doc.HasMember(STR_FOSSILIZE_NODE_RENDER_PASSES));
    assert(ret = ret && doc[STR_FOSSILIZE_NODE_RENDER_PASSES].IsArray());

    for (const auto& renderPass : doc[STR_FOSSILIZE_NODE_RENDER_PASSES].GetArray())
    {
        // Create and reset our render pass create info structure.
        VkRenderPassCreateInfo* pRenderPass = new VkRenderPassCreateInfo{};
        pRenderPass->sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

        // Verify that the members exist.
        assert(ret = ret && renderPass.HasMember(STR_FOSSILIZE_NODE_HASH));
        assert(ret = ret && renderPass.HasMember(STR_FOSSILIZE_NODE_FLAGS));
        assert(ret = ret && renderPass.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES));
        assert(ret = ret && renderPass.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTIONS));
        assert(ret = ret && renderPass.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_ATTACHMENTS));

        // Verify the type.
        assert(ret = ret && renderPass[STR_FOSSILIZE_NODE_HASH].IsUint64());
        assert(ret = ret && renderPass[STR_FOSSILIZE_NODE_FLAGS].IsUint());
        assert(ret = ret && renderPass[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES].IsArray());
        assert(ret = ret && renderPass[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTIONS].IsArray());
        assert(ret = ret && renderPass[STR_FOSSILIZE_NODE_RENDER_PASS_ATTACHMENTS].IsArray());

        // Flags.
        pRenderPass->flags = renderPass[STR_FOSSILIZE_NODE_FLAGS].GetUint();

        if (ret)
        {
            const uint32_t dependencyCount = renderPass[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES].GetArray().Size();
            if (dependencyCount > 0)
            {
                VkSubpassDependency* pSubpassDependency = new VkSubpassDependency[dependencyCount]{};

                size_t index = 0;
                // Dependencies.
                for (const auto& subpassDependency : renderPass[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES].GetArray())
                {
                    // Verify that the members exist.
                    assert(ret = ret && subpassDependency.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_FLAGS));
                    assert(ret = ret && subpassDependency.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_DST_ACCESS_MASK));
                    assert(ret = ret && subpassDependency.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_DST_STAGE_MASK));
                    assert(ret = ret && subpassDependency.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_DST_SUBPASS));
                    assert(ret = ret && subpassDependency.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_SRC_ACCESS_MASK));
                    assert(ret = ret && subpassDependency.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_SRC_STAGE_MASK));
                    assert(ret = ret && subpassDependency.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_SRC_SUBPASS));

                    // Verify the type.
                    assert(ret = ret && subpassDependency[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_FLAGS].IsUint());
                    assert(ret = ret && subpassDependency[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_DST_ACCESS_MASK].IsUint());
                    assert(ret = ret && subpassDependency[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_DST_STAGE_MASK].IsUint());
                    assert(ret = ret && subpassDependency[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_DST_SUBPASS].IsUint());
                    assert(ret = ret && subpassDependency[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_SRC_ACCESS_MASK].IsUint());
                    assert(ret = ret && subpassDependency[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_SRC_STAGE_MASK].IsUint());
                    assert(ret = ret && subpassDependency[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_SRC_SUBPASS].IsUint());

                    // Fill-up our structure.
                    pSubpassDependency[index].dependencyFlags = static_cast<VkDependencyFlags>(subpassDependency[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_FLAGS].GetUint());
                    pSubpassDependency[index].dstAccessMask = static_cast<VkAccessFlags>(subpassDependency[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_DST_ACCESS_MASK].GetUint());
                    pSubpassDependency[index].srcAccessMask = static_cast<VkAccessFlags>(subpassDependency[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_SRC_ACCESS_MASK].GetUint());
                    pSubpassDependency[index].dstStageMask = static_cast<VkAccessFlags>(subpassDependency[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_DST_STAGE_MASK].GetUint());
                    pSubpassDependency[index].srcStageMask = static_cast<VkPipelineStageFlags>(subpassDependency[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_SRC_STAGE_MASK].GetUint());
                    pSubpassDependency[index].dstSubpass = subpassDependency[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_DST_SUBPASS].GetUint();
                    pSubpassDependency[index].srcSubpass = subpassDependency[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DEPENDENCIES_SRC_SUBPASS].GetUint();

                    // Move to the next item in our array.
                    ++index;
                }

                // Add the dependency info to our render pass structure.
                pRenderPass->dependencyCount = dependencyCount;
                pRenderPass->pDependencies = pSubpassDependency;

                // Attachments.
                const uint32_t attachmentCount = renderPass[STR_FOSSILIZE_NODE_RENDER_PASS_ATTACHMENTS].GetArray().Size();
                if (attachmentCount > 0)
                {
                    VkAttachmentDescription* pAttachments = new VkAttachmentDescription[attachmentCount]{};

                    index = 0;
                    for (const auto& attachment : renderPass[STR_FOSSILIZE_NODE_RENDER_PASS_ATTACHMENTS].GetArray())
                    {
                        // Verify that the members exist.
                        assert(ret = ret && attachment.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_FLAGS));
                        assert(ret = ret && attachment.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_FORMAT));
                        assert(ret = ret && attachment.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_FINAL_LAYOUT));
                        assert(ret = ret && attachment.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_INITIAL_LAYOUT));
                        assert(ret = ret && attachment.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_LOAD_OP));
                        assert(ret = ret && attachment.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_STORE_OP));
                        assert(ret = ret && attachment.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_SAMPLES));
                        assert(ret = ret && attachment.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_STENCIL_LOAD_OP));
                        assert(ret = ret && attachment.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_STENCIL_STORE_OP));

                        // Verify the type.
                        assert(ret = ret && attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_FLAGS].IsUint());
                        assert(ret = ret && attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_FORMAT].IsUint());
                        assert(ret = ret && attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_FINAL_LAYOUT].IsUint());
                        assert(ret = ret && attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_INITIAL_LAYOUT].IsUint());
                        assert(ret = ret && attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_LOAD_OP].IsUint());
                        assert(ret = ret && attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_STORE_OP].IsUint());
                        assert(ret = ret && attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_SAMPLES].IsUint());
                        assert(ret = ret && attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_STENCIL_LOAD_OP].IsUint());
                        assert(ret = ret && attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_STENCIL_STORE_OP].IsUint());

                        // Fill-up our structure.
                        pAttachments[index].flags = static_cast<VkFlags>(attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_FLAGS].GetUint());
                        pAttachments[index].format = static_cast<VkFormat>(attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_FORMAT].GetUint());
                        pAttachments[index].finalLayout = static_cast<VkImageLayout>(attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_FINAL_LAYOUT].GetUint());
                        pAttachments[index].initialLayout = static_cast<VkImageLayout>(attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_INITIAL_LAYOUT].GetUint());
                        pAttachments[index].loadOp = static_cast<VkAttachmentLoadOp>(attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_LOAD_OP].GetUint());
                        pAttachments[index].storeOp = static_cast<VkAttachmentStoreOp>(attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_STORE_OP].GetUint());
                        pAttachments[index].samples = static_cast<VkSampleCountFlagBits>(attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_SAMPLES].GetUint());
                        pAttachments[index].stencilLoadOp = static_cast<VkAttachmentLoadOp>(attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_STENCIL_LOAD_OP].GetUint());
                        pAttachments[index].stencilStoreOp = static_cast<VkAttachmentStoreOp>(attachment[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_ATTACHMENTS_STENCIL_STORE_OP].GetUint());

                        // Move to the next item in our array.
                        ++index;
                    }

                    // Add the attachment info to our render pass structure.
                    pRenderPass->attachmentCount = attachmentCount;
                    pRenderPass->pAttachments = pAttachments;

                    // Sub-pass descriptions.
                    const uint32_t subpassDescriptionCount = renderPass[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTIONS].GetArray().Size();
                    if (subpassDescriptionCount > 0)
                    {
                        VkSubpassDescription* pDescrtipions = new VkSubpassDescription[subpassDescriptionCount]{};

                        // Add the  sub-pass description items.
                        index = 0;
                        for (const auto& description : renderPass[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTIONS].GetArray())
                        {
                            // Verify that the members exist.
                            assert(ret = ret && description.HasMember(STR_FOSSILIZE_NODE_FLAGS));
                            assert(ret = ret && description.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTION_PIPELINE_BIND_POINT));
                            assert(ret = ret && description.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTION_COLOR_ATTACHMENTS));
                            assert(ret = ret && description.HasMember(STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTION_DEPTH_STENCIL_ATTACHMENTS));

                            // Verify the type.
                            assert(ret = ret && description[STR_FOSSILIZE_NODE_FLAGS].IsUint());
                            assert(ret = ret && description[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTION_PIPELINE_BIND_POINT].IsUint());
                            assert(ret = ret && description[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTION_COLOR_ATTACHMENTS].IsArray());
                            assert(ret = ret && description[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTION_DEPTH_STENCIL_ATTACHMENTS].IsObject());

                            // Fill up our structure.
                            pDescrtipions[index].flags = description[STR_FOSSILIZE_NODE_FLAGS].GetUint();
                            pDescrtipions[index].pipelineBindPoint = static_cast<VkPipelineBindPoint>(description[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTION_PIPELINE_BIND_POINT].GetUint());

                            // Color attachments.
                            uint32_t numColorAttachments = description[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTION_COLOR_ATTACHMENTS].GetArray().Size();

                            if (numColorAttachments)
                            {
                                VkAttachmentReference* pColorAttachments = new VkAttachmentReference[numColorAttachments]{};

                                // Add the color attachment items.
                                uint32_t colorAttachmentIndex = 0;
                                for (const auto& colorAttachment : description[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTION_COLOR_ATTACHMENTS].GetArray())
                                {
                                    // Verify that the members exist.
                                    assert(ret = ret && colorAttachment.HasMember(STR_FOSSILIZE_NODE_ATTACHMENT_REFERENCE_ATTACHMENT));
                                    assert(ret = ret && colorAttachment.HasMember(STR_FOSSILIZE_NODE_ATTACHMENT_REFERENCE_LAYOUT));

                                    // Verify the type.
                                    assert(ret = ret && colorAttachment[STR_FOSSILIZE_NODE_ATTACHMENT_REFERENCE_ATTACHMENT].IsUint());
                                    assert(ret = ret && colorAttachment[STR_FOSSILIZE_NODE_ATTACHMENT_REFERENCE_LAYOUT].IsUint());

                                    // Fill up our structure.
                                    pColorAttachments[colorAttachmentIndex].attachment = colorAttachment[STR_FOSSILIZE_NODE_ATTACHMENT_REFERENCE_ATTACHMENT].GetUint();
                                    pColorAttachments[colorAttachmentIndex].layout = static_cast<VkImageLayout>(colorAttachment[STR_FOSSILIZE_NODE_ATTACHMENT_REFERENCE_LAYOUT].GetUint());

                                    ++colorAttachmentIndex;
                                }

                                // Add the color attachments to the sub-pass description.
                                pDescrtipions[index].colorAttachmentCount = numColorAttachments;
                                pDescrtipions[index].pColorAttachments = pColorAttachments;
                            }

                            // Depth-stencil attachment.
                            const auto& depthStencilObj = description[STR_FOSSILIZE_NODE_RENDER_PASS_SUB_PASS_DESCRIPTION_DEPTH_STENCIL_ATTACHMENTS].GetObject();

                            // Verify that the members exist.
                            assert(ret = ret && depthStencilObj.HasMember(STR_FOSSILIZE_NODE_ATTACHMENT_REFERENCE_ATTACHMENT));
                            assert(ret = ret && depthStencilObj.HasMember(STR_FOSSILIZE_NODE_ATTACHMENT_REFERENCE_LAYOUT));

                            // Verify the type.
                            assert(ret = ret && depthStencilObj[STR_FOSSILIZE_NODE_ATTACHMENT_REFERENCE_ATTACHMENT].IsUint());
                            assert(ret = ret && depthStencilObj[STR_FOSSILIZE_NODE_ATTACHMENT_REFERENCE_LAYOUT].IsUint());

                            VkAttachmentReference* pDepthStencilAttachment = new VkAttachmentReference{};

                            // Fill up our structure.
                            pDepthStencilAttachment->attachment = depthStencilObj[STR_FOSSILIZE_NODE_ATTACHMENT_REFERENCE_ATTACHMENT].GetUint();
                            pDepthStencilAttachment->layout = static_cast<VkImageLayout>(depthStencilObj[STR_FOSSILIZE_NODE_ATTACHMENT_REFERENCE_LAYOUT].GetUint());

                            // Add the depth stencil attachment to the current sub pass description.
                            pDescrtipions[index].pDepthStencilAttachment = pDepthStencilAttachment;

                            // Move to the next sub pass description item in our array.
                            ++index;
                        }

                        // Set the sub-pass description count and items to our render pass create info structure.
                        pRenderPass->subpassCount = subpassDescriptionCount;
                        pRenderPass->pSubpasses = pDescrtipions;

                        // We are done filling up our render pass create info structure.
                        // Now, let's add it to our render pass collection.
                        rgaRenderPasses.push_back(pRenderPass);
                    }
                }
            }
        }
    }

    // Graphics pipelines.
    std::vector<rgPsoGraphicsVulkan*> rgaGraphicsPipelineRecipes;
    assert(ret = ret && doc.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINES));
    assert(ret = ret && doc[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINES].IsArray());

    // Track the index of the current graphics pipeline.
    uint32_t graphicsPipelineIndex = 0;

    for (const auto& graphicsPipeline : doc[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINES].GetArray())
    {
        // Create and reset our graphics pipeline create info structure.
        rgPsoGraphicsVulkan* pGraphicsPipelineRecipe = new rgPsoGraphicsVulkan{};

        // Verify that the members exist.
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_HASH));
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_FLAGS));
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_BASE_PIPELINE_HANDLE));
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_BASE_PIPELINE_INDEX));
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_LAYOUT));
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_RENDER_PASS));
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_SUB_PASS));
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_DYNAMIC_STATE));
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_MULTISAMPLE_STATE));
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_VERTEX_INPUT_STATE));
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_RASTERIZATION_STATE));
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_INPUT_ASSEMBLY_STATE));
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_COLOR_BLEND_STATE));
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_VIEWPORT_STATE));
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_DEPTH_STENCIL_STATE));
        assert(ret = ret && graphicsPipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_STAGES));

        // Verify the type.
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_HASH].IsUint64());
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_FLAGS].IsUint());
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_BASE_PIPELINE_HANDLE].IsUint());
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_BASE_PIPELINE_INDEX].IsInt());
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_LAYOUT].IsUint());
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_RENDER_PASS].IsUint());
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_SUB_PASS].IsUint());
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_DYNAMIC_STATE].IsObject());
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_MULTISAMPLE_STATE].IsObject());
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_VERTEX_INPUT_STATE].IsObject());
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_RASTERIZATION_STATE].IsObject());
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_INPUT_ASSEMBLY_STATE].IsObject());
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_COLOR_BLEND_STATE].IsObject());
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_VIEWPORT_STATE].IsObject());
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_DEPTH_STENCIL_STATE].IsObject());
        assert(ret = ret && graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_STAGES].IsArray());

        // Fill up our structure.
        VkGraphicsPipelineCreateInfo* pGraphicsPiplineCreateInfo = pGraphicsPipelineRecipe->GetGraphicsPipelineCreateInfo();
        assert(pGraphicsPiplineCreateInfo != nullptr);
        if (pGraphicsPiplineCreateInfo != nullptr)
        {
            pGraphicsPiplineCreateInfo->flags = static_cast<VkFlags>(graphicsPipeline[STR_FOSSILIZE_NODE_FLAGS].GetUint());

            // @TODO: figure out what should we do with this one. Is this a handle or an internal reference within the JSON.
            pGraphicsPiplineCreateInfo->sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pGraphicsPiplineCreateInfo->basePipelineHandle = reinterpret_cast<VkPipeline>(graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_BASE_PIPELINE_HANDLE].GetInt64());
            pGraphicsPiplineCreateInfo->basePipelineIndex = static_cast<VkFlags>(graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_BASE_PIPELINE_INDEX].GetInt());
            pGraphicsPiplineCreateInfo->subpass = graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_SUB_PASS].GetUint();

            // Pipeline layouts and descriptor set layouts.
            // Get the pipeline layout index within our collection.
            uint32_t pipelineLayoutIndex = graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_LAYOUT].GetUint();
            assert(pipelineLayoutIndex <= rgaPipelineLayouts.size());
            if (pipelineLayoutIndex > 0)
            {
                assert(rgaPipelineLayouts[pipelineLayoutIndex - 1] != nullptr);
                if (rgaPipelineLayouts[pipelineLayoutIndex - 1] != nullptr)
                {
                    VkPipelineLayoutCreateInfo* pPipelineLayoutCreateInfo = rgaPipelineLayouts[pipelineLayoutIndex - 1]->m_pVkPipelineLayoutCreateInfo;
                    assert(pPipelineLayoutCreateInfo != nullptr);
                    pGraphicsPipelineRecipe->SetPipelineLayoutCreateInfo(pPipelineLayoutCreateInfo);

                    // Add the descriptor set layout create info structures.
                    for (size_t descriptorSetLayoutIndex : rgaPipelineLayouts[pipelineLayoutIndex - 1]->m_descriptorSetLayoutIndices)
                    {
                        assert(descriptorSetLayoutIndex <= rgaDescriptorSetLayouts.size());
                        if (descriptorSetLayoutIndex <= rgaDescriptorSetLayouts.size())
                        {
                            pGraphicsPipelineRecipe->AddDescriptorSetLayoutCreateInfo(rgaDescriptorSetLayouts[descriptorSetLayoutIndex - 1]);
                        }
                    }
                }
            }

            // Retrieve the render pass index.
            uint32_t renderPassIndex = graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_RENDER_PASS].GetUint();
            if (renderPassIndex > 0)
            {
                // Verify that we are not going out of bounds.
                assert(renderPassIndex <= rgaRenderPasses.size());

                // The index in our vector would always be -1.
                VkRenderPassCreateInfo* pRenderPass = rgaRenderPasses[renderPassIndex - 1];
                assert(pRenderPass != nullptr);
                pGraphicsPipelineRecipe->SetRenderPassCreateInfo(pRenderPass);
            }

            // Dynamic state.
            const auto& dynamicStateObj = graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_DYNAMIC_STATE].GetObject();

            // Verify that the members exist.
            assert(ret = ret && dynamicStateObj.HasMember(STR_FOSSILIZE_NODE_FLAGS));
            assert(ret = ret && dynamicStateObj.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_DYNAMIC_STATE));

            // Verify the type.
            assert(ret = ret && dynamicStateObj[STR_FOSSILIZE_NODE_FLAGS].IsUint());
            assert(ret = ret && dynamicStateObj[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_DYNAMIC_STATE].IsArray());

            uint32_t dynamicStateCount = dynamicStateObj[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_DYNAMIC_STATE].GetArray().Size();
            if (dynamicStateCount > 0)
            {
                // Allocate our dynamic state create info.
                VkPipelineDynamicStateCreateInfo* pDynamicStateCreateInfo = new VkPipelineDynamicStateCreateInfo{};
                pDynamicStateCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                pDynamicStateCreateInfo->flags = static_cast<VkPipelineDynamicStateCreateFlags>(dynamicStateObj[STR_FOSSILIZE_NODE_FLAGS].GetUint());
                pDynamicStateCreateInfo->dynamicStateCount = dynamicStateCount;

                // Create an array of dynamic states.
                VkDynamicState* pDynamicStates = new VkDynamicState[dynamicStateCount]{};

                // The index within our array.
                size_t dynamicStateIndex = 0;

                for (const auto& dynamicState : dynamicStateObj[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_DYNAMIC_STATE].GetArray())
                {
                    VkDynamicState diValue = static_cast<VkDynamicState>(dynamicState.GetUint());
                    pDynamicStates[dynamicStateIndex++] = diValue;
                }

                // Add the dynamic states array to our create info object.
                pDynamicStateCreateInfo->pDynamicStates = pDynamicStates;

                // Add the dynamic state create info to the graphics pipeline create info.
                pGraphicsPiplineCreateInfo->pDynamicState = pDynamicStateCreateInfo;
            }

            // Multisample state.
            const auto& multiSampleStateObj = graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_MULTISAMPLE_STATE].GetObject();
            VkPipelineMultisampleStateCreateInfo* pMultiSampleStateCreateInfo = new VkPipelineMultisampleStateCreateInfo{};
            pMultiSampleStateCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

            // Verify that the members exist.
            assert(ret = ret && multiSampleStateObj.HasMember(STR_FOSSILIZE_NODE_FLAGS));
            assert(ret = ret && multiSampleStateObj.HasMember(STR_FOSSILIZE_NODE_MULTI_SAMPLE_STATE_RASTERIZATION_SAMPLES));
            assert(ret = ret && multiSampleStateObj.HasMember(STR_FOSSILIZE_NODE_MULTI_SAMPLE_SAMPLE_SHADING_ENABLE));
            assert(ret = ret && multiSampleStateObj.HasMember(STR_FOSSILIZE_NODE_MULTI_SAMPLE_MIN_SAMPLE_SHADING));
            assert(ret = ret && multiSampleStateObj.HasMember(STR_FOSSILIZE_NODE_MULTI_ALPHA_TO_ONE_ENABLE));
            assert(ret = ret && multiSampleStateObj.HasMember(STR_FOSSILIZE_NODE_MULTI_ALPHA_TO_COVERAGE_ENABLE));

            // Verify the type.
            assert(ret = ret && multiSampleStateObj[STR_FOSSILIZE_NODE_FLAGS].IsUint());
            assert(ret = ret && multiSampleStateObj[STR_FOSSILIZE_NODE_MULTI_SAMPLE_STATE_RASTERIZATION_SAMPLES].IsInt64());
            assert(ret = ret && multiSampleStateObj[STR_FOSSILIZE_NODE_MULTI_SAMPLE_SAMPLE_SHADING_ENABLE].IsUint());
            assert(ret = ret && multiSampleStateObj[STR_FOSSILIZE_NODE_MULTI_SAMPLE_MIN_SAMPLE_SHADING].IsFloat());
            assert(ret = ret && multiSampleStateObj[STR_FOSSILIZE_NODE_MULTI_ALPHA_TO_ONE_ENABLE].IsUint());
            assert(ret = ret && multiSampleStateObj[STR_FOSSILIZE_NODE_MULTI_ALPHA_TO_COVERAGE_ENABLE].IsUint());

            // Fill up our structure.
            pMultiSampleStateCreateInfo->flags = multiSampleStateObj[STR_FOSSILIZE_NODE_FLAGS].GetUint();
            pMultiSampleStateCreateInfo->rasterizationSamples = static_cast<VkSampleCountFlagBits>(multiSampleStateObj[STR_FOSSILIZE_NODE_MULTI_SAMPLE_STATE_RASTERIZATION_SAMPLES].GetUint());
            pMultiSampleStateCreateInfo->sampleShadingEnable = multiSampleStateObj[STR_FOSSILIZE_NODE_MULTI_SAMPLE_SAMPLE_SHADING_ENABLE].GetUint();
            pMultiSampleStateCreateInfo->minSampleShading = multiSampleStateObj[STR_FOSSILIZE_NODE_MULTI_SAMPLE_MIN_SAMPLE_SHADING].GetFloat();
            pMultiSampleStateCreateInfo->alphaToOneEnable = multiSampleStateObj[STR_FOSSILIZE_NODE_MULTI_ALPHA_TO_ONE_ENABLE].GetUint();
            pMultiSampleStateCreateInfo->alphaToCoverageEnable = multiSampleStateObj[STR_FOSSILIZE_NODE_MULTI_ALPHA_TO_COVERAGE_ENABLE].GetUint();

            // Set our recipe's multi sample state create info.
            pGraphicsPipelineRecipe->SetMultisampleStateCreateInfo(pMultiSampleStateCreateInfo);

            // Vertex input state create info.
            const auto& vertexInputState = graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_VERTEX_INPUT_STATE].GetObject();
            VkPipelineVertexInputStateCreateInfo* pVertexInputCreateInfo = new VkPipelineVertexInputStateCreateInfo{};
            pVertexInputCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

            // Verify that the members exist.
            assert(ret = ret && vertexInputState.HasMember(STR_FOSSILIZE_NODE_FLAGS));
            assert(ret = ret && vertexInputState.HasMember(STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES));
            assert(ret = ret && vertexInputState.HasMember(STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDINGS));

            // Verify the type.
            assert(ret = ret && vertexInputState[STR_FOSSILIZE_NODE_FLAGS].IsUint());
            assert(ret = ret && vertexInputState[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES].IsArray());
            assert(ret = ret && vertexInputState[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDINGS].IsArray());

            // Vertex input state attributes descriptions.
            uint32_t attributeDescriptionCount = vertexInputState[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES].GetArray().Size();

            if (attributeDescriptionCount > 0)
            {
                VkVertexInputAttributeDescription* pAttributeDescriptions = new VkVertexInputAttributeDescription[attributeDescriptionCount]{};
                uint32_t attributeIndex = 0;
                for (const auto& attribute : vertexInputState[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES].GetArray())
                {
                    // Verify that the members exist.
                    assert(ret = ret && attribute.HasMember(STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_LOCATION));
                    assert(ret = ret && attribute.HasMember(STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_BINDING));
                    assert(ret = ret && attribute.HasMember(STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_OFFSET));
                    assert(ret = ret && attribute.HasMember(STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_FORMAT));

                    // Verify the type.
                    assert(ret = ret && attribute[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_LOCATION].IsUint());
                    assert(ret = ret && attribute[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_BINDING].IsUint());
                    assert(ret = ret && attribute[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_OFFSET].IsUint());
                    assert(ret = ret && attribute[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_FORMAT].IsUint());

                    // Fill up our structure.
                    pAttributeDescriptions[attributeIndex].location = attribute[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_LOCATION].GetUint();
                    pAttributeDescriptions[attributeIndex].binding = attribute[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_BINDING].GetUint();
                    pAttributeDescriptions[attributeIndex].offset = attribute[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_OFFSET].GetUint();
                    pAttributeDescriptions[attributeIndex].format = static_cast<VkFormat>(attribute[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_ATTRIBUTES_FORMAT].GetUint());

                    // Move to the next item.
                    ++attributeIndex;
                }

                // Assign to our vertex input create info structure.
                pVertexInputCreateInfo->vertexAttributeDescriptionCount = attributeDescriptionCount;
                pVertexInputCreateInfo->pVertexAttributeDescriptions = pAttributeDescriptions;
            }

            // Vertex input state binding descriptions.
            uint32_t bindingDescriptionCount = vertexInputState[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDINGS].GetArray().Size();

            if (bindingDescriptionCount > 0)
            {
                VkVertexInputBindingDescription* pBindingDescriptions = new VkVertexInputBindingDescription[bindingDescriptionCount]{};
                uint32_t bindingIndex = 0;
                for (const auto& binding : vertexInputState[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDINGS].GetArray())
                {
                    // Verify that the members exist.
                    assert(ret = ret && binding.HasMember(STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDING_BINDING));
                    assert(ret = ret && binding.HasMember(STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDING_STRIDE));
                    assert(ret = ret && binding.HasMember(STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDING_INPUT_RATE));

                    // Verify the type.
                    assert(ret = ret && binding[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDING_BINDING].IsUint());
                    assert(ret = ret && binding[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDING_STRIDE].IsUint());
                    assert(ret = ret && binding[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDING_INPUT_RATE].IsUint());

                    // Fill up our structure.
                    pBindingDescriptions[bindingIndex].binding = binding[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDING_BINDING].GetUint();
                    pBindingDescriptions[bindingIndex].stride = binding[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDING_STRIDE].GetUint();
                    pBindingDescriptions[bindingIndex].inputRate = static_cast<VkVertexInputRate>(binding[STR_FOSSILIZE_NODE_MULTI_VERTEX_INPUT_STATE_BINDING_INPUT_RATE].GetUint());

                    // Move to the next item.
                    ++bindingIndex;
                }

                // Assign to our vertex input create info structure.
                pVertexInputCreateInfo->vertexBindingDescriptionCount = bindingDescriptionCount;
                pVertexInputCreateInfo->pVertexBindingDescriptions = pBindingDescriptions;
            }

            // Add the vertex input create info to our recipe object.
            pGraphicsPipelineRecipe->SetVertexInputStateCreateInfo(pVertexInputCreateInfo);

            // Rasterization state.
            const auto& rasterizationStateObj = graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_RASTERIZATION_STATE].GetObject();
            VkPipelineRasterizationStateCreateInfo* pRasterizationStateCreateInfo = new VkPipelineRasterizationStateCreateInfo{};
            pRasterizationStateCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

            // Verify that the members exist.
            assert(ret = ret && rasterizationStateObj.HasMember(STR_FOSSILIZE_NODE_FLAGS));
            assert(ret = ret && rasterizationStateObj.HasMember(STR_FOSSILIZE_NODE_MULTI_RASTERIZATION_STATE_DEPTH_BIAS_CONSTANT_FACTOR));
            assert(ret = ret && rasterizationStateObj.HasMember(STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_BIAS_SLOPE_FACTOR));
            assert(ret = ret && rasterizationStateObj.HasMember(STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_BIAS_CLAMP));
            assert(ret = ret && rasterizationStateObj.HasMember(STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_BIAS_ENABLE));
            assert(ret = ret && rasterizationStateObj.HasMember(STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_CLAMP_ENABLE));
            assert(ret = ret && rasterizationStateObj.HasMember(STR_FOSSILIZE_NODE_RASTERIZATION_STATE_POLYGON_MODE));
            assert(ret = ret && rasterizationStateObj.HasMember(STR_FOSSILIZE_NODE_RASTERIZATION_STATE_RASTERIZER_DISCARD_ENABLE));
            assert(ret = ret && rasterizationStateObj.HasMember(STR_FOSSILIZE_NODE_RASTERIZATION_STATE_FRONT_FACE));
            assert(ret = ret && rasterizationStateObj.HasMember(STR_FOSSILIZE_NODE_RASTERIZATION_STATE_LINE_WIDTH));
            assert(ret = ret && rasterizationStateObj.HasMember(STR_FOSSILIZE_NODE_RASTERIZATION_STATE_CULL_MDOE));

            // Verify the type.
            assert(ret = ret && rasterizationStateObj[STR_FOSSILIZE_NODE_FLAGS].IsUint());
            assert(ret = ret && rasterizationStateObj[STR_FOSSILIZE_NODE_MULTI_RASTERIZATION_STATE_DEPTH_BIAS_CONSTANT_FACTOR].IsFloat());
            assert(ret = ret && rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_BIAS_SLOPE_FACTOR].IsFloat());
            assert(ret = ret && rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_BIAS_CLAMP].IsFloat());
            assert(ret = ret && rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_BIAS_ENABLE].IsUint());
            assert(ret = ret && rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_CLAMP_ENABLE].IsUint());
            assert(ret = ret && rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_POLYGON_MODE].IsUint());
            assert(ret = ret && rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_RASTERIZER_DISCARD_ENABLE].IsUint());
            assert(ret = ret && rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_FRONT_FACE].IsUint());
            assert(ret = ret && rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_LINE_WIDTH].IsFloat());
            assert(ret = ret && rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_CULL_MDOE].IsUint());

            // Fill up our structure.
            pRasterizationStateCreateInfo->flags = rasterizationStateObj[STR_FOSSILIZE_NODE_FLAGS].GetUint();
            pRasterizationStateCreateInfo->depthBiasConstantFactor = rasterizationStateObj[STR_FOSSILIZE_NODE_MULTI_RASTERIZATION_STATE_DEPTH_BIAS_CONSTANT_FACTOR].GetFloat();
            pRasterizationStateCreateInfo->depthBiasSlopeFactor = rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_BIAS_SLOPE_FACTOR].GetFloat();
            pRasterizationStateCreateInfo->depthBiasClamp = rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_BIAS_CLAMP].GetFloat();
            pRasterizationStateCreateInfo->depthBiasEnable = rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_BIAS_ENABLE].GetUint();
            pRasterizationStateCreateInfo->depthClampEnable = rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_DEPTH_CLAMP_ENABLE].GetUint();
            pRasterizationStateCreateInfo->polygonMode = static_cast<VkPolygonMode>(rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_POLYGON_MODE].GetUint());
            pRasterizationStateCreateInfo->rasterizerDiscardEnable = rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_RASTERIZER_DISCARD_ENABLE].GetUint();
            pRasterizationStateCreateInfo->frontFace = static_cast<VkFrontFace>(rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_FRONT_FACE].GetUint());
            pRasterizationStateCreateInfo->lineWidth = rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_LINE_WIDTH].GetFloat();
            pRasterizationStateCreateInfo->cullMode = rasterizationStateObj[STR_FOSSILIZE_NODE_RASTERIZATION_STATE_CULL_MDOE].GetUint();

            // Set the relevant recipe object member.
            pGraphicsPipelineRecipe->SetRasterizationStateCreateInfo(pRasterizationStateCreateInfo);
        }

        // Input assembly state.
        const auto& inputAssemblyStateObj = graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_INPUT_ASSEMBLY_STATE].GetObject();
        VkPipelineInputAssemblyStateCreateInfo* pInputAssemblyStateCreateInfo = new VkPipelineInputAssemblyStateCreateInfo{};
        pInputAssemblyStateCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

        // Verify that the members exist.
        assert(ret = ret && inputAssemblyStateObj.HasMember(STR_FOSSILIZE_NODE_FLAGS));
        assert(ret = ret && inputAssemblyStateObj.HasMember(STR_FOSSILIZE_NODE_INPUT_ASSEMBLY_STATE_TOPOLY));
        assert(ret = ret && inputAssemblyStateObj.HasMember(STR_FOSSILIZE_NODE_INPUT_ASSEMBLY_STATE_PRIMITIVE_RESTART_ENABLE));

        // Verify the type.
        assert(ret = ret && inputAssemblyStateObj[STR_FOSSILIZE_NODE_FLAGS].IsUint());
        assert(ret = ret && inputAssemblyStateObj[STR_FOSSILIZE_NODE_INPUT_ASSEMBLY_STATE_TOPOLY].IsUint());
        assert(ret = ret && inputAssemblyStateObj[STR_FOSSILIZE_NODE_INPUT_ASSEMBLY_STATE_PRIMITIVE_RESTART_ENABLE].IsUint());

        // Fill up our structure.
        pInputAssemblyStateCreateInfo->flags = static_cast<VkPipelineVertexInputStateCreateFlags>(inputAssemblyStateObj[STR_FOSSILIZE_NODE_FLAGS].GetUint());
        pInputAssemblyStateCreateInfo->topology = static_cast<VkPrimitiveTopology>(inputAssemblyStateObj[STR_FOSSILIZE_NODE_INPUT_ASSEMBLY_STATE_TOPOLY].GetUint());
        pInputAssemblyStateCreateInfo->primitiveRestartEnable = static_cast<VkPipelineVertexInputStateCreateFlags>(inputAssemblyStateObj[STR_FOSSILIZE_NODE_INPUT_ASSEMBLY_STATE_PRIMITIVE_RESTART_ENABLE].GetUint());

        // Set the input assembly state create info to our recipe structure.
        pGraphicsPipelineRecipe->SetInputAssemblyStateCreateInfo(pInputAssemblyStateCreateInfo);

        // Color blend state.
        const auto& colorBlendStateObj = graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_COLOR_BLEND_STATE].GetObject();
        VkPipelineColorBlendStateCreateInfo* pColorBlendStateCreateInfo = new VkPipelineColorBlendStateCreateInfo{};
        pColorBlendStateCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

        // Verify that the members exist.
        assert(ret = ret && colorBlendStateObj.HasMember(STR_FOSSILIZE_NODE_FLAGS));
        assert(ret = ret && colorBlendStateObj.HasMember(STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_LOGIC_OP));
        assert(ret = ret && colorBlendStateObj.HasMember(STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_LOGIC_OP_ENABLE));
        assert(ret = ret && colorBlendStateObj.HasMember(STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_BLEND_CONSTANTS));
        assert(ret = ret && colorBlendStateObj.HasMember(STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_ATTACHMENTS));

        // Verify the type.
        assert(ret = ret && colorBlendStateObj[STR_FOSSILIZE_NODE_FLAGS].IsUint());
        assert(ret = ret && colorBlendStateObj[STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_LOGIC_OP].IsUint());
        assert(ret = ret && colorBlendStateObj[STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_LOGIC_OP_ENABLE].IsUint());
        assert(ret = ret && colorBlendStateObj[STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_BLEND_CONSTANTS].IsArray());
        assert(ret = ret && colorBlendStateObj[STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_ATTACHMENTS].IsArray());

        // Fill up our structure.
        pColorBlendStateCreateInfo->flags = static_cast<VkPipelineVertexInputStateCreateFlags>(colorBlendStateObj[STR_FOSSILIZE_NODE_FLAGS].GetUint());
        pColorBlendStateCreateInfo->logicOp = static_cast<VkLogicOp>(colorBlendStateObj[STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_LOGIC_OP].GetUint());
        pColorBlendStateCreateInfo->logicOpEnable = colorBlendStateObj[STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_LOGIC_OP_ENABLE].GetUint();

        // Blend constants.
        const auto& blendConstants = colorBlendStateObj[STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_BLEND_CONSTANTS].GetArray();
        uint32_t blendConstantCount = blendConstants.Size();
        const int BLEND_CONSTANT_COUNT = 4;
        assert(blendConstantCount == BLEND_CONSTANT_COUNT);
        if (blendConstantCount == BLEND_CONSTANT_COUNT)
        {
            uint32_t blendConstantIndex = 0;
            for (const auto& blendConstant : blendConstants)
            {
                pColorBlendStateCreateInfo->blendConstants[blendConstantIndex++] = blendConstant.GetFloat();
            }
        }

        // Color blend attachments.
        const auto& colorBlendAttachments = colorBlendStateObj[STR_FOSSILIZE_NODE_COLOR_BLEND_STATE_ATTACHMENTS].GetArray();
        uint32_t colorBlendAttachmentCount = colorBlendAttachments.Size();
        if (colorBlendAttachmentCount > 0)
        {
            uint32_t colorBlendAttachmentIndex = 0;
            VkPipelineColorBlendAttachmentState* pColorBlendAttachement = new VkPipelineColorBlendAttachmentState[colorBlendAttachmentCount]{};
            for (const auto& colorBlendAttachment : colorBlendAttachments)
            {
                // Verify that the members exist.
                assert(ret = ret && colorBlendAttachment.HasMember(STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_DST_ALPHA_BLEND_FACTOR));
                assert(ret = ret && colorBlendAttachment.HasMember(STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_SRC_ALPHA_BLEND_FACTOR));
                assert(ret = ret && colorBlendAttachment.HasMember(STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_DST_COLOR_BLEND_FACTOR));
                assert(ret = ret && colorBlendAttachment.HasMember(STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_SRC_COLOR_BLEND_FACTOR));
                assert(ret = ret && colorBlendAttachment.HasMember(STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_COLOR_WRITE_MASK));
                assert(ret = ret && colorBlendAttachment.HasMember(STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_ALPHA_BLEND_OP));
                assert(ret = ret && colorBlendAttachment.HasMember(STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_COLOR_BLEND_OP));
                assert(ret = ret && colorBlendAttachment.HasMember(STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_BLEND_ENABLE));

                // Verify the type.
                assert(ret = ret && colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_DST_ALPHA_BLEND_FACTOR].IsUint());
                assert(ret = ret && colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_SRC_ALPHA_BLEND_FACTOR].IsUint());
                assert(ret = ret && colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_DST_COLOR_BLEND_FACTOR].IsUint());
                assert(ret = ret && colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_SRC_COLOR_BLEND_FACTOR].IsUint());
                assert(ret = ret && colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_COLOR_WRITE_MASK].IsUint());
                assert(ret = ret && colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_ALPHA_BLEND_OP].IsUint());
                assert(ret = ret && colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_COLOR_BLEND_OP].IsUint());
                assert(ret = ret && colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_BLEND_ENABLE].IsUint());

                // Fill up our structure.
                pColorBlendAttachement[colorBlendAttachmentIndex].dstAlphaBlendFactor = static_cast<VkBlendFactor>(colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_DST_ALPHA_BLEND_FACTOR].GetUint());
                pColorBlendAttachement[colorBlendAttachmentIndex].srcAlphaBlendFactor = static_cast<VkBlendFactor>(colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_SRC_ALPHA_BLEND_FACTOR].GetUint());
                pColorBlendAttachement[colorBlendAttachmentIndex].dstColorBlendFactor = static_cast<VkBlendFactor>(colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_DST_COLOR_BLEND_FACTOR].GetUint());
                pColorBlendAttachement[colorBlendAttachmentIndex].srcColorBlendFactor = static_cast<VkBlendFactor>(colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_SRC_COLOR_BLEND_FACTOR].GetUint());
                pColorBlendAttachement[colorBlendAttachmentIndex].colorWriteMask = static_cast<VkColorComponentFlags>(colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_COLOR_WRITE_MASK].GetUint());
                pColorBlendAttachement[colorBlendAttachmentIndex].alphaBlendOp = static_cast<VkBlendOp>(colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_ALPHA_BLEND_OP].GetUint());
                pColorBlendAttachement[colorBlendAttachmentIndex].colorBlendOp = static_cast<VkBlendOp>(colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_COLOR_BLEND_OP].GetUint());
                pColorBlendAttachement[colorBlendAttachmentIndex].blendEnable = colorBlendAttachment[STR_FOSSILIZE_NODE_COLOR_BLEND_ATTACHMENT_BLEND_ENABLE].GetUint();

                // Move to the next item.
                ++colorBlendAttachmentIndex;
            }

            // Assign to our color blend state create info structure.
            pColorBlendStateCreateInfo->attachmentCount = colorBlendAttachmentCount;
            pColorBlendStateCreateInfo->pAttachments = pColorBlendAttachement;

            // Set the relevant member in our recipe object.
            pGraphicsPipelineRecipe->SetColorBlendStateCreateInfo(pColorBlendStateCreateInfo);
        }

        // Viewport state.
        const auto& viewportStateObj = graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_VIEWPORT_STATE].GetObject();
        VkPipelineViewportStateCreateInfo* pViewportState = new VkPipelineViewportStateCreateInfo{};
        pViewportState->sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

        // Verify that the members exist.
        assert(ret = ret && viewportStateObj.HasMember(STR_FOSSILIZE_NODE_FLAGS));
        assert(ret = ret && viewportStateObj.HasMember(STR_FOSSILIZE_NODE_VIEWPORT_STATE_VIEWPORT_COUNT));
        assert(ret = ret && viewportStateObj.HasMember(STR_FOSSILIZE_NODE_VIEWPORT_STATE_SCISSOR_COUNT));

        // Verify the type.
        assert(ret = ret && viewportStateObj[STR_FOSSILIZE_NODE_FLAGS].IsUint());
        assert(ret = ret && viewportStateObj[STR_FOSSILIZE_NODE_VIEWPORT_STATE_VIEWPORT_COUNT].IsUint());
        assert(ret = ret && viewportStateObj[STR_FOSSILIZE_NODE_VIEWPORT_STATE_SCISSOR_COUNT].IsUint());

        // Fill up our structure.
        pViewportState->flags = static_cast<VkPipelineViewportStateCreateFlags>(viewportStateObj[STR_FOSSILIZE_NODE_FLAGS].GetUint());
        pViewportState->viewportCount = viewportStateObj[STR_FOSSILIZE_NODE_VIEWPORT_STATE_VIEWPORT_COUNT].GetUint();
        pViewportState->scissorCount = viewportStateObj[STR_FOSSILIZE_NODE_VIEWPORT_STATE_SCISSOR_COUNT].GetUint();

        // Set the relevant member of our recipe object.
        pGraphicsPipelineRecipe->SetViweportStateCreateInfo(pViewportState);

        // Depth stencil state.
        const auto& depthStencilStateObj = graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_DEPTH_STENCIL_STATE].GetObject();
        VkPipelineDepthStencilStateCreateInfo* pDepthStencilStateCreateInfo = new VkPipelineDepthStencilStateCreateInfo{};
        pDepthStencilStateCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

        // Verify that the members exist.
        assert(ret = ret && depthStencilStateObj.HasMember(STR_FOSSILIZE_NODE_FLAGS));
        assert(ret = ret && depthStencilStateObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_ENABLE));
        assert(ret = ret && depthStencilStateObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_MAX_DEPTH_BOUNDS));
        assert(ret = ret && depthStencilStateObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_MIN_DEPTH_BOUNDS));
        assert(ret = ret && depthStencilStateObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_BOUNDS_TEST_ENABLE));
        assert(ret = ret && depthStencilStateObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_WRITE_ENABLE));
        assert(ret = ret && depthStencilStateObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_TEST_ENABLE));
        assert(ret = ret && depthStencilStateObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_COMPARE_OP));
        assert(ret = ret && depthStencilStateObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_FRONT));
        assert(ret = ret && depthStencilStateObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_BACK));

        // Verify the type.
        assert(ret = ret && depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_ENABLE].IsUint());
        assert(ret = ret && depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_MAX_DEPTH_BOUNDS].IsFloat());
        assert(ret = ret && depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_MIN_DEPTH_BOUNDS].IsFloat());
        assert(ret = ret && depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_BOUNDS_TEST_ENABLE].IsUint());
        assert(ret = ret && depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_WRITE_ENABLE].IsUint());
        assert(ret = ret && depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_TEST_ENABLE].IsUint());
        assert(ret = ret && depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_COMPARE_OP].IsUint());
        assert(ret = ret && depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_FRONT].IsObject());
        assert(ret = ret && depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_BACK].IsObject());

        // Fill up our structure.
        pDepthStencilStateCreateInfo->flags = static_cast<VkPipelineViewportStateCreateFlags>(depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_ENABLE].GetUint());
        pDepthStencilStateCreateInfo->maxDepthBounds = depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_MAX_DEPTH_BOUNDS].GetFloat();
        pDepthStencilStateCreateInfo->minDepthBounds = depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_MIN_DEPTH_BOUNDS].GetFloat();
        pDepthStencilStateCreateInfo->depthBoundsTestEnable = depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_BOUNDS_TEST_ENABLE].GetUint();
        pDepthStencilStateCreateInfo->depthWriteEnable = depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_WRITE_ENABLE].GetUint();
        pDepthStencilStateCreateInfo->depthTestEnable = depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_TEST_ENABLE].GetUint();
        pDepthStencilStateCreateInfo->depthCompareOp = static_cast<VkCompareOp>(depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_DEPTH_COMPARE_OP].GetUint());

        // Front.
        const auto& depthStencilFrontObj = depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_FRONT].GetObject();

        // Verify that the members exist.
        assert(ret = ret && depthStencilFrontObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_COMPARE_OP));
        assert(ret = ret && depthStencilFrontObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_WRITE_MASK));
        assert(ret = ret && depthStencilFrontObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_REFERENCE));
        assert(ret = ret && depthStencilFrontObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_COMPARE_MASK));
        assert(ret = ret && depthStencilFrontObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_PASS_OP));
        assert(ret = ret && depthStencilFrontObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_FAIL_OP));
        assert(ret = ret && depthStencilFrontObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_DEPTH_FAIL_OP));

        // Verify the type.
        assert(ret = ret && depthStencilFrontObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_COMPARE_OP].IsUint());
        assert(ret = ret && depthStencilFrontObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_WRITE_MASK].IsUint());
        assert(ret = ret && depthStencilFrontObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_REFERENCE].IsUint());
        assert(ret = ret && depthStencilFrontObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_COMPARE_MASK].IsUint());
        assert(ret = ret && depthStencilFrontObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_PASS_OP].IsUint());
        assert(ret = ret && depthStencilFrontObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_FAIL_OP].IsUint());
        assert(ret = ret && depthStencilFrontObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_DEPTH_FAIL_OP].IsUint());

        // Fill up our structure.
        pDepthStencilStateCreateInfo->front.compareOp = static_cast<VkCompareOp>(depthStencilFrontObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_COMPARE_OP].GetUint());
        pDepthStencilStateCreateInfo->front.writeMask = depthStencilFrontObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_WRITE_MASK].GetUint();
        pDepthStencilStateCreateInfo->front.reference = depthStencilFrontObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_REFERENCE].GetUint();
        pDepthStencilStateCreateInfo->front.compareMask = depthStencilFrontObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_COMPARE_MASK].GetUint();
        pDepthStencilStateCreateInfo->front.passOp = static_cast<VkStencilOp>(depthStencilFrontObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_PASS_OP].GetUint());
        pDepthStencilStateCreateInfo->front.failOp = static_cast<VkStencilOp>(depthStencilFrontObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_FAIL_OP].GetUint());
        pDepthStencilStateCreateInfo->front.depthFailOp = static_cast<VkStencilOp>(depthStencilFrontObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_DEPTH_FAIL_OP].GetUint());

        // Back.
        const auto& depthStencilBackObj = depthStencilStateObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_BACK].GetObject();

        // Verify that the members exist.
        assert(ret = ret && depthStencilBackObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_COMPARE_OP));
        assert(ret = ret && depthStencilBackObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_WRITE_MASK));
        assert(ret = ret && depthStencilBackObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_REFERENCE));
        assert(ret = ret && depthStencilBackObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_COMPARE_MASK));
        assert(ret = ret && depthStencilBackObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_PASS_OP));
        assert(ret = ret && depthStencilBackObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_FAIL_OP));
        assert(ret = ret && depthStencilBackObj.HasMember(STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_DEPTH_FAIL_OP));

        // Verify the type.
        assert(ret = ret && depthStencilBackObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_COMPARE_OP].IsUint());
        assert(ret = ret && depthStencilBackObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_WRITE_MASK].IsUint());
        assert(ret = ret && depthStencilBackObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_REFERENCE].IsUint());
        assert(ret = ret && depthStencilBackObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_COMPARE_MASK].IsUint());
        assert(ret = ret && depthStencilBackObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_PASS_OP].IsUint());
        assert(ret = ret && depthStencilBackObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_FAIL_OP].IsUint());
        assert(ret = ret && depthStencilBackObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_DEPTH_FAIL_OP].IsUint());

        // Fill up our structure.
        pDepthStencilStateCreateInfo->back.compareOp = static_cast<VkCompareOp>(depthStencilBackObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_COMPARE_OP].GetUint());
        pDepthStencilStateCreateInfo->back.writeMask = depthStencilBackObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_WRITE_MASK].GetUint();
        pDepthStencilStateCreateInfo->back.reference = depthStencilBackObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_REFERENCE].GetUint();
        pDepthStencilStateCreateInfo->back.compareMask = depthStencilBackObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_COMPARE_MASK].GetUint();
        pDepthStencilStateCreateInfo->back.passOp = static_cast<VkStencilOp>(depthStencilBackObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_PASS_OP].GetUint());
        pDepthStencilStateCreateInfo->back.failOp = static_cast<VkStencilOp>(depthStencilBackObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_FAIL_OP].GetUint());
        pDepthStencilStateCreateInfo->back.depthFailOp = static_cast<VkStencilOp>(depthStencilBackObj[STR_FOSSILIZE_NODE_DEPTH_STENCIL_STATE_OP_STATE_DEPTH_FAIL_OP].GetUint());

        // Add the depth stencil state to our recipe object.
        pGraphicsPipelineRecipe->SetDepthStencilStateCreateInfo(pDepthStencilStateCreateInfo);

        // Serialize our pipeline recipe.
        std::stringstream outputFileName;
        outputFileName << outputDirectory << "/" << STR_FILE_BASE_FILENAME_PSO << ++graphicsPipelineIndex;

        std::string baseOutputFileName = outputFileName.str().c_str();
        std::string psoOutputFileName = outputFileName.str().c_str();
        psoOutputFileName.append(STR_FILE_EXTENSION_PSO);

        std::string errMsg;
        bool isGraphicsPipelineSerialized = rgPsoSerializerVulkan::WriteStructureToFile(pGraphicsPipelineRecipe, psoOutputFileName, errMsg);
        assert(isGraphicsPipelineSerialized);

        // Add the file to the output buffer.
        if (isGraphicsPipelineSerialized)
        {
            rgaPsoFiles.push_back(outputFileName.str().c_str());

            // Serialize the graphics pipeline's shaders.
            for (const auto& shaderStage : graphicsPipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_STAGES].GetArray())
            {
                // Verify that the members exist.
                assert(ret = ret && shaderStage.HasMember(STR_FOSSILIZE_NODE_FLAGS));
                assert(ret = ret && shaderStage.HasMember(STR_FOSSILIZE_NODE_PIPELINE_STAGES_NAME));
                assert(ret = ret && shaderStage.HasMember(STR_FOSSILIZE_NODE_PIPELINE_STAGES_MODULE));
                assert(ret = ret && shaderStage.HasMember(STR_FOSSILIZE_NODE_PIPELINE_STAGES_STAGE));

                // Verify the type.
                assert(ret = ret && shaderStage[STR_FOSSILIZE_NODE_FLAGS].IsUint());
                assert(ret = ret && shaderStage[STR_FOSSILIZE_NODE_PIPELINE_STAGES_NAME].IsString());
                assert(ret = ret && shaderStage[STR_FOSSILIZE_NODE_PIPELINE_STAGES_MODULE].IsUint());
                assert(ret = ret && shaderStage[STR_FOSSILIZE_NODE_PIPELINE_STAGES_STAGE].IsUint());

                std::string entryPointName = shaderStage[STR_FOSSILIZE_NODE_PIPELINE_STAGES_NAME].GetString();
                uint32_t shaderModuleIndex = shaderStage[STR_FOSSILIZE_NODE_PIPELINE_STAGES_MODULE].GetUint();
                VkShaderStageFlagBits stageFlags = static_cast<VkShaderStageFlagBits>(shaderStage[STR_FOSSILIZE_NODE_PIPELINE_STAGES_STAGE].GetUint());

                // Construct the file name for the output SPIR-V file.
                std::stringstream spirvFileName;
                spirvFileName << baseOutputFileName << "_";

                // Append the stage name.
                switch (stageFlags)
                {
                case VK_SHADER_STAGE_VERTEX_BIT:
                    spirvFileName << "vert";
                    break;
                case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
                    spirvFileName << "tesc";
                    break;
                case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
                    spirvFileName << "tese";
                    break;
                case VK_SHADER_STAGE_GEOMETRY_BIT:
                    spirvFileName << "geom";
                    break;
                case VK_SHADER_STAGE_FRAGMENT_BIT:
                    spirvFileName << "frag";
                    break;
                // These are invalid.
                case VK_SHADER_STAGE_COMPUTE_BIT:
                case VK_SHADER_STAGE_ALL_GRAPHICS:
                case VK_SHADER_STAGE_ALL:
                default:
                    // Unknown or invalid shader stage.
                    assert(false);
                    break;
                }

                // Append the entry point name.
                spirvFileName << "_" << entryPointName;

                // Append the extension.
                spirvFileName << STR_FILE_EXTENSION_SPIRV;

                // Locate the shader module in the map and serialize.
                assert(shaderModuleIndex <= shaderModules.size());
                if (shaderModuleIndex <= shaderModules.size())
                {
                    auto iter = shaderModules.find(shaderModuleIndex - 1);
                    assert(iter != shaderModules.end());
                    if (iter != shaderModules.end())
                    {
                        // Save the binary SPIR-V data to the disk.
                        WriteBinaryFile(spirvFileName.str(), iter->second.pCode, iter->second.codeSize);
                    }
                }
            }
        }
    }

    // Compute pipelines.
    std::vector<rgPsoComputeVulkan*> rgaComputePipelineRecipes;
    assert(ret = ret && doc.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINES));
    assert(ret = ret && doc[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINES].IsArray());

    // Track the index of the current compute pipeline.
    uint32_t computePipelineIndex = 0;

    for (const auto& computePipeline : doc[STR_FOSSILIZE_NODE_COMPUTE_PIPELINES].GetArray())
    {
        // Create and reset our compute pipeline create info structure.
        rgPsoComputeVulkan* pComputePipelineRecipe = new rgPsoComputeVulkan{};

        // Verify that the members exist.
        assert(ret = ret && computePipeline.HasMember(STR_FOSSILIZE_NODE_HASH));
        assert(ret = ret && computePipeline.HasMember(STR_FOSSILIZE_NODE_FLAGS));
        assert(ret = ret && computePipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_BASE_PIPELINE_HANDLE));
        assert(ret = ret && computePipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_BASE_PIPELINE_INDEX));
        assert(ret = ret && computePipeline.HasMember(STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_LAYOUT));
        assert(ret = ret && computePipeline.HasMember(STR_FOSSILIZE_NODE_COMPUTE_PIPELINE_STAGE));

        // Verify the type.
        assert(ret = ret && computePipeline[STR_FOSSILIZE_NODE_HASH].IsUint64());
        assert(ret = ret && computePipeline[STR_FOSSILIZE_NODE_FLAGS].IsUint());
        assert(ret = ret && computePipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_BASE_PIPELINE_HANDLE].IsUint());
        assert(ret = ret && computePipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_BASE_PIPELINE_INDEX].IsInt());
        assert(ret = ret && computePipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_LAYOUT].IsUint());
        assert(ret = ret && computePipeline[STR_FOSSILIZE_NODE_COMPUTE_PIPELINE_STAGE].IsObject());

        // Fill up our structure.
        VkComputePipelineCreateInfo* pComputePiplineCreateInfo = pComputePipelineRecipe->GetComputePipelineCreateInfo();
        assert(pComputePiplineCreateInfo != nullptr);
        if (pComputePiplineCreateInfo != nullptr)
        {
            pComputePiplineCreateInfo->flags = static_cast<VkFlags>(computePipeline[STR_FOSSILIZE_NODE_FLAGS].GetUint());

            // @TODO: figure out what should we do with this one. Is this a handle or an internal reference within the JSON.
            pComputePiplineCreateInfo->sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pComputePiplineCreateInfo->basePipelineHandle = reinterpret_cast<VkPipeline>(computePipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_BASE_PIPELINE_HANDLE].GetInt64());
            pComputePiplineCreateInfo->basePipelineIndex = static_cast<VkFlags>(computePipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_BASE_PIPELINE_INDEX].GetInt());

            // Pipeline layouts and descriptor set layouts.
            // Get the pipeline layout index within our collection.
            uint32_t pipelineLayoutIndex = computePipeline[STR_FOSSILIZE_NODE_GRAPHICS_PIPELINE_LAYOUT].GetUint();
            assert(pipelineLayoutIndex <= rgaPipelineLayouts.size());
            if (pipelineLayoutIndex > 0)
            {
                assert(rgaPipelineLayouts[pipelineLayoutIndex - 1] != nullptr);
                if (rgaPipelineLayouts[pipelineLayoutIndex - 1] != nullptr)
                {
                    VkPipelineLayoutCreateInfo* pPipelineLayoutCreateInfo = rgaPipelineLayouts[pipelineLayoutIndex - 1]->m_pVkPipelineLayoutCreateInfo;
                    assert(pPipelineLayoutCreateInfo != nullptr);
                    pComputePipelineRecipe->SetPipelineLayoutCreateInfo(pPipelineLayoutCreateInfo);

                    // Add the descriptor set layout create info structures.
                    for (size_t descriptorSetLayoutIndex : rgaPipelineLayouts[pipelineLayoutIndex - 1]->m_descriptorSetLayoutIndices)
                    {
                        assert(descriptorSetLayoutIndex <= rgaDescriptorSetLayouts.size());
                        if (descriptorSetLayoutIndex <= rgaDescriptorSetLayouts.size())
                        {
                            pComputePipelineRecipe->AddDescriptorSetLayoutCreateInfo(rgaDescriptorSetLayouts[descriptorSetLayoutIndex - 1]);
                        }
                    }
                }
            }

            // Serialize our pipeline recipe.
            std::stringstream outputFileName;
            outputFileName << outputDirectory << "/" << STR_FILE_BASE_FILENAME_PSO << ++graphicsPipelineIndex;

            std::string baseOutputFileName = outputFileName.str().c_str();
            std::string psoOutputFileName = outputFileName.str().c_str();
            psoOutputFileName.append(STR_FILE_EXTENSION_PSO);

            std::string errMsg;
            bool isComputePipelineSerialized = rgPsoSerializerVulkan::WriteStructureToFile(pComputePipelineRecipe, psoOutputFileName, errMsg);
            assert(isComputePipelineSerialized);

            // Add the file to the output buffer.
            if (isComputePipelineSerialized)
            {
                rgaPsoFiles.push_back(outputFileName.str().c_str());

                // Serialize the compute pipeline's shader.
                const auto& shaderStage = computePipeline[STR_FOSSILIZE_NODE_COMPUTE_PIPELINE_STAGE].GetObject();

                // Verify that the members exist.
                assert(ret = ret && shaderStage.HasMember(STR_FOSSILIZE_NODE_FLAGS));
                assert(ret = ret && shaderStage.HasMember(STR_FOSSILIZE_NODE_PIPELINE_STAGES_NAME));
                assert(ret = ret && shaderStage.HasMember(STR_FOSSILIZE_NODE_PIPELINE_STAGES_MODULE));
                assert(ret = ret && shaderStage.HasMember(STR_FOSSILIZE_NODE_PIPELINE_STAGES_STAGE));

                // Verify the type.
                assert(ret = ret && shaderStage[STR_FOSSILIZE_NODE_FLAGS].IsUint());
                assert(ret = ret && shaderStage[STR_FOSSILIZE_NODE_PIPELINE_STAGES_NAME].IsString());
                assert(ret = ret && shaderStage[STR_FOSSILIZE_NODE_PIPELINE_STAGES_MODULE].IsUint());
                assert(ret = ret && shaderStage[STR_FOSSILIZE_NODE_PIPELINE_STAGES_STAGE].IsUint());

                std::string entryPointName = shaderStage[STR_FOSSILIZE_NODE_PIPELINE_STAGES_NAME].GetString();
                uint32_t shaderModuleIndex = shaderStage[STR_FOSSILIZE_NODE_PIPELINE_STAGES_MODULE].GetUint();
                VkShaderStageFlagBits stageFlags = static_cast<VkShaderStageFlagBits>(shaderStage[STR_FOSSILIZE_NODE_PIPELINE_STAGES_STAGE].GetUint());

                // Construct the file name for the output SPIR-V file.
                std::stringstream spirvFileName;
                spirvFileName << baseOutputFileName << "_";

                // Append the stage name.
                switch (stageFlags)
                {
                case VK_SHADER_STAGE_COMPUTE_BIT:
                    spirvFileName << "comp";
                    break;

                // The following are invalid for a compute pipeline.
                case VK_SHADER_STAGE_VERTEX_BIT:
                case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
                case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
                case VK_SHADER_STAGE_GEOMETRY_BIT:
                case VK_SHADER_STAGE_FRAGMENT_BIT:
                case VK_SHADER_STAGE_ALL_GRAPHICS:
                case VK_SHADER_STAGE_ALL:
                default:
                    // Unknown shader stage.
                    assert(false);
                    break;
                }

                // Append the entry point name.
                spirvFileName << "_" << entryPointName;

                // Append the extension.
                spirvFileName << STR_FILE_EXTENSION_SPIRV;

                // Locate the shader module in the map and serialize.
                assert(shaderModuleIndex <= shaderModules.size());
                if (shaderModuleIndex <= shaderModules.size())
                {
                    auto iter = shaderModules.find(shaderModuleIndex - 1);
                    assert(iter != shaderModules.end());
                    if (iter != shaderModules.end())
                    {
                        // Save the binary SPIR-V data to the disk.
                        WriteBinaryFile(spirvFileName.str(), iter->second.pCode, iter->second.codeSize);
                    }
                }
            }
        }
    }

    return ret;
}
