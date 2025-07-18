
add_library(FastUI
    Button.cpp
    Character.cpp
    CharacterOpenGL.cpp
    Checkbox.cpp
    Drawer.cpp
    DrawerOpenGL.cpp
    DropdownList.cpp
    Font.cpp
    FontOpenGL.cpp
    Image.cpp
    ImageOpenGL.cpp
    LinearLayout.cpp
    ShaderProgram.cpp
    Spacer.cpp
    TextArea.cpp
    TextField.cpp
    UnicodeString.cpp
    Util.cpp
    View.cpp
)

set(RESOURCE_FILES
    char.frag
    char.vert
    image.frag
    image.vert
    shader.frag
    shader.vert
    cat.jpg
    NotoEmoji-Regular.ttf
    NotoSansSymbols2-Regular.ttf
    OpenSans-Regular.ttf
)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm REQUIRED)
find_package(glad REQUIRED)
find_package(Freetype REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc dt in io)
find_path(STB_INCLUDE_DIRS "stb_image.h")

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
target_compile_definitions(${PROJECT_NAME} PUBLIC GLM_ENABLE_EXPERIMENTAL)

target_include_directories(${PROJECT_NAME} PUBLIC ${STB_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    glfw
    glad::glad
    glm::glm
    Freetype::Freetype
    ICU::dt                                 #::data for non-windows? https://cmake.org/cmake/help/latest/module/FindICU.html
    ICU::uc
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

#Copy resources (shaders, fonts, images) to build dir
foreach (RES IN LISTS RESOURCE_FILES)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${PROJECT_SOURCE_DIR}/src/${RES}
                 $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endforeach()
