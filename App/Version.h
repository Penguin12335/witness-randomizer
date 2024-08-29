#pragma once

#define TO_STRING2(s) #s
#define TO_STRING(s) TO_STRING2(s)

#define MAJOR 3
#define MINOR 2
#define PATCH 4

#define VERSION_STR     TO_STRING(MAJOR) "." TO_STRING(MINOR) "." TO_STRING(PATCH)
#define VERSION			MAJOR, MINOR, PATCH

#define PRODUCT_NAME L"Witness Random Puzzle Generator"
#define WINDOW_CLASS L"WitnessRandomizer"
