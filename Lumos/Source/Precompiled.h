#pragma once

#ifdef __cplusplus
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <cstddef>
#include <cfloat>
#include <cstring>
#include <utility>
#include <memory>
#include <thread>
#include <cassert>
#include <variant>
#include <stdio.h>
#include <math.h>

#include "Core/Reference.h"
#include "Core/LMLog.h"
#include "Core/Core.h"
#include "Core/Profiler.h"
#include "Core/Thread.h"
#include "Core/DataStructures/Vector.h"
#include "Graphics/RHI/Definitions.h"

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cereal/cereal.hpp>
#include <imgui/imgui.h>

#ifdef LUMOS_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#ifdef LUMOS_RENDER_API_VULKAN
#include "Platform/Vulkan/VK.h"
#endif

#ifdef LUMOS_RENDER_API_OPENGL
#include <glad/glad.h>
#endif
#endif
