/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pyerima <pyerima@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 16:12:14 by pyerima           #+#    #+#             */
/*   Updated: 2023/11/26 16:14:52 by pyerima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <stdlib.h>
#include <stdio.h>

void	create_list(t_list **list, int fd)
{
	int		char_read;
	char	*buf;

	//check through the line if '\n' is present
	//printf("Createlist1: %p\n", *list);
	while (!found_newline(*list))
	{
		buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (buf == NULL)
			return ;
		//note(Buffer_size is set in the header)
		char_read = read(fd, buf, BUFFER_SIZE);
		if (char_read <= 0)
		{
			free(buf);
			return ;
		}
		//printf("%d\n", char_read);
		//add '\0' to the end of buf
		buf[char_read] = '\0';
		//add the buf ro the list
		append(list, buf);
		//printf("Createlist2: %p\n", *list);
		//printf("%s\n", (*list)->str_buf);
	}
}

void	append(t_list **list, char *buf)
{
	t_list	*new_node;
	t_list	*last_node;

	last_node = find_last_node(*list);
	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return ;
	//if the list is empty(*list = NULL)
	if (NULL == last_node)
		*list = new_node;
	else
		last_node->next = new_node;
	new_node->str_buf = buf;
	new_node->next = NULL;
}

//get (line \n)
char	*get_line(t_list *list)
{
	int		str_len;
	char	*next_str;

	if (list == NULL)
		return (NULL);
	//count how many chars until '\n'
	str_len = len_to_newline(list);
	next_str = malloc(str_len + 1);
	if (next_str == NULL)
		return (NULL);
	//copy the string in the buffer and return it
	copy_str(list, next_str);
	return (next_str);
}

/*
	polish (clean) linked list for next call
*/

void	polish_list(t_list **list)
{
	t_list	*last_node;
	t_list	*clean_node;
	int		i;
	int		k;
	char	*buf;

	buf = malloc(BUFFER_SIZE + 1);
	clean_node = malloc(sizeof(t_list));
	if (buf == NULL || clean_node == NULL)
		return ;
	//printf("Allocating: %p\n", clean_node);
	//printf("List: %p\n", *list);
	last_node = find_last_node(*list);
	i = 0;
	k = 0;
	//skip all the chars before '\0' and '\n'
	while (last_node->str_buf[i] != '\0' && last_node->str_buf[i] != '\n')
		i++;
	while (last_node->str_buf[i] != '\0' && last_node->str_buf[++i])
		buf[k++] = last_node->str_buf[i];
	buf[k] = '\0';
	clean_node->str_buf = buf;
	clean_node->next = NULL;
	dealloc(list, clean_node, buf);
}

char	*get_next_line(int fd)
{
	static t_list	*list[1024];
	char			*next_line;

	//list = NULL;
	//note- fd can only be positive || read function gives out -1 if the file cannot be opened
	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0 || read(fd, &next_line, 0) < 0)
	{
		return (NULL);
	}
	//creat the list till it gets a '\n
	create_list(&list[fd], fd);
	if (list[fd] == NULL)
		return (NULL);
	//fetch the line from list
	next_line = get_line(list[fd]);
	//
	polish_list(&list[fd]);
	return (next_line);
}
