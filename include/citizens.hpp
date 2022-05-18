#include <string>
#include "countries.hpp"
using namespace std;

class citizen
{
public:
    int id;
    string name;
    string lastname;
    int age;
    citizen *next;
    country *citizen_country;
    citizen(int, string, string, int, country *);
};

class map_citizens
{
    citizen *head;

public:
    map_citizens();
    ~map_citizens();
    citizen *insert(int, string, string, country *, int);
};