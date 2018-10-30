#pragma once
#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);


void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Deletes a buffer
#define RELEASE( x ) \
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

// Transforms
#define radiansToDegrees(x) ((x*180.0f)/3.141592f)
#define degreesToRadians(x) ((x*3.141592f)/180.0f)

// Configuration -----------
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FULLSCREEN false
#define RESIZEABLE true
#define SCREEN_SIZE 2
#define TITLE "QT engine"