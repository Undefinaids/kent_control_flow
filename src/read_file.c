//
// Created by antoine on 08/05/2020.
//

#include "file.h"

static char *read_file(const char *path) {
    int fd;
    struct stat sb;
    char *buf;

    if ((fd = open(path, O_RDONLY)) == -1) {
        ERROR("Failed to open the file");
        return (NULL);
    }
    if (fstat(fd, &sb) == -1) {
        ERROR("Failed to fstat");
        return (NULL);
    }
    buf = calloc(sizeof(char), sb.st_size + 1);
    if (buf == NULL) {
        ERROR("Failed to calloc the buffer");
        return (NULL);
    }
    if (read(fd, buf, sb.st_size) == -1) {
        ERROR("Failed to open the file");
        return (NULL);
    }
    for (int i = sb.st_size - 1; i >= 0 && buf[i] == '\n'; --i)
        buf[i] = '\0';
    close(fd);
    return (buf);
}

static const char *get_line(char *buf, int *index) {
    int i = 0;
    char *res;

    for (; buf[*index + i] != '\0' && buf[*index + i] != '\n'; ++i);
    res = strndup(buf + *index, i);
    *index += i + 1;
    return (res);
}

static const char **format_text(char *content) {
    int lines = 1;
    const char **formatted_text;
    int index = 0;

    for (int i = 0; content[i] != '\0'; ++i) {
        content[i] == '\n' ? ++lines : 0;
    }
    if ((formatted_text = calloc(sizeof(char*), lines + 1)) == NULL) {
        ERROR("Failed to calloc the buffer");
        return (NULL);
    }
    for (int i = 0; i < lines; ++i)
        formatted_text[i] = get_line(content, &index);
    return (formatted_text);
}

const char **read_and_format_file(const char *path) {
    char *content;
    const char **lines;

    if ((content = read_file(path)) == NULL) {
        return (NULL);
    }
    lines = format_text(content);
    free(content);
    return (lines);
}