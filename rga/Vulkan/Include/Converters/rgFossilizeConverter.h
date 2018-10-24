#pragma once

// C++.
#include <string>

// Local.
#include <rga/Vulkan/Include/rgPipelineTypes.h>

class rgFossilizeConverter
{
public:
    // Converts a single fossilize JSON file to one or more multiple RGA PSO files.
    // Returns true for success, false otherwise.
    static bool Convert(const std::string& pathToFossilizeFile, const std::string& outputDirectory, std::vector<std::string>& rgaPsoFiles);

    // Converts a single fossilize JSON file to one or more multiple RGA PSO files.
// Returns true for success, false otherwise.
    static bool Convert(const rapidjson::Document& doc, const std::string& outputDirectory, std::vector<std::string>& rgaPsoFiles);

private:
    rgFossilizeConverter() = delete;
    ~rgFossilizeConverter() = delete;
};