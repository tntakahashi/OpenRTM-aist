cmake_minimum_required (VERSION 2.6)

project (StaticFSM)


link_directories(${ORB_LINK_DIR})
include_directories(
		${RTM_INCLUDE_DIR}
		${ORB_INCLUDE_DIR}
		)

ADD_DEFINITIONS(${ORB_C_FLAGS_LIST})


set(target Display)

set(srcs Display.cpp Display.h)
set(standalone_srcs DisplayComp.cpp ${srcs})

if(VXWORKS AND NOT RTP)
	set(standalone_srcs ${standalone_srcs} ${CMAKE_SOURCE_DIR}/src/lib/rtm/DataFlowComponentBase.cpp)
	set(libs ${RTCSKEL_PROJECT_NAME})

	add_executable(${target} ${standalone_srcs})
	target_link_libraries(${target} ${libs})
	add_dependencies(${target} ${RTM_PROJECT_NAME})

	install(TARGETS ${target} LIBRARY DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				ARCHIVE DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				RUNTIME DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				COMPONENT examples)
else()
	if(VXWORKS)
		set(libs ${RTM_PROJECT_NAME} ${ORB_LIBRARIES} ${COIL_PROJECT_NAME} ${RTCSKEL_PROJECT_NAME})
	else(VXWORKS)
		set(libs ${RTM_PROJECT_NAME} ${ORB_LIBRARIES} ${COIL_PROJECT_NAME})
	endif(VXWORKS)

	add_executable(${target}Comp ${standalone_srcs})
	target_link_libraries(${target}Comp ${libs} ${RTM_LINKER_OPTION})
	add_dependencies(${target}Comp ${RTM_PROJECT_NAME})

	add_library(${target} SHARED ${srcs})
	target_link_libraries(${target} ${libs} ${RTM_LINKER_OPTION})
	add_dependencies(${target} ${RTM_PROJECT_NAME})
	set_target_properties(${target} PROPERTIES PREFIX "")

	install(TARGETS ${target}Comp LIBRARY DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				ARCHIVE DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				RUNTIME DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				COMPONENT examples)
	
	install(TARGETS ${target} LIBRARY DESTINATION ${INSTALL_RTM_EXAMPLE_SHAREDLIB_DIR}
				ARCHIVE DESTINATION ${INSTALL_RTM_EXAMPLE_SHAREDLIB_DIR}
				RUNTIME DESTINATION ${INSTALL_RTM_EXAMPLE_SHAREDLIB_DIR}
				COMPONENT examples)

	install(FILES ${standalone_srcs} DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}/src/StaticFSM COMPONENT sources)

endif()

if(VXWORKS)
	if(RTP)
		set_target_properties(${target}Comp PROPERTIES SUFFIX ".vxe")
	else(RTP)	
		set_target_properties(${target} PROPERTIES SUFFIX ".out")
	endif(RTP)
endif(VXWORKS)



set(target Inputbutton)



set(srcs Inputbutton.cpp Inputbutton.h)
set(standalone_srcs InputbuttonComp.cpp ${srcs})

if(VXWORKS AND NOT RTP)
	set(standalone_srcs ${standalone_srcs} ${CMAKE_SOURCE_DIR}/src/lib/rtm/DataFlowComponentBase.cpp)
	set(libs ${RTCSKEL_PROJECT_NAME})

	add_executable(${target} ${standalone_srcs})
	target_link_libraries(${target} ${libs})
	add_dependencies(${target} ${RTM_PROJECT_NAME})

	install(TARGETS ${target} LIBRARY DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				ARCHIVE DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				RUNTIME DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				COMPONENT examples)
