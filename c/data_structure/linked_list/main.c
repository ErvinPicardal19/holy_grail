#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;


void push(Node** p_head, int num);
void insert(Node **p_head, int pos, int num);
void pop(Node** head);
void delete(Node** head, int pos);
void print_linked_list(Node* head);
void clean(Node** head);

int main()
{
    Node* head;
    head = NULL;
    int input, choice, pos;
    while(1)
    {
        puts("<---LINKED LISTS--->"); 
        puts("1. Push Node");
        puts("2. Insert Node");
        puts("3. Pop Node");
        puts("4. Delete Node");
        puts("5. Exit");
        printf("Option: ");
        scanf("%d", &choice);
        switch(choice)
        {
            case 1:
                printf("Enter integer to push: ");
                scanf("%d", &input);
                push(&head, input);
            break;
            case 2:
                printf("Enter integer to insert: ");
                scanf("%d", &input);
                printf("Enter position to insert: ");
                scanf("%d", &pos);
                insert(&head, pos, input);
            break;
            case 3:
                puts("Popping Stack...");
                pop(&head);
            break;
            case 4:
                printf("Enter position to delete: ");
                scanf("%d", &pos);
                delete(&head, pos);
            break;
            case 5:
                clean(&head);
                return 0;
            break;
        }
        print_linked_list(head);
        puts("\n");
    }
}


void push(Node** p_head, int num)
{
    Node* temp;
    temp = (Node*)malloc(sizeof(Node));
    temp->data = num;
    temp->next = NULL;

    if(*p_head == NULL)
    {
        *p_head = temp;
        return;
    }

    temp->next = *p_head;
    *p_head = temp;
    temp = NULL;

}

void insert(Node **p_head, int pos, int num)
{
    Node* curr = *p_head;
    Node* tempNode = (Node*)malloc(sizeof(Node));
    tempNode->data = num;
    
    if (curr == NULL)
    {
        push(p_head, num);
        return;
    }

    if (pos == 1) {
        push(p_head, num);
        return;
    };

    for(int i = 1; i < pos-1; i++)
    {
        if(curr->next != NULL) 
        {
            curr = curr->next;
        }
        else
        {
            puts("Position does not exist");
            return;
        } 
    }
    tempNode->next = curr->next;
    curr->next = tempNode;
    curr = NULL;
    tempNode = NULL;
}

void pop(Node** head)
{
    if (*head == NULL)
    {
        puts("Nothing to pop...");
        return;
    }

   if ((*head)->next == NULL)
    {
        free(*head);
        *head = NULL;
        return;
    }

    Node* temp = *head;
    *head = temp->next;
    free(temp);
    temp = NULL;
}

void delete(Node** head, int pos)
{
    Node* temp = *head;
    int i = 0;
    
    if(temp == NULL)
        return;

    if(pos == 1)
    {
        pop(head);
        return;
    }

    while(i < pos-2)
    {
        temp = temp->next;
        i++;
    }

    Node* p_del_node = temp->next;
    temp->next = p_del_node->next;
    p_del_node->next = NULL;
    free(p_del_node);
    p_del_node = NULL;
    temp = NULL;
}


void print_linked_list(Node* head)
{

    if(head == NULL) return;

    while(head != NULL)
    {
        printf("%d\t==>\t", head->data);
        head = head->next;
    }
    printf("NULL");
}

void clean(Node** head)
{
    Node* temp = *head;

    while(temp != NULL)
    {
        temp = temp->next;
        free(*head);
        *head = temp;
    }

    puts("\nLinked list cleaned.");
}
