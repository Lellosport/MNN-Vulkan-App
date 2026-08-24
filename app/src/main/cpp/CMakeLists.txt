cmake_minimum_required(VERSION 3.10.2)
project(MNNVulkanTest)

# Punta alla cartella degli header (che estrarrà GitHub)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Importa le tue librerie dal telefono
add_library(MNN SHARED IMPORTED)
set_target_properties(MNN PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libMNN.so)

add_library(MNN_Vulkan SHARED IMPORTED)
set_target_properties(MNN_Vulkan PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libMNN_Vulkan.so)

# Crea l'eseguibile e collegalo ai sistemi Android
add_executable(test_vulkan test_vulkan.cpp)
target_link_libraries(test_vulkan MNN MNN_Vulkan log android)

