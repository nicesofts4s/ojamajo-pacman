set(SOURCES 
    main.cxx
    save.cxx

	states/intro_state.cxx
    states/insert_coin_state.cxx
    states/gameplay_state.cxx
    states/credits_state.cxx
    states/adjust_volume_state.cxx
    states/game_over_state.cxx
    states/select_character_state.cxx
    states/unlock_shorts_san_state.cxx
        
    gameplay/states/start_state.cxx
    gameplay/states/play_state.cxx
    gameplay/states/death_state.cxx
    gameplay/states/win_state.cxx

    gameplay/data/characters.cxx

    gameplay/player.cxx
    gameplay/ghost.cxx
    gameplay/ai/ghosts.cxx
)
    
add_executable(ojamajo-pacman WIN32 ${SOURCES})

target_compile_options(ojamajo-pacman PRIVATE "-Wno-narrowing")
target_compile_options(ojamajo-pacman PRIVATE "-Wno-deprecated-declarations")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIR})
	target_link_libraries(ojamajo-pacman ${OPENGL_LIBRARIES})
endif()

if(UNIX)
	find_package(SFML REQUIRED system window graphics audio)
	if(SFML_FOUND)
		include_directories(${SFML_INCLUDE_DIR})
		target_link_libraries(ojamajo-pacman ${SFML_LIBRARIES})
	endif()
endif()

if(WIN32)
	target_link_libraries(ojamajo-pacman ${CMAKE_SOURCE_DIR}/lib/libsfml-graphics.a)
	target_link_libraries(ojamajo-pacman ${CMAKE_SOURCE_DIR}/lib/libsfml-system.a)
	target_link_libraries(ojamajo-pacman ${CMAKE_SOURCE_DIR}/lib/libsfml-window.a)
    target_link_libraries(ojamajo-pacman ${CMAKE_SOURCE_DIR}/lib/libsfml-audio.a)

    include_directories("${CMAKE_SOURCE_DIR}/include")

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
  	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
  	set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS} -static-libgcc -s")
  	set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static-libgcc -static-libstdc++ -s")
endif()
