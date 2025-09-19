/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalegria <aalegria@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:01:34 by aalegria          #+#    #+#             */
/*   Updated: 2024/09/30 17:52:15 by aalegria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_node;

	new_node = malloc(sizeof (t_list));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}

/*#include "libft.h"

int main(void)
{
    t_list *node;
    char *content = "Hola, mundo";

    node = ft_lstnew((void *)content);
    if (node)
    {
        printf("Contenido: %s\n", (char *)node->content);
    }
    return 0;
}*/
