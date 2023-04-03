//Односвязный список
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100

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
    int ch, num;
    char line[SIZE], *ptr = line;
    printf("Linked list. Available commands:\ni - add node, u - edit node, d - remove node, p - show list, q - quit.\n");
    while (ch != 'q')
    {
        ch = getchar();
        switch (ch)
        {
        case 'i':
            insert_last();
            break;
        case 'u':
            printf("Enter number and value of record: ");
            scanf("%d%s", &num, ptr);
            update_node(num, ptr);
            break;
        case 'd':
            printf("Enter number of record: ");
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
    static int count = 1;
    struct node *ptr = (struct node*)malloc(sizeof(struct node));
    ptr->index = count;
    strcpy(ptr->data, "<empty>");
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
    current = lst.head;
    while (current != NULL)
    {
        int val = current->index;
        if (val == i)
        {
            strcpy(current->data, d);
            printf("Record %d changed.\n", i);
        }
        current = current->next;
    }
}

void delete_node(int i)
{
    struct node *temp;
    int count = lst.size;
    if (lst.head == NULL)
    {
        return;
    }
    current = lst.head;
    temp = NULL;
    while (current != NULL)
    {
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
        temp = current;
        current = current->next;
    }
}

void delete_all()
{
    struct node *tmp = lst.head;
    current = lst.head;
    while (current != NULL)
    {
        current = current->next;
        free(current);
    }
    free(tmp);
    lst.size = 0;
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

