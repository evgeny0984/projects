//Односвязный список
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100
#define BLANK "<empty>"

struct node
{
    int index;
    char data[SIZE];
    struct node *next;
};

struct list
{
    int size;
    struct node *head;
}lst;

struct node *current;
struct node *create_node();
void insert_last();
void update_node(int, char*);
void delete_node(int);
void delete_all();
void print_all();

int main()
{
    lst.size = 0;
    lst.head = NULL;
    int ch = 0, num;
    char line[SIZE], *ptr = line;
    printf("Linked list. Available commands:\na - add item, e - edit item, r - remove item, p - print list, q - quit.\n");
    while (ch != 'q')
    {
        putchar('>');         
        ch = getchar();
        switch (ch)
        {
        case 'a':
            insert_last();
            break;
        case 'e':
            printf("Enter number to edit: ");
            scanf("%d", &num);
            printf("And value: ");
            scanf("%s", ptr);
            update_node(num, ptr);
            break;
        case 'r':
            printf("Enter number to remove: ");
            scanf("%d", &num);
            delete_node(num);
            break;
        case 'p':
            print_all();
            break;
        case 'q':
            printf("Done.\n");
            break;
        default:
            printf("Command not found.\n");
            break;
        }
        while (getchar() != '\n');
    }
    delete_all();
    return 0;
}

struct node *create_node()
{
    struct node *ptr = (struct node*)malloc(sizeof(struct node));
    static int count = 1;
    ptr->index = count;
    strcpy(ptr->data, BLANK);
    ptr->next = NULL;
    count++;
    return ptr;
}

void insert_last()
{
    struct node *temp;
    int count = lst.size;
    if (lst.head == NULL)
    {
        lst.head = create_node();
    }
    else
    {
        current = lst.head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        temp = create_node();
        current->next = temp;
    }
    printf("New record added.\n");
    count++;
    lst.size = count;
}

void update_node(int i, char *d)
{
    if (lst.head == NULL)
    {
        printf("No records.\n");
        return;
    }
    current = lst.head;
    while (current != NULL)
    {
        int val = current->index;
        if (val == i)
        {
            strcpy(current->data, d);
            printf("Record %d changed.\n", i);
            break;
        }
        current = current->next;
    }
    if (current == NULL)
    {
        printf("Record not found.\n");
    }
}

void delete_node(int i)
{
    struct node *temp = NULL;
    int count = lst.size;
    if (lst.head == NULL)
    {
        printf("No records.\n");
        return;
    }
    current = lst.head;
    while (current->index != i)
    {
        temp = current;
        current = current->next;
        if (current == NULL)
        {
            printf("Record not found.\n");
            return;
        }
    }
    int val = current->index;
    if (val == i)
    {
        if (temp == NULL)
        {
            lst.head = current->next;
        }
        else
        {
            temp->next = current->next;
        }
        free(current);
        count--;
        lst.size = count;
        printf("Record %d removed.\n", i);
    }
}

void delete_all()
{
    struct node *temp = NULL;
    current = lst.head;
    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp);
    }
}

void print_all()
{
    current = lst.head;
    printf("#\tField\n");
    while (current != NULL)
    {
        printf("%d\t%s\n", current->index, current->data);
        current = current->next;
    }
    printf("Total: %d\n", lst.size);
}
