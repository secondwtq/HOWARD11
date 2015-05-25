cmake_minimum_required(VERSION 3.0)
project(HOWARD11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(HOWARD11 ${SOURCE_FILES} Debug.hxx Node.hxx HowardBase.hxx Node.hxx HowardBase.hxx
    Common.hxx ScriptNode.hxx Handle.hxx HowardBaseEx.cxx Heroes.hxx HandleManager.hxx Asset.hxx
    Node.cxx RootNode.hxx Handle.cxx SceneManager.hxx Entry.cxx HandleE.hxx Stannum.hxx
    StannumTestNode.hxx GLShader.hxx GLShader.cxx GLShaderExt.hxx GLShaderExt.cxx GLShaderExtDef.hxx
    howardgl.hxx StannumTest.hxx GLFoundation.cxx GLCommon.hxx GLFoundation.hxx StannumShaderCache.hxx
    StannumCommon.hxx StannumShaderCache.cxx Stannum.cxx StannumTexture.hxx AtTheVeryBeginning.hxx AtTheVeryBeginning.cxx Acheron.hxx
    FSM.hxx FSM.cxx FSMCommon.hxx FSMStream.hxx FSMProxyTemplates.hxx FSMLogger.hxx)

target_link_libraries(HOWARD11 glfw3)

if(APPLE)

    find_library(FRAMEWORK_COCOA Cocoa)
    find_library(FRAMEWORK_GL OpenGL)
    find_library(FRAMEWORK_IOKIT IOKit)
    find_library(FRAMEWORK_CORE_VIDEO CoreVideo)
    # mark_as_advanced(FRAMEWORK_COCOA)
    set(EXTRA_LIBS_APPLE ${FRAMEWORK_COCOA} ${FRAMEWORK_GL} ${FRAMEWORK_IOKIT}
        ${FRAMEWORK_CORE_VIDEO})

endif(APPLE)

target_link_libraries(HOWARD11 ${EXTRA_LIBS_APPLE})
target_link_libraries(HOWARD11 confuse)
