#include <string>
#include "skip_list.hpp"
#include "bloom.hpp"
using namespace std;

class virus
{
public:
    string name;
    BloomFilter* bloom;
    Skip_List* skip_vaccinated;
    Skip_List* skip_non_vaccinated;
    virus *next;
    virus(string,unsigned long);
    ~virus();
};

class map_viruses
{
    virus *head;
public:
    map_viruses();
    ~map_viruses();
    void insert(string,unsigned long,citizen*,string,string,string);
    void new_insert(string,unsigned long,citizen*,string,string,string);
    void vaccinate_now(string,unsigned long,citizen*,string,string,string);
    void printnonVaccinated(string);
    virus* search(string);
    void print_stats(int);
};