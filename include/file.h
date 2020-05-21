//
// Created by antoine on 19/05/2020.
//

#ifndef NEW_CONTROL_FLOW_FILE_H
#define NEW_CONTROL_FLOW_FILE_H

#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "error.h"

const char **read_and_format_file(const char *path);

#endif //NEW_CONTROL_FLOW_FILE_H
