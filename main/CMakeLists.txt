set(
	SOURCES
	eve.c
	eve.h
	eve_esp32.c
	main.c
)

idf_component_register(SRCS ${SOURCES})
target_compile_definitions(
	${COMPONENT_LIB}
	PRIVATE
		PB_VERSION="${PROJECT_VER}"
		EVE_ESP32
)
