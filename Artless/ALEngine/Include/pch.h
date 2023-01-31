/*!
file:	pch.h
author:	Wong Man Cong
email:	w.mancong\@digipen.edu
brief:	This file contain all the header files that are will be precompiled instead of
		recompiling it every build

		All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	PCH_H
#define PCH_H

// MACROS
#define NOMINMAX
#define _USE_MATH_DEFINES
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))	// find the number of elements in an array
#define EDITOR 1

// C++ standard library
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <cstdint>
#include <memory>
#include <thread>
#include <utility>
#include <mutex>
#include <cmath>
#include <cfloat>
#include <limits>
#include <cassert>
#include <random>
#include <fstream>
#include <chrono>
#include <iterator>
#include <filesystem>
#include <scoped_allocator>
#include <ctime>
#include <chrono>
#include <bitset>
#include <atomic>

// Data structures
#include <string>
#include <sstream>
#include <stack>
#include <deque>
#include <queue>
#include <vector>
#include <set> 
#include <array>
#include <map>
#include <bitset>
#include <unordered_set>
#include <unordered_map>

// Windows API
#include <Windows.h>

// ImGui
#include "Editor/imgui/imgui.h"
#include "Editor/imgui/imgui_impl_glfw.h"
#include "Editor/imgui/imgui_impl_opengl3.h"
#include "Editor/ImGuizmo/ImGuizmo.h"

// Graphics
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#define STB_IMAGE_IMPLEMENTATION

// Self declared header file
#include <Common Header/MySerializer.h>
#include <Common Header/Utility.h>
#include <Common Header/MyMemory.h>
#include <Common Header/TemplateClasses.h>
#include <Common Header/MyMath.h>
#include <Common Header/MyGraphics.h>
#include <Common Header/MyAudio.h>
#include <Common Header/MyEngine.h>
#include <Common Header/ECS.h>
#include <Common Header/MyInput.h>
#include <Common Header/MyExceptions.h>
#include <Common Header/Random.h>
#include <Common Header/MyEditor.h>
#include <Common Header/MyAssetManager.h>
#include <Common Header/MyScene.h>
#include <Common Header//MyUndoRedo.h>

// Serializations
#include <Serialization/Serializer.h>
#include <Engine/TurnStateManager.h>

#endif