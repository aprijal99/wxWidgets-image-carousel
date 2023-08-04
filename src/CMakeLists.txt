find_package(wxWidgets REQUIRED core base xml) # call 'core base xml' here to enable wxXmlDocument

include_directories(../include ${wxWidgets_INCLUDE_DIRS})
file(GLOB SOURCES "./*.cpp")

add_executable(ImageCarousel WIN32 ${SOURCES})
target_link_libraries(ImageCarousel ${wxWidgets_LIBRARIES})