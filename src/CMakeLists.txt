add_subdirectory(Core)
add_subdirectory(Utility)
add_subdirectory(Math)
add_subdirectory(Input)
add_subdirectory(Graphics)
add_subdirectory(ECS)
add_subdirectory(Resource)

set(INARIKONKON_MODULES
    ${CMAKE_CURRENT_LIST_DIR}/InariKonKon.cppm
    ${INARIKONKON_CORE_MODULES}
    ${INARIKONKON_UTILITY_MODULES}
    ${INARIKONKON_MATH_MODULES}
    ${INARIKONKON_INPUT_MODULES}
    ${INARIKONKON_GRAPHICS_MODULES}
    ${INARIKONKON_ECS_MODULES}
    ${INARIKONKON_RESOURCE_MODULES}
    PARENT_SCOPE)