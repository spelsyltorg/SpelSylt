#pragma once


#ifdef _DEBUG
#pragma comment(lib, "SFML/Lib/sfml-audio-d.lib")
#pragma comment(lib, "SFML/Lib/sfml-graphics-d.lib")
#pragma comment(lib, "SFML/Lib/sfml-main-d.lib")
#pragma comment(lib, "SFML/Lib/sfml-network-d.lib")
#pragma comment(lib, "SFML/Lib/sfml-system-d.lib")
#pragma comment(lib, "SFML/Lib/sfml-window-d.lib")
#else
#pragma comment(lib, "SFML/Lib/sfml-audio.lib")
#pragma comment(lib, "SFML/Lib/sfml-graphics.lib")
#pragma comment(lib, "SFML/Lib/sfml-main.lib")
#pragma comment(lib, "SFML/Lib/sfml-network.lib")
#pragma comment(lib, "SFML/Lib/sfml-system.lib")
#pragma comment(lib, "SFML/Lib/sfml-window.lib")
#endif