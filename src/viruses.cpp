#include <iostream>
#include <string.h>
#include "../include/viruses.hpp"
using namespace std;

virus::virus(string virus_name, unsigned long bloomsize)
{
    name = virus_name;
    next = NULL;
    bloom = new BloomFilter(bloomsize);
    skip_vaccinated = new Skip_List();
    skip_non_vaccinated = new Skip_List();
}

virus::~virus()
{
    delete bloom;
    delete skip_vaccinated;
    delete skip_non_vaccinated;
}

map_viruses::map_viruses()
{
    head = NULL;
};

map_viruses::~map_viruses()
{
    virus *del;
    while (head != NULL) // Delete list nodes one by one
    {
        del = head;
        head = head->next;
        delete del;
    }
};

void map_viruses::insert(string virus_name, unsigned long bloomsize, citizen *cit, string vaccinated, string date, string country)
{
    virus *temp = NULL;
    if (head == NULL) // Empty bucket becomes the new node
    {
        virus *new_node = new virus(virus_name, bloomsize);
        new_node->next = NULL;
        if (vaccinated == "NO" && !new_node->skip_vaccinated->find(cit->id) && !new_node->skip_non_vaccinated->find(cit->id)) // If record not vaccinated insert to the not_vaccinated skip_list
        {
            new_node->skip_non_vaccinated->insert(cit, date);
        }
        else if (vaccinated == "YES" && !new_node->skip_vaccinated->find(cit->id) && !new_node->skip_non_vaccinated->find(cit->id)) // If record vaccinated insert to the vaccinated skip_list and bloom filter
        {
            new_node->skip_vaccinated->insert(cit, date);
            new_node->bloom->add(to_string(cit->id).c_str());
        }
        else // Already in the Skip list
        {
            if (vaccinated == "NO")
                cout << "ERROR IN RECORD " << cit->id << " " << cit->name << " " << cit->lastname << " " << country << " " << cit->age << " " << virus_name << " "
                     << "NO" << endl;
            else
                cout << "ERROR IN RECORD " << cit->id << " " << cit->name << " " << cit->lastname << " " << country << " " << cit->age << " " << virus_name << " "
                     << "YES"
                     << " " << date << endl;
        }
        head = new_node;
    }
    else
    {
        virus *last = head;
        while (last != NULL) // Check if virus is already in the hashtable
        {
            if (last->name == virus_name)
            {
                if (vaccinated == "NO" && !last->skip_vaccinated->find(cit->id) && !last->skip_non_vaccinated->find(cit->id)) // If record not vaccinated insert to the not_vaccinated skip_list
                {
                    last->skip_non_vaccinated->insert(cit, date);
                }
                else if (vaccinated == "YES" && !last->skip_non_vaccinated->find(cit->id) && !last->skip_vaccinated->find(cit->id)) // If record vaccinated insert to the vaccinated skip_list and bloom filter
                {
                    last->skip_vaccinated->insert(cit, date);
                    last->bloom->add(to_string(cit->id).c_str());
                }
                else
                {
                    if (vaccinated == "NO")
                        cout << "ERROR IN RECORD " << cit->id << " " << cit->name << " " << cit->lastname << " " << country << " " << cit->age << " " << virus_name << " "
                             << "NO" << endl;
                    else
                        cout << "ERROR IN RECORD " << cit->id << " " << cit->name << " " << cit->lastname << " " << country << " " << cit->age << " " << virus_name << " "
                             << "YES"
                             << " " << date << endl;
                }
                return;
            }
            temp = last;
            last = last->next;
        }
        virus *new_node = new virus(virus_name, bloomsize); // Virus not in the hashtable
        new_node->next = NULL;
        if (vaccinated == "NO" && !new_node->skip_vaccinated->find(cit->id) && !new_node->skip_non_vaccinated->find(cit->id)) // If record not vaccinated insert to the not_vaccinated skip_list
        {
            new_node->skip_non_vaccinated->insert(cit, date);
        }
        else if (vaccinated == "YES" && !new_node->skip_vaccinated->find(cit->id) && !new_node->skip_non_vaccinated->find(cit->id)) // If record vaccinated insert to the vaccinated skip_list and bloom filter
        {
            new_node->skip_vaccinated->insert(cit, date);
            new_node->bloom->add(to_string(cit->id).c_str());
        }
        else // Already in the Skip list
        {
            if (vaccinated == "NO")
                cout << "ERROR IN RECORD " << cit->id << " " << cit->name << " " << cit->lastname << " " << country << " " << cit->age << " " << virus_name << " "
                     << "NO" << endl;
            else
                cout << "ERROR IN RECORD " << cit->id << " " << cit->name << " " << cit->lastname << " " << country << " " << cit->age << " " << virus_name << " "
                     << "YES"
                     << " " << date << endl;
        }
        temp->next = new_node;
    }
}

