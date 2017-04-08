#include "errors.h"

int GetMessage(std::string &str, tERRORS er)
{
    switch(er) {
    case OK:
        str = "No errors\n";
        break;
    case FILE_ERROR:
        str = "Error in file\n";
        break;
    case FILE_NOT_FIND:
        str = "File not found\n";
        break;
    case MEMORY_ERROR:
        str = "Error in allocating memory\n";
        break;
    case OBJECT_EMPTY:
        str = "No object\n";
        break;
    case SCENE_NOT_FOUND:
        str = "Error: scene not found\n";
        break;
    default:
        str = "Unknown error";
        break;
    }

    return 0;
}
