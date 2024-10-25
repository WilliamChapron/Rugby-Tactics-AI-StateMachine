#pragma once

#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DEBUG_NEW
#endif


#ifdef SFML_MODE
	#include <SFML/Graphics.hpp>
	#include <SFML/Window.hpp>
	#include <SFML/System.hpp>
#endif

#include <windows.h>

#include <wingdi.h>
#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <cstdlib>
#include <chrono>