void map_viruses::new_insert(string virus_name, unsigned long bloomsize, citizen *cit, string vaccinated, string date, string country)
{
    virus *temp = NULL;
    if (head == NULL) // Empty bucket becomes the new node
    {
        virus *new_node = new virus(virus_name, bloomsize);
        new_node->next = NULL;
        skip_node *temp_new = new_node->skip_vaccinated->find(cit->id);
        if (vaccinated == "NO" && !temp_new && !new_node->skip_non_vaccinated->find(cit->id)) // If record not vaccinated insert to the not_vaccinated skip_list
        {
            new_node->skip_non_vaccinated->insert(cit, date);
        }
        else if (vaccinated == "YES" && !temp_new && !new_node->skip_non_vaccinated->find(cit->id)) // If record vaccinated insert to the vaccinated skip_list and bloom filter
        {
            new_node->skip_vaccinated->insert(cit, date);
            new_node->bloom->add(to_string(cit->id).c_str());
        }
        else if (vaccinated == "YES" && !temp_new && new_node->skip_non_vaccinated->find(cit->id)) // If record already exists in the non_vaccinated skip_list delete it and add it vaccinated skip_list and bloom filter
        {
            new_node->skip_non_vaccinated->delete_node(cit->id);
            new_node->skip_vaccinated->insert(cit, date);
            new_node->bloom->add(to_string(cit->id).c_str());
        }
        else // Already in the Skip list or Already Vaccinated
        {
            if (vaccinated == "NO")
                cout << "ERROR IN RECORD " << cit->id << " " << cit->name << " " << cit->lastname << " " << country << " " << cit->age << " " << virus_name << " "
                     << "NO" << endl;
            else
                cout << "ERROR: CITIZEN " << cit->id << " ALREADY VACCINATED ON " << temp_new->date << endl;
        }
        head = new_node;
    }
    else
    {
        virus *last = head;
        while (last != NULL) // Check if virus is already in the hashtable
        {
            if (last->name == virus_name)
            {
                skip_node *temp_new = last->skip_vaccinated->find(cit->id);
                if (vaccinated == "NO" && !temp_new && !last->skip_non_vaccinated->find(cit->id)) // If record not vaccinated insert to the not_vaccinated skip_list
                {
                    last->skip_non_vaccinated->insert(cit, date);
                }
                else if (vaccinated == "YES" && !temp_new && !last->skip_non_vaccinated->find(cit->id)) // If record vaccinated insert to the vaccinated skip_list and bloom filter
                {
                    last->skip_vaccinated->insert(cit, date);
                    last->bloom->add(to_string(cit->id).c_str());
                }
                else if (vaccinated == "YES" && !temp_new && last->skip_non_vaccinated->find(cit->id)) // If record already exists in the non_vaccinated skip_list delete it and add it vaccinated skip_list and bloom filter
                {
                    last->skip_non_vaccinated->delete_node(cit->id);
                    last->skip_vaccinated->insert(cit, date);
                    last->bloom->add(to_string(cit->id).c_str());
                }
                else // Already in the Skip list or Already Vaccinated
                {
                    if (vaccinated == "NO")
                        cout << "ERROR IN RECORD " << cit->id << " " << cit->name << " " << cit->lastname << " " << country << " " << cit->age << " " << virus_name << " "
                             << "NO" << endl;
                    else
                        cout << "ERROR: CITIZEN " << cit->id << " ALREADY VACCINATED ON " << temp_new->date << endl;
                }
                return;
            }
            temp = last;
            last = last->next;
        }
        virus *new_node = new virus(virus_name, bloomsize); // Virus not in the hashtable
        new_node->next = NULL;
        skip_node *temp_new = new_node->skip_vaccinated->find(cit->id);
        if (vaccinated == "NO" && !temp_new && !new_node->skip_non_vaccinated->find(cit->id)) // If record not vaccinated insert to the not_vaccinated skip_list
        {
            new_node->skip_non_vaccinated->insert(cit, date);
        }
        else if (vaccinated == "YES" && !temp_new && !new_node->skip_non_vaccinated->find(cit->id)) // If record vaccinated insert to the vaccinated skip_list and bloom filter
        {
            new_node->skip_vaccinated->insert(cit, date);
            new_node->bloom->add(to_string(cit->id).c_str());
        }
        else if (vaccinated == "YES" && !temp_new && new_node->skip_non_vaccinated->find(cit->id)) // If record already exists in the non_vaccinated skip_list delete it and add it vaccinated skip_list and bloom filter
        {
            new_node->skip_non_vaccinated->delete_node(cit->id);
            new_node->skip_vaccinated->insert(cit, date);
            new_node->bloom->add(to_string(cit->id).c_str());
        }
        else // Already in the Skip list or Already Vaccinated
        {
            if (vaccinated == "NO")
                cout << "ERROR IN RECORD " << cit->id << " " << cit->name << " " << cit->lastname << " " << country << " " << cit->age << " " << virus_name << " "
                     << "NO" << endl;
            else
                cout << "ERROR: CITIZEN " << cit->id << " ALREADY VACCINATED ON " << temp_new->date << endl;
        }
        temp->next = new_node;
    }
}

