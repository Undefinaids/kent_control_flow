//
// Created by antoine on 19/05/2020.
//

#ifndef NEW_CONTROL_FLOW_ERROR_H
#define NEW_CONTROL_FLOW_ERROR_H

#include <stdio.h>

#define LOG(...) fprintf(stdout, __VA_ARGS__)
#define ERROR(msg) (fprintf(stderr, "Oops, something went wrong\nIn file: %s\n\
In function: %s\nLine: %d\nMessage: %s\n", __FILE__, __func__, __LINE__, msg))

#endif //NEW_CONTROL_FLOW_ERROR_H
