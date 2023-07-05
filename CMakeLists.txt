#cmake_minimum_required(VERSION 3.23)
#project(IGRIOP)
#
#set(CMAKE_CXX_STANDARD 11)
#
#add_executable(IGRIOP gpt_game.cpp)
#
#target_link_libraries(IGRIOP "-framework OpenGL" "-framework GLUT")
cmake_minimum_required(VERSION 3.23)
project(IGRIOP)

# Поиск пакетов OpenGL и GLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Включение заголовочных файлов и библиотек OpenGL и GLUT
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

# Добавление исполняемого файла вашего проекта
add_executable(IGRIOP gpt_game.cpp)

find_package(GLUT REQUIRED)
target_link_libraries(IGRIOP GLUT::GLUT)

# Линковка библиотек OpenGL и GLUT с исполняемым файлом
target_link_libraries(IGRIOP ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
