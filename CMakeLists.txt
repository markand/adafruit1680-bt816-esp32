cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME pb)
set(PROJECT_VER 0.1.0)

set(
	COMPONENTS 

	# ESP.
	console
	driver
	main
	spi
)

set(PB_PIN_CS "10" CACHE STRING "Chip Select GPIO pin")
set(PB_PIN_PD "11" CACHE STRING "Power down GPIO pin")
set(PB_PIN_MOSI "7" CACHE STRING "SPI MOSI GPIO pin")
set(PB_PIN_MISO "2" CACHE STRING "SPI MISO GPIO pin")
set(PB_PIN_SCLK "6" CACHE STRING "SPI clock GPIO pin")

configure_file(
	${CMAKE_SOURCE_DIR}/sysconfig.h
	${CMAKE_BINARY_DIR}/sysconfig.h
)
include_directories(${CMAKE_BINARY_DIR})

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(pb)
