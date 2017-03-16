#ifndef ERRORS_H
#define ERRORS_H
#include <string>

//возможные ошибки работы функций
enum tERRORS { OK = 0,
              FILE_NOT_FIND,
              FILE_ERROR,
              MEMORY_ERROR,
              OBJECT_EMPTY,
              SCENE_NOT_FOUND
            };

int GetMessage(std::string &str, tERRORS er);

#endif // ERRORS_H
