#include <iostream>
#include "../include/countries.hpp"
using namespace std;

country::country(string cntr_name)
{
    cntr = cntr_name;
    population = 0;
    next = NULL;
}

country::~country()
{
}

map_countries::map_countries()
{
    head = NULL;
};

map_countries::~map_countries()
{
    country *del;
    while (head != NULL) // Delete list nodes one by one
    {
        del = head;
        head = head->next;
        delete del;
    }
};

country *map_countries::insert_return(string cntr_name)
{
    country *temp = NULL;
    if (head == NULL) // Empty bucket becomes the new node
    {
        country *new_node = new country(cntr_name);
        new_node->population++;
        new_node->next = NULL;
        head = new_node;
        return head;
    }
    else // Search for Country and if not found add to bucket
    {
        country *last = head;
        while (last != NULL)
        {
            if (last->cntr == cntr_name)
            {
                last->population++;
                return last;
            }
            temp = last;
            last = last->next;
        }
        country *new_node = new country(cntr_name); // Insert new node
        new_node->population++;
        new_node->next = NULL;
        temp->next = new_node;
        return temp->next;
    }
}