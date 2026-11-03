include(FetchContent)

add_library(Dependencies INTERFACE)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 12.2.0)
FetchContent_MakeAvailable(fmt)

set(SPDLOG_FMT_EXTERNAL
    ON
    CACHE BOOL "" FORCE)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.17.0)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.5.1)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  entt
  GIT_REPOSITORY https://github.com/skypjack/entt.git
  GIT_TAG v4.0.0)
FetchContent_MakeAvailable(entt)

find_package(Vulkan 1.4.313 REQUIRED shaderc_combined)

FetchContent_Declare(
  VMA
  GIT_REPOSITORY
    https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
  GIT_TAG v3.4.0)
FetchContent_MakeAvailable(vma)

find_package(OpenGL REQUIRED)

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v2.0.8
  SOURCE_SUBDIR cmake)
FetchContent_MakeAvailable(glad)

glad_add_library(glad_gl_core_46 STATIC REPRODUCIBLE LOADER API gl:core=4.6)

set(EXTERNAL_LIBRARIES
    fmt::fmt
    spdlog::spdlog
    glfw
    EnTT::EnTT
    Vulkan::Vulkan
    Vulkan::shaderc_combined
    GPUOpen::VulkanMemoryAllocator
    OpenGL::GL
    glad_gl_core_46)

target_link_libraries(Dependencies INTERFACE ${EXTERNAL_LIBRARIES})

foreach(DEPENDENCY IN LISTS EXTERNAL_LIBRARIES)
  get_target_property(INCLUDES ${DEPENDENCY} INTERFACE_INCLUDE_DIRECTORIES)

  if(INCLUDES)
    target_include_directories(Dependencies SYSTEM INTERFACE ${INCLUDES})
  endif()
endforeach()
