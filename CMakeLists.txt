
project(Peach-Chess)

cmake_minimum_required(VERSION 3.22.1)
file (GLOB SOURCES src/*.cpp)

set(SDL2_DIR "C:/Dev/SDL2/x86_64-w64-mingw32") 
set(SDL2IMAGE_DIR "C:/Dev/SDL2_image/x86_64-w64-mingw32") 




set(SDL2_INCLUDE_DIR "${SDL2_DIR}/include/SDL2") 
set(SDL2_LIBRARY_DIR "${SDL2_DIR}/lib") 
set(SDL2IMAGE_INCLUDE_DIRS "${SDL2IMAGE_DIR}/include/SDL2") 
set(SDL2IMAGE_LIBRARIES "${SDL2IMAGE_DIR}/lib") 

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)

message(${SDL2IMAGE_INCLUDE_DIRS})


add_executable(${PROJECT_NAME} ${SOURCES})

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    mingw32 
    ${SDL2_LIBRARY_DIR}/libSDL2main.a 
    ${SDL2_LIBRARY_DIR}/libSDL2.dll.a 
    ${SDL2IMAGE_LIBRARIES}/libSDL2_image.dll.a
    -static-libstdc++ 
    -static-libgcc
    )


target_include_directories(${PROJECT_NAME} PUBLIC include/)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY assets DESTINATION .)









