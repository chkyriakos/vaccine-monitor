#include "citizens.hpp"

class skip_node
{
public:
    citizen *citizen_id;
    string date;
    skip_node *next;
    skip_node *down;
};

class Skip_List
{
    skip_node *head;
    int level;

public:
    Skip_List();
    ~Skip_List();
    void insert(citizen *, string date);
    int random();
    void Print();
    skip_node *find(int);
    void delete_node(int);
    int count(string);
    int count_dates(string, string, string);
    int count_by_age(string,int,int);
    int count_by_age_dates(string,int,int, string, string);
    int dtoi(string, string, string);
};
