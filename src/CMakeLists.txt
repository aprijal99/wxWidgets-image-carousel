find_package(wxWidgets REQUIRED)

include_directories(../include ${wxWidgets_INCLUDE_DIRS})
file(GLOB SOURCES "./*.cpp")

add_executable(ImageCarousel WIN32 ${SOURCES})
target_link_libraries(ImageCarousel ${wxWidgets_LIBRARIES})