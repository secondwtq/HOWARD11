cmake_minimum_required(VERSION 3.0)
project(HOWARD11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(SYSTEM src)

function(prepend var prefix)
   set(listVar "")
   foreach(f ${ARGN})
      list(APPEND listVar "${prefix}/${f}")
   endforeach(f)
   set(${var} "${listVar}" PARENT_SCOPE)
endfunction(prepend)

set(SOURCE_FSM FSM.hxx FSM.cxx FSMProxyTemplates.hxx FSMStream.hxx FSMCommon.hxx FSMLogger.hxx FSMLogger.hxx)

set(SOURCE_VERDANDI GLCommon.hxx GLFoundation.hxx GLFoundation.cxx
    GLShader.hxx GLShader.cxx GLShaderExt.hxx GLShaderExt.cxx
    GLShaderExtDef.hxx)

set(SOURCE_STANNUM StannumCommon.hxx Stannum.hxx Stannum.cxx StannumShaderCache.hxx
    StannumShaderCache.cxx StannumTexture.hxx StannumTexture.cxx StannumTest.hxx)

set(SOURCE_DWIGHT HowardBase.hxx HowardBaseEx.cxx Node.hxx Node.cxx Handle.hxx Handle.cxx
RootNode.hxx HandleE.hxx HandleManager.hxx SceneManager.hxx SceneManager.cxx ScriptNode.hxx
StannumTestNode.hxx)

set(SOURCE_THIRDPT howardgl.hxx stb.cxx)

prepend(SOURCE_FSM "FSM" ${SOURCE_FSM})
prepend(SOURCE_VERDANDI Verdandi ${SOURCE_VERDANDI})
prepend(SOURCE_STANNUM Stannum ${SOURCE_STANNUM})
prepend(SOURCE_DWIGHT Dwight ${SOURCE_DWIGHT})
prepend(SOURCE_THIRDPT thirdpt ${SOURCE_THIRDPT})

set(SOURCE_ALL ${SOURCE_ALL} ${SOURCE_FSM} ${SOURCE_VERDANDI} ${SOURCE_STANNUM} ${SOURCE_DWIGHT}
    ${SOURCE_THIRDPT})

set(SOURCE_OTHER src/Debug.hxx
    src/Common.hxx src/Asset.hxx src/Entry.cxx src/AtTheVeryBeginning.hxx src/AtTheVeryBeginning.cxx
    src/Acheron.hxx src/Asset.cxx)

prepend(SOURCE_ALL src ${SOURCE_ALL})

add_executable(HOWARD11 ${SOURCE_ALL} ${SOURCE_OTHER})

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
