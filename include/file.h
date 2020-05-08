//
// Created by antoine on 08/05/2020.
//

#ifndef KENT_CONTROL_FLOW_FILE_H
#define KENT_CONTROL_FLOW_FILE_H

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

const char **read_and_format_file(const char *path);

#endif //KENT_CONTROL_FLOW_FILE_H
