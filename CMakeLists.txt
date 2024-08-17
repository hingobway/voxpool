cmake_minimum_required(VERSION 3.22)

project(VOXPOOL_CMAKE VERSION 0.5.0)

# add_subdirectory(~/JUCE JUCE)
add_subdirectory(C:/JUCE JUCE)

juce_add_plugin(Plugin
  COMPANY_NAME foster
  PLUGIN_MANUFACTURER_CODE Fost
  PLUGIN_CODE Ft7c

  FORMATS AU VST3 Standalone
  PRODUCT_NAME "VoxPoolCMAKE"
)

juce_generate_juce_header(Plugin)

target_compile_definitions(Plugin
  PUBLIC
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0
  JUCE_VST3_CAN_REPLACE_VST2=0

  DONT_SET_USING_JUCE_NAMESPACE=1
)

file(GLOB_RECURSE HEADER_FILES src/*.h)

# SOURCE FILES
target_sources(Plugin
  PRIVATE
  ${HEADER_FILES}

  src/PluginProcessor.cpp
  src/PluginEditor.cpp
)

# # solution tree - only add if you use a solution-based editor
# file(GLOB_RECURSE SOURCE_TREE src/*)
# source_group(src TREE ../src PREFIX src FILES ${SOURCE_TREE})

# BINARY DATA
juce_add_binary_data(BinaryData
  SOURCES
  assets/svg/button_off.svg
  assets/svg/button_on.svg
  assets/svg/instructions.svg
  assets/svg/knob3.svg

  assets/fonts/AtkinsonHyperlegible-Bold.ttf
  assets/fonts/AtkinsonHyperlegible-Regular.ttf
)

target_link_libraries(Plugin
  PRIVATE
  BinaryData

  juce::juce_audio_devices
  juce::juce_audio_formats
  juce::juce_audio_utils

  PUBLIC
  juce::juce_recommended_config_flags
  juce::juce_recommended_lto_flags
  juce::juce_recommended_warning_flags
)
