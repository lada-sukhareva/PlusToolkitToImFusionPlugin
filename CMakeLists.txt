# Define a new CMake project for the plugin
cmake_minimum_required(VERSION 3.2.0)
project(MhaAdapterPlugin)

# Locate the ImFusion SDK.
# List required modules/plugins in the COMPONENTS section (e.g. COMPONENTS ImFusionSeg ImFusionReg).
#find_package(ImFusionLib REQUIRED)
find_package(ImFusionLib COMPONENTS ImFusionML ImFusionUS REQUIRED)

# Enable automatic MOC, RCC and UIC preprocessing for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


# Define and configure the CMake target
set(Sources
	MhaAdapterAlgorithm.cpp
	MhaAdapterController.cpp
	MhaAdapterFactory.cpp
	MhaAdapterPlugin.cpp
)
set(Headers
	MhaAdapterAlgorithm.h
	MhaAdapterController.h
	MhaAdapterFactory.h
	MhaAdapterPlugin.h
)
set(UiSources
	MhaAdapterController.ui
)

# Define target library
add_library(MhaAdapterPlugin SHARED ${Sources} ${UiHeaders} ${Headers})
target_include_directories(MhaAdapterPlugin PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
)
# Link agains the ImFusionLib and selected modules/plugins
#target_link_libraries(MhaAdapterPlugin PRIVATE
#	ImFusionLib
#)
target_link_libraries(MhaAdapterPlugin PRIVATE ImFusionLib ImFusionML ImFusionUS)

# Define output target directories and provide instructions on how to launch
# the ImFusion Suite with the built custom plugin.
# These functions are provided by the ImFusionLib target config. 
imfusion_set_common_target_properties()
imfusion_provide_ide_instructions()

