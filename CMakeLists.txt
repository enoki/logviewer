cmake_minimum_required(VERSION 2.8.12)
project(logviewer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
FIND_PACKAGE(Qt5Widgets REQUIRED)

set(sources
    main.cpp
    GYLogModel.cpp
    GYLogView.cpp
    GYLogWindow.cpp
)
set(headers
)

add_executable(${PROJECT_NAME}
    WIN32 MACOSX_BUNDLE
    ${sources})

if(APPLE)
    set(OSX_SDK_VERSION 10.9)
    set(OSX_MIN_VERSION 10.9)
    SET(CMAKE_OSX_SYSROOT "${OSX_DEVELOPER_ROOT}/Platforms/MacOSX.platform/Developer/SDKs/MacOSX${OSX_SDK_VERSION}.sdk")
    SET (CMAKE_XCODE_IN_APPLICATIONS 1)

    SET(CMAKE_OSX_SDK_NAME macosx${OSX_SDK_VERSION})
    SET(CMAKE_OSX_ARCHITECTURES "x86_64")
    SET(CMAKE_CXX_FLAGS "-mmacosx-version-min=${OSX_MIN_VERSION} -stdlib=libc++ -std=c++11 -gdwarf-2")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -D_DEBUG -Wall")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE}")

    SET(CMAKE_C_FLAGS "-mmacosx-version-min=${OSX_MIN_VERSION} -gdwarf-2")
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -D_DEBUG -Wall")
    SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3")
    SET(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELEASE}")
elseif(WIN32)
    SET(CMAKE_CXX_FLAGS "/nologo /WX- /MP /Oi /Oy- /EHsc /Zc:wchar_t /Zc:forScope /Gd /fp:fast")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} /MDd /Z7 /RTC1 /GS /Od /bigobj")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /MD /Z7 /GS- /O2 /Ob2")
    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE}")

    SET(CMAKE_C_FLAGS "/nologo /WX- /MP /Oi /Oy- /EHsc /Zc:wchar_t /Zc:forScope /Gd /fp:fast")
    SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} /MDd /Z7 /RTC1 /GS /Od /bigobj")
    SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} /MD /Z7 /GS- /O2 /Ob2")
    SET(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELEASE}")
endif()

#if(APPLE)
#    Find_library(AppKitFramework AppKit)
#    find_library(CoreFoundationFramework CoreFoundation)
#    find_library(IOKitFramework IOKit)
#    find_library(IOKitFramework IOKit)
#    find_library(OpenGLFramework OpenGL)
#    set(PLATFORM_LIBS
#        ${AppKitFramework}
#        ${CoreFoundationFramework}
#        ${IOKitFramework}
#        ${OpenGLFramework}
#        c++
#    )
#    macro(CopyTargetDylib target)
#        get_property(dll_path TARGET ${target} PROPERTY LOCATION_${CMAKE_BUILD_TYPE})
#        message("Copy ${dll_path} to ${CMAKE_BINARY_DIR}/bin")
#        file(COPY ${dll_path} DESTINATION ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.app/Contents/MacOS)
#    endmacro()
#    CopyTargetDylib(neatimage)
#elseif(WIN32)
#    set(PLATFORM_LIBS
#        glut32
#        glew32
#    )
#    macro(CopyTargetDll target)
#        get_property(dll_path TARGET ${target} PROPERTY LOCATION_${CMAKE_BUILD_TYPE})
#        message("Copy ${dll_path} to ${CMAKE_BINARY_DIR}/bin")
#        file(COPY ${dll_path} DESTINATION ${CMAKE_BINARY_DIR}/bin)
#    endmacro()
#    CopyTargetDll(Qt5::Core)
#    CopyTargetDll(Qt5::Gui)
#    CopyTargetDll(Qt5::Widgets)
#    CopyTargetDll(Qt5::OpenGL)
#    CopyTargetDll(boost_chrono)
#    CopyTargetDll(boost_system)
#    CopyTargetDll(boost_thread)
#endif()

target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets)