void map_viruses::vaccinate_now(string virus_name, unsigned long bloomsize, citizen *cit, string vaccinated, string date, string country)
{
    virus *temp = NULL;
    if (head == NULL) // Empty bucket becomes the new node
    {
        virus *new_node = new virus(virus_name, bloomsize);
        new_node->next = NULL;
        skip_node *temp_new = new_node->skip_vaccinated->find(cit->id);
        if (!temp_new && !new_node->skip_non_vaccinated->find(cit->id)) // If record doesnt exist add it vaccinated skip_list and bloom filter
        {
            new_node->skip_vaccinated->insert(cit, date);
            new_node->bloom->add(to_string(cit->id).c_str());
        }
        else if (!temp_new && new_node->skip_non_vaccinated->find(cit->id)) // If record already exists in the non_vaccinated skip_list delete it and add it vaccinated skip_list and bloom filter
        {
            new_node->skip_non_vaccinated->delete_node(cit->id);
            new_node->skip_vaccinated->insert(cit, date);
            new_node->bloom->add(to_string(cit->id).c_str());
        }
        else // Already Vaccinated
        {
            cout << "ERROR CITIZEN " << cit->id << " ALREADY VACCINATED ON " << temp_new->date << endl;
        }
        head = new_node;
    }
    else
    {
        virus *last = head;
        while (last != NULL) // Check if virus is already in the hashtable
        {
            if (last->name == virus_name)
            {
                skip_node *temp_new = last->skip_vaccinated->find(cit->id);
                if (!temp_new && !last->skip_non_vaccinated->find(cit->id)) // If record doesnt exist add it vaccinated skip_list and bloom filter
                {
                    last->skip_vaccinated->insert(cit, date);
                    last->bloom->add(to_string(cit->id).c_str());
                }
                else if (!temp_new && last->skip_non_vaccinated->find(cit->id)) // If record already exists in the non_vaccinated skip_list delete it and add it vaccinated skip_list and bloom filter
                {
                    last->skip_non_vaccinated->delete_node(cit->id);
                    last->skip_vaccinated->insert(cit, date);
                    last->bloom->add(to_string(cit->id).c_str());
                }
                else // Already Vaccinated
                {
                    cout << "ERROR CITIZEN " << cit->id << " ALREADY VACCINATED ON " << temp_new->date << endl;
                }
                return;
            }
            temp = last;
            last = last->next;
        }
        virus *new_node = new virus(virus_name, bloomsize); // Virus not in the hashtable
        new_node->next = NULL;
        skip_node *temp_new = new_node->skip_vaccinated->find(cit->id);
        if (!temp_new && !new_node->skip_non_vaccinated->find(cit->id)) // If record doesnt exist add it vaccinated skip_list and bloom filter
        {
            new_node->skip_vaccinated->insert(cit, date);
            new_node->bloom->add(to_string(cit->id).c_str());
        }
        else if (!temp_new && new_node->skip_non_vaccinated->find(cit->id)) // If record already exists in the non_vaccinated skip_list delete it and add it vaccinated skip_list and bloom filter
        {
            new_node->skip_non_vaccinated->delete_node(cit->id);
            new_node->skip_vaccinated->insert(cit, date);
            new_node->bloom->add(to_string(cit->id).c_str());
        }
        else // Already Vaccinated
        {
            cout << "ERROR CITIZEN " << cit->id << " ALREADY VACCINATED ON " << temp_new->date << endl;
        }
        temp->next = new_node;
    }
}

void map_viruses::printnonVaccinated(string country) // Print list of non vaccinated records of given virus
{
    virus *node = head;
    while (node != NULL)
    {
        if (node->name == country)
        {
            node->skip_non_vaccinated->Print();
        }
        node = node->next;
    }
};

void map_viruses::print_stats(int id) // Print vaccine status for each virus
{
    virus *node = head;
    while (node != NULL)
    {
        skip_node *sn = node->skip_vaccinated->find(id);
        if (sn)
        {
            cout << node->name;
            cout << " YES " << sn->date << endl;
        }
        sn = node->skip_non_vaccinated->find(id);
        if (sn)
        {
            cout << node->name;
            cout << " NO" << endl;
        }
        node = node->next;
    }
};

virus *map_viruses::search(string value) // Search map to find if virus exists
{
    virus *node = head;
    while (node != NULL)
    {
        if (node->name == value)
        {
            return node;
        }
        node = node->next;
    }
    return NULL;
};
