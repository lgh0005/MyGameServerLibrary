#pragma once

// C++
#include <array>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <type_traits>
#include <functional>
#include <thread>
#include <memory>
#include <mutex>
#include <atomic>
#include <optional>
#include <utility>
#include <limits>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>

// Shared
#include "Types.h"
#include "Aliases.h"
#include "Macros.h"

// Debug
#include "Logger.h"
#include "Asserter.h"

// Managers
#include "ThreadManager.h"

#define MGSL_THREAD_MGR MGSL::Shared::ThreadManager::Instance()