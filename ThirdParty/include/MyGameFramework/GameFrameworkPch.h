#pragma once

// Shared
#ifdef _DEBUG
#pragma comment(lib, "MyGameShared_d.lib")
#pragma comment(lib, "MyGameServerProtocol_d.lib")
#pragma comment(lib, "MyGameServerLibrary_d.lib")
#else
#pragma comment(lib, "MyGameShared.lib")
#pragma comment(lib, "MyGameServerProtocol.lib")
#pragma comment(lib, "MyGameServerLibrary.lib")
#endif

#include "MyGameServerLibrary/ServerLibPch.h"

// C++
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

// Windows
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// OpenGL
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/wglew.h>
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "Opengl32.lib")

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

// ktx
#ifdef _DEBUG
#pragma comment(lib, "ktx_d.lib")
#else
#pragma comment(lib, "ktx.lib")
#endif

// freetype
#include <freetype/ft2build.h>
#include FT_FREETYPE_H
#pragma comment(lib, "freetype.lib")

// Custom
#include "Enums.h"
#include "GLContext.h"
#include "Vertex.h"
#include "Uniform.h"
#include "InstanceData.h"
#include "RenderItem.h"
#include "Glyph.h"

// Managers
#include "InputManager.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "UIManager.h"
#include "CollisionManager.h"

#define MGSL_INPUT_MGR    MGSL::Framework::InputManager::Instance()
#define MGSL_TIME_MGR     MGSL::Framework::TimeManager::Instance()
#define MGSL_RENDER_MGR   MGSL::Framework::RenderManager::Instance()
#define MGSL_OBJECT_MGR   MGSL::Framework::ObjectManager::Instance()
#define MGSL_RESOURCE_MGR MGSL::Framework::ResourceManager::Instance()
#define MGSL_UI_MGR		  MGSL::Framework::UIManager::Instance()
#define MGSL_COLLIDE_MGR  MGSL::Framework::CollisionManager::Instance()