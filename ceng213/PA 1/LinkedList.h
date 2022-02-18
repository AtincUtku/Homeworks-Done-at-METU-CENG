#ifndef LINKEDLIST_H
#define LINKEDLIST_H


#include <iostream>

#include "Node.h"

template<class T>
class LinkedList {
public: // DO NOT CHANGE THIS PART.
    LinkedList();
    LinkedList(const T arr[], int arrSize);
    LinkedList(const LinkedList<T> &obj);

    ~LinkedList();

    Node<T> *getFirstNode() const;
    Node<T> *getLastNode() const;
    Node<T> *getNode(const T &data) const;

    int getNumberOfNodes() const;
    bool isEmpty() const;
    bool contains(Node<T> *node) const;

    void insertAtTheHead(const T &data);
    void insertAtTheTail(const T &data);
    void insertSorted(const T &data);

    void removeNode(Node<T> *node);
    void removeNode(const T &data);
    void removeAllNodes();

    void print() const;

    T *toArray() const;

    LinkedList<T> &operator=(const LinkedList<T> &rhs);

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.

private: // DO NOT CHANGE THIS PART.
    Node<T> *head;
    Node<T> *tail;
};

template<class T>
LinkedList<T>::LinkedList() 
{
    head = nullptr;
    tail = nullptr;

}

template<class T>
LinkedList<T>::LinkedList(const T arr[], int arrSize) 
{
    if (arrSize == 0)
    {
        head = nullptr;
        tail = nullptr;
    }
    
    else
    {
        head = new Node<T>;
        head->data = arr[0];
        head->prev = nullptr;
        head->next = nullptr;
        tail = head;
      
        
       Node<T> *curr;
        
        for (int i=1; i<arrSize; i++)
        {
            curr = new Node<T>;
            
            
            
            curr->data = arr[i];
            curr->prev = tail;
            curr->next = nullptr;
            
            tail->next = curr;
            
            tail = tail->next;
            
            curr = nullptr;
        }
        
        
            
    }
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &obj) 
{
    Node<T> *curr;
    Node<T> *obj_curr;
    
    if (obj.head == nullptr)
    {
        head = nullptr;
        tail = nullptr;
    }
    
    else
    {
        head = new Node<T>((obj.head)->data, nullptr, nullptr);
        curr = head;
        obj_curr = (obj.head)->next;
        
        while (obj_curr != nullptr)
        {
            curr->next = new Node<T>(obj_curr->data, curr, nullptr);
            curr = curr->next;
            obj_curr = obj_curr->next;
        }
        
        tail = curr;
    }
}

template<class T>
LinkedList<T>::~LinkedList() 
{
    Node<T> *curr = head;
    Node<T> *after;
    
    while (curr != nullptr)
    {
        after = curr->next;
        delete curr;
        curr = after;
        
    }
    
    
}

template<class T>
Node<T> *LinkedList<T>::getFirstNode() const 
{
    if (head == nullptr)
    {
        return nullptr;
    }
    
    else
    {
        Node<T> *curr = head;
        return curr;
    }
}

template<class T>
Node<T> *LinkedList<T>::getLastNode() const 
{
    if (head == nullptr)
    {
        return nullptr;
    }
    
    else
    {
        Node<T> *curr = head;
        while (curr->next != nullptr)
        {
            curr = curr->next;
        }
        
        return curr;
        
    }    
}

template<class T>
Node<T> *LinkedList<T>::getNode(const T &data) const 
{
    Node<T> *curr = head;
    int control = 0;
    
    while (curr != nullptr)
    {
        if (curr->data == data)
        {
            control = 1;
            return curr;
        }
        
        else
        {
            curr = curr->next;
        }
    }
    
    if (control == 0)
    {
        return nullptr;
    }
    
    
}

template<class T>
int LinkedList<T>::getNumberOfNodes() const 
{
    int count = 0;
    Node<T> *curr = head;
    
    while (curr != nullptr)
    {
        count++;
        curr = curr->next;
    }
    
    return count;
}

template<class T>
bool LinkedList<T>::isEmpty() const 
{
    if (head == nullptr)
    {
        return true;
    }
    
    else
    {
        return false;
    }
}

template<class T>
bool LinkedList<T>::contains(Node<T> *node) const 
{
    Node<T> *curr = head;
    int control = 0;
    
    while (curr != nullptr)
    {
        if ((curr->data == node->data) && (curr->prev == node->prev) && (curr->next == node->next))
        {
            control = 1;
            return true;
        }
        
        else
        {
            curr = curr->next;
        }
    }
    
    if (control == 0)
    {
        return false;
    }
    

}

