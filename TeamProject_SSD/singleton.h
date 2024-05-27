#pragma once

#define MAKESINGLE(classname)									\
public:															\
	static classname& getInstance() {							\
		static classname instance{};							\
		return instance;										\
	}															\
																\
private:														\
	classname() { }												\
	classname& operator=(const classname& other) = delete;		\
	classname(const classname& other) = delete;					\