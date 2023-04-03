//Односвязный список
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

struct Node
{
    string data;
    Node *next;
};

class List
{
private:
    Node *head;
    int size;
public:
    List();
    ~List();
    void addNode(string);
    void printList();
};

List::List()
{
    head = nullptr;
    size = 0;
}

List::~List()
{
    Node *current = head;
    while (current != nullptr)
    {
        current = current->next;
        delete current;
    }
    delete head;
}

void List::addNode(string str)
{
    Node *ptr = new Node;
    ptr->data = str;
    ptr->next = nullptr;
    if (head == nullptr)
    {
        head = ptr;
    }
    else
    {
        Node *current = head;
        while (current->next != nullptr)
        {
            current = current->next;
        }
        current->next = ptr;
    }
    size++;
}

void List::printList()
{
    Node *current = head;
    cout << "Field" << endl;
    while (current != nullptr)
    {
        cout << current->data << endl;
        current = current->next;
    }
    cout << "Total: " << size << endl;
}

int main()
{
    List lst;
    string str;
    cout << "Linked list. Enter the data:" << endl;
    getline(cin, str);
    do
    {
        lst.addNode(str);
        getline(cin, str);
    }
    while (isspace(str[0]) == 0);
    lst.printList();
    return 0;
}