template<class T>
void LinkedList<T>::insertAtTheHead(const T &data) 
{
    if (head == nullptr)
    {
        Node<T> *tmp = new Node<T>;
        tmp->data = data;
        tmp->prev = nullptr;
        tmp->next = nullptr;
        
        head = tmp;
        tail = tmp;
    }
    
    else
    {
        Node <T> *curr = new Node<T>;
        curr->data = data;
        curr->prev = nullptr;
        curr->next = head;
        
        head = curr;
    }
}



template<class T>
void LinkedList<T>::insertAtTheTail(const T &data) 
{
    if (head == nullptr)
    {
        Node<T> *tmp = new Node<T>;
        tmp->data = data;
        tmp->prev = nullptr;
        tmp->next = nullptr;
        
        head = tmp;
        tail = tmp;        
    }
    
    else
    {
        Node<T> *curr = new Node<T>;
        curr->data = data;
        curr->prev = tail;
        curr->next = nullptr;
        
        tail->next = curr;
        tail = curr;
    }
}

template<class T>
void LinkedList<T>::insertSorted(const T &data) 
{
    Node<T> *curr = head;
    Node<T> *x = new Node<T>;
    x->data = data;
    x->prev = nullptr;
    x->next = nullptr;
    
    
    
    if (head == nullptr)
    {
       head = x;
       tail = x;
    }
    
    else if (head && head->data >= x->data)
    {
        x->next = head;
        x->next->prev = x;
        head = x;
    }
    
    else
    {
        while (curr && curr->next != nullptr && curr->next->data < x->data)
        {
            curr = curr->next;
        }
        
        x->next = curr->next;
        
        if (curr && curr->next != nullptr)
        {
            x->next->prev = x;
        }
        
        curr->next = x;
        x->prev = curr;
        
        if (x->next == nullptr)
        {
            tail = x;
        }
    }
}

template<class T>
void LinkedList<T>::removeNode(Node<T> *node) 
{
    
    if (head == nullptr || node == nullptr)
    {
        return;
    }
    
    else
    {
        if (head == node)
        {
            head = node->next;
        }
    
        if (node->next != nullptr)
        {
            node->next->prev = node->prev;
        }
    
        if (node->prev != nullptr)
        {
            node->prev->next = node->next;
        }
        
        if (node->next == nullptr)
        {
            tail = node->prev;
        }
    
        delete node;
    }
}

template<class T>
void LinkedList<T>::removeNode(const T &data) 
{
    Node<T> *curr = head;
    
    
    if (head == nullptr)
    {
        return ;
    }
    
    else
    {
        while (curr != nullptr)
        {
            if (curr->data == data)
            {
                break;
            }
            
            curr = curr->next;
        }
       
    this->removeNode(curr);    
        
    }

}

template<class T>
void LinkedList<T>::removeAllNodes() 
{
    Node<T> *curr;

    while (head != nullptr)
    {
        curr = head;
        head = head->next;
        delete curr;
    }
}

template<class T>
void LinkedList<T>::print() const {
    std::cout << "Printing the linked list ..." << std::endl;

    if (this->isEmpty()) {
        std::cout << "empty" << std::endl;
        return;
    }

    Node<T> *node = this->head;

    while (node) {
        std::cout << *node << std::endl;
        node = node->next;
    }
}

template<class T>
T *LinkedList<T>::toArray() const 
{
    if (head == nullptr)
    {
        return nullptr;
    }
    
    else
    {
        int count;
        count = this->getNumberOfNodes();
        
        T *arr;
        arr = new T[count];
        
        Node<T> *curr = head;
        
        for (int i=0; i<count; i++)
        {
            if (curr)
            {
                arr[i] = curr->data;
                curr = curr->next;
            }
        }
        
        return arr;
    }

}

template<class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &rhs) 
{
    if (this == &rhs)
    {
        return *this;
    }
    
    else
    {
        
        if (rhs.head == nullptr)
        {
            head = nullptr;
            tail = nullptr;
        }
        
        else
        {
            head = new Node<T>;
            (this->head)->data = (rhs.head)->data;
            Node<T> *curr = head;
            Node<T> *tmp = rhs.head->next;
            
            while (tmp != nullptr)
            {
                curr->next = new Node<T>;
                curr->next->data = tmp->data;
                curr->next->prev = curr;
                curr->next->next = nullptr;
                
                curr = curr->next;
                tmp = tmp->next;
                
            }
            
            tail = curr;
            
        }
        
        return *this;
    }
}


#endif //LINKEDLIST_H
