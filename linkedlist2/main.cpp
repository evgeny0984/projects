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
    Node *temp = nullptr; 
    while (current != nullptr)
    {
        temp = current;
        current = current->next;
        delete temp;
    }
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
    int count = 1;
    cout << "#\tField" << endl;
    while (current != nullptr)
    {
        cout << count << '\t' << current->data << endl;
        current = current->next;
        count++;  
    }
    cout << "Total: " << size << endl;
}

int main()
{
    List lst;
    string str;
    cout << "Linked list. Enter symbols or space to quit.\n>"; 
    getline(cin, str);
    while (isspace(str[0]) == 0)
    {
        lst.addNode(str);
        cout << ">";
        getline(cin, str);
    }
    lst.printList();
    return 0;
}
