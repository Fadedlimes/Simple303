cmake_minimum_required(VERSION 3.22)
project(Simple303 VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure macOS backwards compatibility
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")
endif()

# Fetch latest JUCE with macOS 15 / Xcode 16 compatibility fixes
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(JUCE)

file(GLOB_RECURSE DSP_SOURCES "DSPCode/*.cpp" "DSPCode/*.h")
file(GLOB_RECURSE PLUGIN_SOURCES "Source/*.cpp" "Source/*.h")

set(PLUGIN_FORMATS VST3 Standalone)
if(APPLE)
    list(APPEND PLUGIN_FORMATS AU)
endif()

juce_add_plugin(Simple303
    COMPANY_NAME "Fadedlimes"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT FALSE
    PLUGIN_MANUFACTURER_CODE FdLm
    PLUGIN_CODE S303
    FORMATS ${PLUGIN_FORMATS}
    PRODUCT_NAME "Simple303"
)

target_sources(Simple303
    PRIVATE
        ${PLUGIN_SOURCES}
        ${DSP_SOURCES}
)

target_include_directories(Simple303
    PRIVATE
        Source
        DSPCode
)

target_compile_definitions(Simple303
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        _USE_MATH_DEFINES
        NOMINMAX
)

target_link_libraries(Simple303
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_gui_basics
        juce::juce_gui_extra
)
