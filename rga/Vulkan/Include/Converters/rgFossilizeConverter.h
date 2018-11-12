#pragma once

// C++.
#include <string>
#include <vector>
#include <map>

// Local.
#include <rga/Vulkan/Include/rgPipelineTypes.h>

class rgFossilizeConverter
{
public:
    // Converts a single fossilize JSON file to one or more multiple RGA PSO files.
    // Returns true for success, false otherwise.
    static bool Convert(const rapidjson::Document& doc, const std::string& outputDirectory, bool isLogEnabled,
        const std::map<uint32_t, VkShaderModuleCreateInfo>& shaderModules, std::vector<std::string>& rgaPsoFiles);

private:
    rgFossilizeConverter() = delete;
    ~rgFossilizeConverter() = delete;
};