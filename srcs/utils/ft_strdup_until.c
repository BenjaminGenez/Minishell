#include "minishell.h"

char *ft_strdup_until(const char *str, char c)
{
    size_t len = 0;
    char *result;

    // Find the length up to the character `c`
    while (str[len] && str[len] != c)
        len++;

    // Allocate memory for the new string
    result = malloc(len + 1);
    if (!result)
        return (NULL);

    // Copy the characters up to `c`
    for (size_t i = 0; i < len; i++)
        result[i] = str[i];

    result[len] = '\0'; // Null-terminate the string
    return (result);
}