#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct list_head
{
    struct list_head *prev;
    struct list_head *next;
};

struct my_data
{
    int data;
    struct list_head head;
};

#define INIT_LIST_HEAD(ptr) \
do \
{ \
    (ptr)->next = (ptr); \
    (ptr)->prev = (ptr); \
} while(0)

void add_entry(struct list_head *new, struct list_head *head)
{
    new->next = head->next;
    new->prev = head;
    head->next->prev = new;
    head->next= new;
}

void del_entry(struct list_head *pos)
{
    pos->prev->next = pos->next;  
    pos->next->prev = pos->prev;
    free(pos);
}

void get_head_info(struct list_head *head)
{
    printf("head address: %p\n", head);
    printf("next pointer: %p\n", head->next);
    printf("prev pointer: %p\n\n", head->prev);
}

#define list_for_each(pos, head) \
for (pos = (head)->next; pos != (head); pos = pos->next) 

void clean_list(struct list_head *head)
{
    struct list_head *pos;
    struct list_head *temp;
    for(pos = head->next; pos != head; )
    {
        struct my_data *entry = (struct my_data *)((char *)pos - offsetof(struct my_data, head));
        temp = entry->head.next;
        free(entry);
        pos = temp;
    }
}

int main(int argc, char *argv[])
{
	struct list_head head;

    INIT_LIST_HEAD(&head);
    
    struct my_data *d1 = (struct my_data *) malloc(sizeof(struct my_data));
    struct my_data *d2 = (struct my_data *) malloc(sizeof(struct my_data));
    
    d1->data = 10;
    d2->data = 20;

    add_entry(&d2->head, &head);
    add_entry(&d1->head, &head);

    struct list_head *pos;
    list_for_each(pos, &head)
    {
        struct my_data *entry = (struct my_data *)((char *)pos - offsetof(struct my_data, head));
        printf("Data: %d\n", entry->data);
    }

    clean_list(&head);

	return 0;
}
