//
// Created by antoine on 08/05/2020.
//

#ifndef KENT_CONTROL_FLOW_ERROR_H
#define KENT_CONTROL_FLOW_ERROR_H

#define LOG(...) fprintf(stdout, __VA_ARGS__)
#define ERROR(msg) (fprintf(stderr, "Oops, something went wrong\nIn file: %s\n\
In function: %s\nLine: %d\nMessage: %s\n", __FILE__, __func__, __LINE__, msg))

#endif //KENT_CONTROL_FLOW_ERROR_H
