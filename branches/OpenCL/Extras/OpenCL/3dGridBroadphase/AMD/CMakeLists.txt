
ADD_DEFINITIONS(-DCL_PLATFORM_AMD=0x54321)

INCLUDE_DIRECTORIES(
	${BULLET_PHYSICS_SOURCE_DIR}/src
	${BULLET_PHYSICS_SOURCE_DIR}/Extras/OpenCL/Shared
	${BULLET_PHYSICS_SOURCE_DIR}/Extras/OpenCL/3dGridBroadphase/Shared
	$ENV{AMDAPPSDKROOT}/include
)

SET(3dGridBroadphaseOCL_SRCS
	${BULLET_PHYSICS_SOURCE_DIR}/Extras/OpenCL/Shared/btOclCommon.cpp
	${BULLET_PHYSICS_SOURCE_DIR}/Extras/OpenCL/3dGridBroadphase/Shared/bt3dGridBroadphaseOCL.cpp
	${BULLET_PHYSICS_SOURCE_DIR}/Extras/OpenCL/3dGridBroadphase/Shared/bt3dGridBroadphaseOCL.cl
)

SET(3dGridBroadphaseOCL_HDRS
	${BULLET_PHYSICS_SOURCE_DIR}/Extras/OpenCL/Shared/btOclCommon.h
	${BULLET_PHYSICS_SOURCE_DIR}/Extras/OpenCL/3dGridBroadphase/Shared/bt3dGridBroadphaseOCL.h
)

ADD_LIBRARY(3dGridBroadphaseOCL_AMD ${3dGridBroadphaseOCL_SRCS} ${3dGridBroadphaseOCL_HDRS})
SET_TARGET_PROPERTIES(3dGridBroadphaseOCL_AMD PROPERTIES VERSION ${BULLET_VERSION})
SET_TARGET_PROPERTIES(3dGridBroadphaseOCL_AMD PROPERTIES SOVERSION ${BULLET_VERSION})

IF (INSTALL_LIBS)
	IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)
		#FILES_MATCHING requires CMake 2.6
		IF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5)
			IF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
				INSTALL(TARGETS 3dGridBroadphaseOCL_AMD DESTINATION .)
			ELSE (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
				INSTALL(TARGETS 3dGridBroadphaseOCL_AMD DESTINATION lib${LIB_SUFFIX})
				INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include FILES_MATCHING PATTERN "*.h")
			ENDIF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
		ENDIF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5)

		IF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
			SET_TARGET_PROPERTIES(3dGridBroadphaseOCL_AMD PROPERTIES FRAMEWORK true)
			SET_TARGET_PROPERTIES(3dGridBroadphaseOCL_AMD PROPERTIES PUBLIC_HEADER ${3dGridBroadphaseOCL_HDRS})
		ENDIF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
	ENDIF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)
ENDIF (INSTALL_LIBS)

