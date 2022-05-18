#include <iostream>
#include "../include/citizens.hpp"
using namespace std;

citizen::citizen(int id_a, string onoma, string epitheto, int ilikia, country *nationality)
{
    id = id_a;
    name = onoma;
    lastname = epitheto;
    age = ilikia;
    citizen_country = nationality;
    next = NULL;
}

map_citizens::map_citizens()
{
    head = NULL;
};

map_citizens::~map_citizens()
{
    citizen *del;
    while (head != NULL) // Delete list nodes one by one
    {
        del = head;
        head = head->next;
        delete del;
    }
};

citizen *map_citizens::insert(int id, string onoma, string epitheto, country *nationality, int ilikia)
{

    citizen *temp = NULL;
    if (head == NULL) // Empty bucket becomes the new node
    {
        citizen *new_node = new citizen(id, onoma, epitheto, ilikia, nationality);
        new_node->next = NULL;
        head = new_node;
        return head;
    }
    else
    {
        citizen *last = head;
        while (last != NULL)
        {
            if (last->id == id)
            {
                if (last->name != onoma || last->lastname != epitheto || last->age != ilikia || last->citizen_country->cntr != nationality->cntr)
                {
                    return NULL;
                }
                return last;
            }
            temp = last;
            last = last->next;
        }
        citizen *new_node = new citizen(id, onoma, epitheto, ilikia, nationality); // Insert new node
        new_node->next = NULL;
        temp->next = new_node;
        return temp->next;
    }
}