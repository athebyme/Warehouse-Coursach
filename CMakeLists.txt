cmake_minimum_required(VERSION 3.12)
project(Warehouse)

set(CMAKE_CXX_STANDARD 14)

add_executable(
	Warehouse
	main.cpp
	Creatures/Human.h
	Warehouse/Logic.cpp
	Warehouse/Logic.h
	Exceptions/Exceptions.h)
