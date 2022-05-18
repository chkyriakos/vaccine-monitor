#include <string>
using namespace std;

class country
{
public:
    string cntr;
    int population;
    country *next;
    country(string);
    ~country();
};

class map_countries
{
public:
    country *head;
    map_countries();
    ~map_countries();
    country *insert_return(string);
    void print();
};