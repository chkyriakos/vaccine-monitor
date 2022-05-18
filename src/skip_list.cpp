#include <iostream>
#include "../include/skip_list.hpp"
#include "string.h"
using namespace std;

Skip_List::Skip_List()
{
    head = new skip_node();
    head->down = NULL;
    head->next = NULL;
    head->citizen_id = new citizen(-1, "JB", "007", 0, NULL);
    head->date = "0";
    level = 1;
}

Skip_List::~Skip_List()
{
    skip_node *temp;
    skip_node *del;
    while (head != NULL) // While skip list exists
    {
        temp = head;
        delete temp->citizen_id;
        head = head->down;
        while (temp != NULL) //  Delete level
        {
            del = temp;
            temp = temp->next;
            delete del;
        }
    }
}

int Skip_List::random() //  Random function to find level of new node
{
    int i = 1;
    while (rand() % 2 == 1)
    {
        i++;
    }
    return i;
}

void Skip_List::insert(citizen *citizen_entry, string dat)
{
    int level_of_node = random(); //  Pick random level
    int new_headers = 0;

    if (level_of_node > level) // If random level higher than level of skip list add height
    {
        new_headers = level_of_node - level;
        level += new_headers;
    }

    for (int i = 0; i < new_headers; i++) // Add the new height and change head
    {
        skip_node *temp = new skip_node();
        temp->down = head;
        temp->next = NULL;
        temp->citizen_id = new citizen(-(level + i + 1), "JB", "007", 0, NULL);
        temp->date = "0";
        head = temp;
    }
    skip_node *temp = head;
    for (int i = level; i > level_of_node; i--) //  Go to the top level of the inserted node
    {
        if (temp != NULL)
        {
            temp = temp->down;
        }
    }
    skip_node *above = NULL;
    while (temp != NULL)
    {
        while (temp->next != NULL && temp->next->citizen_id->id < citizen_entry->id) //  Find the position of the inserted node
        {
            temp = temp->next;
        }
        skip_node *new_node = new skip_node(); //  Create and initialize new skip node
        new_node->citizen_id = citizen_entry;
        new_node->down = NULL;
        new_node->next = temp->next;
        new_node->date = dat;
        temp->next = new_node;
        if (above != NULL) //  Connect the node above to the current node
        {
            above->down = new_node;
        }
        above = new_node;
        temp = temp->down; //  Go to the level below
    }
}

skip_node *Skip_List::find(int citizen_id)
{
    skip_node *temp = head;

    while (temp != NULL) //  While skip list exists
    {
        if (temp->next == NULL) //  If it is the last node of the level go down
            temp = temp->down;
        else if (temp->next->citizen_id->id == citizen_id) //  If id is the same print return true
        {
            return temp->next;
        }
        else if (temp->next->citizen_id->id > citizen_id) //  If next id is greater than given go a level down
            temp = temp->down;
        else //  If next id is less than given go right
            temp = temp->next;
    }
    return NULL;
}

void Skip_List::Print()
{
    skip_node *temp = head;
    while (temp->down)
        temp = temp->down;
    temp = temp->next;
    while (temp)
    {
        cout << temp->citizen_id->id << " " << temp->citizen_id->name << " " << temp->citizen_id->lastname << " " << temp->citizen_id->citizen_country->cntr << " " << temp->citizen_id->age << endl;
        temp = temp->next;
    }
}

void Skip_List::delete_node(int citizen_id)
{
    skip_node *temp_head = head;
    skip_node *temp;
    skip_node *for_del;
    int found=0;
    while (temp_head != NULL) // While skip list exists
    {
        if(found) // All nodes of given citizenID are already deleted
            break;
        temp = temp_head;
        while (temp != NULL && temp->next != NULL && temp->next->citizen_id->id <= citizen_id)
        {
            if (temp->next->citizen_id->id == citizen_id) //  If next id is the same as given delete record
            {
                found=1;
                for_del = temp->next;
                temp->next = for_del->next; //  Connect temp to temp next next
                delete for_del;
                temp = temp->down; //  Go down a level
            }
            else
            {
                temp = temp->next; //  If next id isnt the same as given go right
            }
        }
        temp_head = temp_head->down;
    }
    temp = head;
    while (temp->next == NULL && temp->down != NULL) // Delete level if it is empty
    {
        level--;
        temp = temp->down;
    }
    head = temp;
}

int Skip_List::count(string country)
{
    int counter = 0;
    skip_node *temp = head;
    while (temp->down) // Go to the first level of the skip list
    {
        temp = temp->down;
    }
    temp = temp->next;
    while (temp) // count vaccinated or not vaccinated citizens of the country
    {
        if (temp->citizen_id->citizen_country->cntr == country)
        {
            counter++;
        }
        temp = temp->next;
    }
    return counter;
}

int Skip_List::count_dates(string country, string d1, string d2)
{
    int counter = 0;
    skip_node *temp = head;
    while (temp->down) // Go to the first level of the skip list
    {
        temp = temp->down;
    }
    temp = temp->next;
    while (temp) // count vaccinated citizens of the country
    {
        if (temp->citizen_id->citizen_country->cntr == country && dtoi(temp->date, d1, d2))
        {
            counter++;
        }
        temp = temp->next;
    }
    return counter;
}

int Skip_List::count_by_age(string country, int from, int to)
{
    int counter = 0;
    skip_node *temp = head;
    while (temp->down) // Go to the first level of the skip list
    {
        temp = temp->down;
    }
    temp = temp->next;
    while (temp) // count vaccinated or not vaccinated citizens of the country
    {
        if (temp->citizen_id->citizen_country->cntr == country && temp->citizen_id->age >= from && temp->citizen_id->age < to)
        {
            counter++;
        }
        temp = temp->next;
    }
    return counter;
}

int Skip_List::count_by_age_dates(string country, int from, int to, string d1, string d2)
{
    int counter = 0;
    skip_node *temp = head;
    while (temp->down) // Go to the first level of the skip list
    {
        temp = temp->down;
    }
    temp = temp->next;
    while (temp) // count vaccinated citizens of the country
    {
        if (temp->citizen_id->citizen_country->cntr == country && dtoi(temp->date, d1, d2) && temp->citizen_id->age >= from && temp->citizen_id->age < to)
        {
            counter++;
        }
        temp = temp->next;
    }
    return counter;
}

int Skip_List::dtoi(string date, string d1, string d2) // Date comparison
{
    int dd = atoi(strtok((char *)date.c_str(), "-"));
    int mm = atoi(strtok(NULL, "-"));
    int yyyy = atoi(strtok(NULL, "\0\n"));

    int dd1 = atoi(strtok((char *)d1.c_str(), "-"));
    int m1 = atoi(strtok(NULL, "-"));
    int yyy1 = atoi(strtok(NULL, "\0\n"));

    int dd2 = atoi(strtok((char *)d2.c_str(), "-"));
    int m2 = atoi(strtok(NULL, "-"));
    int yyy2 = atoi(strtok(NULL, "\0\n"));

    if (yyyy > yyy1 && yyyy < yyy2)
        return 1;
    else if ((mm > m1 && yyyy == yyy1) || (mm < m2 && yyyy == yyy2))
        return 1;
    else if ((dd >= dd1 && mm == m1 && yyyy == yyy1) || (dd <= dd2 && mm == m2 && yyyy == yyy2))
        return 1;
    return 0;
}