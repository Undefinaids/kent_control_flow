//
// Created by antoine on 19/05/2020.
//

#include "error.h"
#include "utils.h"

int is_alphanum(char c) {
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

char *epur_str(char *str) {
	char *res;
	int length = 0;

	for (int i = 0; str[i] != '\0'; ++i)
		if (is_alphanum(str[i]))
			++length;
	if ((res = calloc(sizeof(char), length + 1)) == NULL)
		return (NULL);
	for (int i = 0, j = 0; str[i] != '\0'; ++i) {
		if (is_alphanum(str[i])) {
			res[j] = str[i];
			++j;
		}
	}
	return (res);
}

char *extract_first_label(char *str) {
	unsigned int i = 0;
	char *duplicated;
	char *result;

	while (str[i] != '\0' && is_alphanum(str[i]) == 0)
		i++;
	while (str[i] != '\0' && is_alphanum(str[i]) == 1)
		i++;
	duplicated = strndup(str, i);
	result = epur_str(duplicated);
	free(duplicated);
	return (result);
}