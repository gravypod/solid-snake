//
// Created by gravypod on 9/18/17.
//

#ifndef ENGINE_IMPORTHISTORY_H
#define ENGINE_IMPORTHISTORY_H

#include <stdbool.h>


bool should_import(const char* filename);
void mark_imported(const char* filename);


#endif //ENGINE_IMPORTHISTORY_H
