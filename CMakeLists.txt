CMAKE_MINIMUM_REQUIRED (VERSION 3.3)

PROJECT (VTKOpenVR)

FIND_PACKAGE (VTK REQUIRED)
INCLUDE (${VTK_USE_FILE})

ADD_EXECUTABLE (${PROJECT_NAME} MACOSX_BUNDLE ${PROJECT_NAME}.cxx)

IF (VTK_LIBRARIES)
  TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${VTK_LIBRARIES})
ELSE ()
  TARGET_LINK_LIBRARIES (${PROJECT_NAME} vtkHybrid vtkWidgets)
ENDIF ()