else()
	if(VXWORKS)
		set(libs ${RTM_PROJECT_NAME} ${ORB_LIBRARIES} ${COIL_PROJECT_NAME} ${RTCSKEL_PROJECT_NAME})
	else(VXWORKS)
		set(libs ${RTM_PROJECT_NAME} ${ORB_LIBRARIES} ${COIL_PROJECT_NAME})
	endif(VXWORKS)

	add_executable(${target}Comp ${standalone_srcs})
	target_link_libraries(${target}Comp ${libs} ${RTM_LINKER_OPTION})
	add_dependencies(${target}Comp ${RTM_PROJECT_NAME})

	add_library(${target} SHARED ${srcs})
	target_link_libraries(${target} ${libs} ${RTM_LINKER_OPTION})
	add_dependencies(${target} ${RTM_PROJECT_NAME})
	set_target_properties(${target} PROPERTIES PREFIX "")

	install(TARGETS ${target}Comp LIBRARY DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				ARCHIVE DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				RUNTIME DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				COMPONENT examples)

	install(TARGETS ${target} LIBRARY DESTINATION ${INSTALL_RTM_EXAMPLE_SHAREDLIB_DIR}
				ARCHIVE DESTINATION ${INSTALL_RTM_EXAMPLE_SHAREDLIB_DIR}
				RUNTIME DESTINATION ${INSTALL_RTM_EXAMPLE_SHAREDLIB_DIR}
				COMPONENT examples)

	install(FILES ${standalone_srcs} DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}/src/StaticFSM COMPONENT sources)

endif()


if(VXWORKS)
	if(RTP)
		set_target_properties(${target}Comp PROPERTIES SUFFIX ".vxe")
	else(RTP)	
		set_target_properties(${target} PROPERTIES SUFFIX ".out")
	endif(RTP)
endif(VXWORKS)



set(target Microwave)



set(srcs Microwave.cpp Microwave.h MicrowaveFsm.cpp MicrowaveFsm.h)
set(standalone_srcs MicrowaveComp.cpp ${srcs})

if(VXWORKS AND NOT RTP)
	set(standalone_srcs ${standalone_srcs} ${CMAKE_SOURCE_DIR}/src/lib/rtm/DataFlowComponentBase.cpp ${CMAKE_SOURCE_DIR}/src/lib/rtm/Macho.cpp)
	set(libs ${RTCSKEL_PROJECT_NAME})

	add_executable(${target} ${standalone_srcs})
	target_link_libraries(${target} ${libs})
	add_dependencies(${target} ${RTM_PROJECT_NAME})

	install(TARGETS ${target} LIBRARY DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				ARCHIVE DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				RUNTIME DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				COMPONENT examples)
else()
	if(VXWORKS)
		set(libs ${RTM_PROJECT_NAME} ${ORB_LIBRARIES} ${COIL_PROJECT_NAME} ${RTCSKEL_PROJECT_NAME})
	else(VXWORKS)
		set(libs ${RTM_PROJECT_NAME} ${ORB_LIBRARIES} ${COIL_PROJECT_NAME})
	endif(VXWORKS)

	add_executable(${target}Comp ${standalone_srcs})
	target_link_libraries(${target}Comp ${libs} ${RTM_LINKER_OPTION})
	add_dependencies(${target}Comp ${RTM_PROJECT_NAME})

	add_library(${target} SHARED ${srcs})
	target_link_libraries(${target} ${libs} ${RTM_LINKER_OPTION})
	add_dependencies(${target} ${RTM_PROJECT_NAME})
	set_target_properties(${target} PROPERTIES PREFIX "")

	install(TARGETS ${target}Comp LIBRARY DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				ARCHIVE DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				RUNTIME DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}
				COMPONENT examples)

	install(TARGETS ${target} LIBRARY DESTINATION ${INSTALL_RTM_EXAMPLE_SHAREDLIB_DIR}
				ARCHIVE DESTINATION ${INSTALL_RTM_EXAMPLE_SHAREDLIB_DIR}
				RUNTIME DESTINATION ${INSTALL_RTM_EXAMPLE_SHAREDLIB_DIR}
				COMPONENT examples)

	install(FILES ${standalone_srcs} DESTINATION ${INSTALL_RTM_EXAMPLE_DIR}/src/StaticFSM COMPONENT sources)

endif()


if(VXWORKS)
	if(RTP)
		set_target_properties(${target}Comp PROPERTIES SUFFIX ".vxe")
	else(RTP)	
		set_target_properties(${target} PROPERTIES SUFFIX ".out")
	endif(RTP)
endif(VXWORKS)
