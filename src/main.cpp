#include <iostream>
#include <string>
#include <string.h>
#include "../include/viruses.hpp"
#include "time.h"

using namespace std;

bool isnum(char *var)
{
    int length = strlen(var);
    for (int i = 0; i < length; i++)
    {
        if (!isdigit(*var))
            return false;
        *var++;
    }
    return true;
}

int myhash(int id, int buckets) // Simple hash function
{
    return id % buckets;
}

unsigned long sdbm(char *str, int buckets) // Given hash function
{
    unsigned long hash = 0;
    int c;

    while (c = *str++)
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash % buckets;
}

bool valid_date(string d) // Check if date format is correct
{
    char *dd = strtok((char *)d.c_str(), "-\n\0 ");
    char *mm = strtok(NULL, "-\n\0 ");
    char *yyyy = strtok(NULL, "\0\n ");
    if (!isnum(dd) || !isnum(mm) || !isnum(yyyy))
    {
        return false;
    }
    if (dd == NULL || strlen(dd) != 2)
    {
        return false;
    }
    else if (mm == NULL || strlen(mm) != 2)
    {
        return false;
    }
    else if (yyyy == NULL || strlen(yyyy) != 4)
    {
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    unsigned long bloomsize;
    FILE *citizenRecordsFile = NULL;
    srand(time(0));

    for (int i = 1; i < argc; i++) //reading parameteres from command line
    {
        if (strcmp("-c", argv[i]) == 0)
            citizenRecordsFile = fopen(argv[i + 1], "r");
        else if (strcmp("-b", argv[i]) == 0)
        {
            if (isnum(argv[i + 1])) // checking if bloomsize is a number
                bloomsize = atoi(argv[i + 1]);
            else
            {
                printf("Error, bloomsize is not a number\n");
                return 0;
            }
        }
    }
    if (citizenRecordsFile == NULL)  // checking if there is an input file
    {
        printf("Error, no input file found\n");
        return 0;
    }

    char *citizenID, *firstName, *lastName, *cntr, *age, *virusName, *vaccinated, *dateVaccinated;
    const size_t length = 120;
    char line[length];

    int country_buckets = 10;
    int virus_buckets = 10;
    int record_buckets = 100;
    map_citizens hash_citizens[record_buckets];
    map_countries hash_countries[country_buckets];
    map_viruses hash_viruses[virus_buckets];

    while (fgets(line, length, citizenRecordsFile) != NULL) // Reading input file line by line
    {
        citizenID = (strtok(line, " "));
        firstName = strtok(NULL, " ");
        lastName = strtok(NULL, " ");
        cntr = strtok(NULL, " ");
        age = strtok(NULL, " ");
        virusName = strtok(NULL, " ");
        vaccinated = strtok(NULL, " \n");
        dateVaccinated = strtok(NULL, "\n");

        if (!isnum(citizenID) || !isnum(age)) // Checking if citizenID and age are numbers
        {
            if (dateVaccinated)
            {
                cout << "ERROR IN RECORD " << citizenID << " " << firstName << " " << lastName << " " << cntr << " " << age << " " << virusName << " " << vaccinated << " " << dateVaccinated << endl;
            }
            else
            {
                cout << "ERROR IN RECORD " << citizenID << " " << firstName << " " << lastName << " " << cntr << " " << age << " " << virusName << " " << vaccinated << endl;
            }
        }
        else if (dateVaccinated) // Adding vaccinated record
        {
            if (strcmp(vaccinated, "YES") == 0 && valid_date(dateVaccinated))
            {
                citizen *cit_pointer = hash_citizens[myhash(atoi(citizenID), record_buckets)].insert(atoi(citizenID), firstName, lastName, hash_countries[sdbm(cntr, country_buckets)].insert_return(cntr), atoi(age));
                if (cit_pointer) // Record is valid
                {
                    hash_viruses[sdbm(virusName, virus_buckets)].insert(virusName, bloomsize, cit_pointer, vaccinated, dateVaccinated, cntr);
                }
                else // Record is invalid
                {
                    cout << "ERROR IN RECORD " << citizenID << " " << firstName << " " << lastName << " " << cntr << " " << age << " " << virusName << " " << vaccinated << " " << dateVaccinated << endl;
                }
            }
            else
            {
                cout << "ERROR IN RECORD " << citizenID << " " << firstName << " " << lastName << " " << cntr << " " << age << " " << virusName << " " << vaccinated << " " << dateVaccinated << endl;
            }
        }
        else if ((strcmp(vaccinated, "NO") == 0)) // Adding non vaccinated record
        {
            citizen *cit_pointer = hash_citizens[myhash(atoi(citizenID), record_buckets)].insert(atoi(citizenID), firstName, lastName, hash_countries[sdbm(cntr, country_buckets)].insert_return(cntr), atoi(age));
            if (cit_pointer) // Record is valid
            {
                hash_viruses[sdbm(virusName, virus_buckets)].insert(virusName, bloomsize, cit_pointer, vaccinated, "0", cntr);
            }
            else // Record is invalid
            {
                cout << "ERROR IN RECORD " << citizenID << " " << firstName << " " << lastName << " " << cntr << " " << age << " " << virusName << " " << vaccinated << endl;
            }
        }
        else // Record is invalid
        {
            cout << "ERROR IN RECORD " << citizenID << " " << firstName << " " << lastName << " " << cntr << " " << age << " " << virusName << " " << vaccinated << endl;
        }
    }
    fclose(citizenRecordsFile); // finished inerting from input file

    char command[120];
    char *c1;
    char *c2;
    char *c3;
    char *c4;
    char *c5;
    char *c6;
    char *c7;
    char *c8;
    char *c9;
    char *c10;

    cout << "TYPE YOUR QUERY" << endl;
    fgets(command, 120, stdin);
    while (strcmp(command, "/exit\n") != 0)
    {
        c1 = strtok(command, "\n ");
        if (strcmp(c1, "/vaccineStatusBloom") == 0)
        {
            c2 = strtok(NULL, "\n ");
            c3 = strtok(NULL, "\n ");
            c4 = strtok(NULL, "\n ");

            if (!isnum(c2)) // check if citizenID is a number
            {
                cout << endl;
                cout << "Correct Usage:" << endl;
                cout << "/vaccineStatusBloom citizenID virusName" << endl;
                cout << endl;
            }
            else if (c3 && !c4) // If the query is correct
            {
                virus *node = hash_viruses[sdbm(c3, virus_buckets)].search(c3); // find given virus
                if (node && node->bloom->search(c2))                            // check if id is in the bloom filter
                {
                    cout << "MAYBE" << endl;
                }
                else
                {
                    cout << "NOT VACCINATED" << endl;
                }
            }
            else // If a parameter is missing or there are more parameters
            {
                cout << endl;
                cout << "Correct Usage:" << endl;
                cout << "/vaccineStatusBloom citizenID virusName" << endl;
                cout << endl;
            }
        }
        else if (strcmp(c1, "/vaccineStatus") == 0)
        {
            c2 = strtok(NULL, "\n ");
            c3 = strtok(NULL, "\n ");
            c4 = strtok(NULL, "\n ");

            if (!isnum(c2)) // check if citizenID is a number
            {
                cout << endl;
                cout << "Correct Usage:" << endl;
                cout << "/vaccineStatus citizenID [virusName]" << endl;
                cout << endl;
            }
            else if (c3 && !c4) // if all the parameters are used
            {
                virus *node = hash_viruses[sdbm(c3, virus_buckets)].search(c3); // find given virus
                if (node && node->bloom->search(c2))                            // check if id is in the bloom filter
                {
                    skip_node *sn = node->skip_vaccinated->find(atoi(c2)); // check if id is in the vaccinated skip list
                    if (sn)
                    {
                        cout << "VACCINATED ON " << sn->date << endl;
                    }
                    else
                    {
                        cout << "NOT VACCINATED" << endl;
                    }
                }
                else
                {
                    cout << "NOT VACCINATED" << endl;
                }
            }
            else if (!c3) // Print vaccine status for all viruses
            {
                for (int i = 0; i < virus_buckets; i++)
                {
                    hash_viruses[i].print_stats(atoi(c2));
                }
            }
            else // If a parameter is missing or there are more parameters
            {
                cout << endl;
                cout << "Correct Usage:" << endl;
                cout << "/vaccineStatus citizenID [virusName]" << endl;
                cout << endl;
            }
        }
        else if (strcmp(c1, "/populationStatus") == 0)
        {
            c2 = strtok(NULL, "\n ");
            c3 = strtok(NULL, "\n ");
            c4 = strtok(NULL, "\n ");
            c5 = strtok(NULL, "\n ");
            c6 = strtok(NULL, "\n ");
            if (!c2 || c6) // If there are more or less parameters
            {
                cout << endl;
                cout << "Correct Usage:" << endl;
                cout << "/populationStatus [country] virusName [date1 date2]" << endl;
                cout << endl;
            }
            else if (!c3 && !c4 && !c5 && !valid_date(c2)) // Only virus given
            {
                virus *vir_cntr = hash_viruses[sdbm(c2, virus_buckets)].search(c2); // Find given virus
                if (vir_cntr)                                                       // If virus exists
                {
                    for (int i = 0; i < country_buckets; i++) // Traverse all the buckets
                    {
                        country *country_temp = hash_countries[i].head;
                        while (country_temp) // If bucket is not empty
                        {
                            int vaccinated_number = 0;
                            int total = 0;
                            vaccinated_number = vir_cntr->skip_vaccinated->count(country_temp->cntr);             // Count vaccinated records of country
                            total = vaccinated_number + vir_cntr->skip_non_vaccinated->count(country_temp->cntr); // Count all records of country
                            float percentage = 0;
                            if (total != 0)
                            {
                                percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage
                            }
                            cout << country_temp->cntr << " " << vaccinated_number << " ";
                            printf("%.2f", percentage);
                            cout << "%" << endl;
                            country_temp = country_temp->next;
                        }
                    }
                }
                else
                {
                    cout << "Virus " << c2 << " does not exist" << endl;
                }
            }
            else if (!c5 && !c4 && !valid_date(c2) && !valid_date(c3)) // Only country and virus given
            {
                virus *vir_cntr = hash_viruses[sdbm(c3, virus_buckets)].search(c3); // Find given virus
                if (vir_cntr)                                                       // If virus exists
                {
                    int vaccinated_number = 0;
                    int total = 0;
                    vaccinated_number = vir_cntr->skip_vaccinated->count(c2);             // Count vaccinated records of country
                    total = vaccinated_number + vir_cntr->skip_non_vaccinated->count(c2); // Count all records of country
                    float percentage = 0;
                    if (total != 0)
                    {
                        percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage
                    }
                    cout << c2 << " " << vaccinated_number << " ";
                    printf("%.2f", percentage);
                    cout << "%" << endl;
                }
                else
                {
                    cout << "Virus " << c3 << " does not exist" << endl;
                }
            }
            else if (!c5) //Only virus and dates given
            {
                if (valid_date(c3) && valid_date(c4) && !valid_date(c2)) // if all the parameters are valid
                {
                    virus *vir_cntr = hash_viruses[sdbm(c2, virus_buckets)].search(c2); // Find given virus
                    if (vir_cntr)                                                       // If virus exists
                    {
                        for (int i = 0; i < country_buckets; i++) // Traverse all the buckets
                        {
                            country *country_temp = hash_countries[i].head;
                            while (country_temp) // If bucket is not empty
                            {
                                int vaccinated_number = 0;
                                int total = 0;
                                vaccinated_number = vir_cntr->skip_vaccinated->count_dates(country_temp->cntr, c3, c4);                                  // Count vaccinated records of country in given dates
                                total = vir_cntr->skip_vaccinated->count(country_temp->cntr) + vir_cntr->skip_non_vaccinated->count(country_temp->cntr); // Count all records of country
                                float percentage = 0;
                                if (total != 0)
                                {
                                    percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage
                                }
                                cout << country_temp->cntr << " " << vaccinated_number << " ";
                                printf("%.2f", percentage);
                                cout << "%" << endl;
                                country_temp = country_temp->next;
                            }
                        }
                    }
                    else
                    {
                        cout << "Virus " << c2 << " does not exist" << endl;
                    }
                }
                else // Not valid parameters
                {
                    cout << endl;
                    cout << "Correct Usage:" << endl;
                    cout << "/populationStatus [country] virusName [date1 date2]" << endl;
                    cout << endl;
                }
            }
            else // All parameters given
            {
                if (valid_date(c4) && valid_date(c5)) // If dates are valid
                {
                    virus *vir_cntr = hash_viruses[sdbm(c3, virus_buckets)].search(c3); // Find given virus
                    if (vir_cntr)                                                       // If virus exists
                    {
                        int vaccinated_number = 0;
                        int total = 0;
                        vaccinated_number = vir_cntr->skip_vaccinated->count_dates(c2, c4, c5);                  // Count vaccinated records of country in given dates
                        total = vir_cntr->skip_vaccinated->count(c2) + vir_cntr->skip_non_vaccinated->count(c2); // Count all records of country
                        float percentage = 0;
                        if (total != 0)
                        {
                            percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage
                        }
                        cout << c2 << " " << vaccinated_number << " ";
                        printf("%.2f", percentage);
                        cout << "%" << endl;
                    }
                    else
                    {
                        cout << "Virus " << c3 << " does not exist" << endl;
                    }
                }
                else // Not valid parameters
                {
                    cout << endl;
                    cout << "Correct Usage:" << endl;
                    cout << "/populationStatus [country] virusName [date1 date2]" << endl;
                    cout << endl;
                }
            }
        }
        else if (strcmp(c1, "/popStatusByAge") == 0)
        {
            c2 = strtok(NULL, "\n ");
            c3 = strtok(NULL, "\n ");
            c4 = strtok(NULL, "\n ");
            c5 = strtok(NULL, "\n ");
            c6 = strtok(NULL, "\n ");
            if (!c2 || c6) // If there are more or less parameters
            {
                cout << endl;
                cout << "Correct Usage:" << endl;
                cout << "/popStatusByAge [country] virusName [date1 date2]" << endl;
                cout << endl;
            }
            else if (!c3 && !c4 && !c5 && !valid_date(c2)) // Only virus given
            {
                virus *vir_cntr = hash_viruses[sdbm(c2, virus_buckets)].search(c2); // Find given virus
                if (vir_cntr)                                                       // If virus exists
                {
                    for (int i = 0; i < country_buckets; i++) // Traverse all the buckets
                    {
                        country *country_temp = hash_countries[i].head;
                        while (country_temp) // If bucket is not empty
                        {
                            cout << country_temp->cntr << endl;
                            cout << "---------" << endl;

                            //0-20
                            int vaccinated_number = 0;
                            int total = 0;
                            vaccinated_number = vir_cntr->skip_vaccinated->count_by_age(country_temp->cntr, 0, 20);             // Count vaccinated records of country for given age
                            total = vaccinated_number + vir_cntr->skip_non_vaccinated->count_by_age(country_temp->cntr, 0, 20); // Count all records of country for given age
                            float percentage = 0;
                            if (total != 0)
                            {
                                percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                            }
                            cout << "0-20 " << vaccinated_number << " ";
                            printf("%.2f", percentage);
                            cout << "%" << endl;

                            //20-40
                            vaccinated_number = 0;
                            total = 0;
                            vaccinated_number = vir_cntr->skip_vaccinated->count_by_age(country_temp->cntr, 20, 40);             // Count vaccinated records of country for given age
                            total = vaccinated_number + vir_cntr->skip_non_vaccinated->count_by_age(country_temp->cntr, 20, 40); // Count all records of country for given age
                            percentage = 0;
                            if (total != 0)
                            {
                                percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                            }
                            cout << "20-40 " << vaccinated_number << " ";
                            printf("%.2f", percentage);
                            cout << "%" << endl;

                            //40-60
                            vaccinated_number = 0;
                            total = 0;
                            vaccinated_number = vir_cntr->skip_vaccinated->count_by_age(country_temp->cntr, 40, 60);             // Count vaccinated records of country for given age
                            total = vaccinated_number + vir_cntr->skip_non_vaccinated->count_by_age(country_temp->cntr, 40, 60); // Count all records of country for given age
                            percentage = 0;
                            if (total != 0)
                            {
                                percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                            }
                            cout << "40-60 " << vaccinated_number << " ";
                            printf("%.2f", percentage);
                            cout << "%" << endl;

                            //60+
                            vaccinated_number = 0;
                            total = 0;
                            vaccinated_number = vir_cntr->skip_vaccinated->count_by_age(country_temp->cntr, 60, 121);             // Count vaccinated records of country for given age
                            total = vaccinated_number + vir_cntr->skip_non_vaccinated->count_by_age(country_temp->cntr, 60, 121); // Count all records of country for given age
                            percentage = 0;
                            if (total != 0)
                            {
                                percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                            }
                            cout << "60+ " << vaccinated_number << " ";
                            printf("%.2f", percentage);
                            cout << "%" << endl;
                            cout << endl;
                            country_temp = country_temp->next;
                        }
                    }
                }
                else
                {
                    cout << "Virus " << c2 << " does not exist" << endl;
                }
            }
            else if (!c5 && !c4 && !valid_date(c2) && !valid_date(c3)) // Only country and virus given
            {
                virus *vir_cntr = hash_viruses[sdbm(c3, virus_buckets)].search(c3); // Find given virus
                if (vir_cntr)                                                       // If virus exists
                {
                    cout << c2 << endl;
                    cout << "---------" << endl;
                    //0-20
                    int vaccinated_number = 0;
                    int total = 0;
                    vaccinated_number = vir_cntr->skip_vaccinated->count_by_age(c2, 0, 20);             // Count vaccinated records of country for given age
                    total = vaccinated_number + vir_cntr->skip_non_vaccinated->count_by_age(c2, 0, 20); // Count all records of country for given age
                    float percentage = 0;
                    if (total != 0)
                    {
                        percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                    }
                    cout << "0-20 " << vaccinated_number << " ";
                    printf("%.2f", percentage);
                    cout << "%" << endl;

                    //20-40
                    vaccinated_number = 0;
                    total = 0;
                    vaccinated_number = vir_cntr->skip_vaccinated->count_by_age(c2, 20, 40);             // Count vaccinated records of country for given age
                    total = vaccinated_number + vir_cntr->skip_non_vaccinated->count_by_age(c2, 20, 40); // Count all records of country for given age
                    percentage = 0;
                    if (total != 0)
                    {
                        percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                    }
                    cout << "20-40 " << vaccinated_number << " ";
                    printf("%.2f", percentage);
                    cout << "%" << endl;

                    //40-60
                    vaccinated_number = 0;
                    total = 0;
                    vaccinated_number = vir_cntr->skip_vaccinated->count_by_age(c2, 40, 60);             // Count vaccinated records of country for given age
                    total = vaccinated_number + vir_cntr->skip_non_vaccinated->count_by_age(c2, 40, 60); // Count all records of country for given age
                    percentage = 0;
                    if (total != 0)
                    {
                        percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                    }
                    cout << "40-60 " << vaccinated_number << " ";
                    printf("%.2f", percentage);
                    cout << "%" << endl;

                    //60+
                    vaccinated_number = 0;
                    total = 0;
                    vaccinated_number = vir_cntr->skip_vaccinated->count_by_age(c2, 60, 121);             // Count vaccinated records of country for given age
                    total = vaccinated_number + vir_cntr->skip_non_vaccinated->count_by_age(c2, 60, 121); // Count all records of country for given age
                    percentage = 0;
                    if (total != 0)
                    {
                        percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                    }
                    cout << "60+ " << vaccinated_number << " ";
                    printf("%.2f", percentage);
                    cout << "%" << endl;
                    cout << endl;
                }
                else
                {
                    cout << "Virus " << c3 << " does not exist" << endl;
                }
            }
            else if (!c5)
            {
                if (valid_date(c3) && valid_date(c4) && !valid_date(c2)) // Only virus and dates given
                {
                    virus *vir_cntr = hash_viruses[sdbm(c2, virus_buckets)].search(c2); // Find given virus
                    if (vir_cntr)                                                       // If virus exists
                    {
                        for (int i = 0; i < country_buckets; i++) // Traverse all the buckets
                        {
                            country *country_temp = hash_countries[i].head;
                            while (country_temp) // If bucket is not empty
                            {
                                cout << country_temp->cntr << endl;
                                cout << "---------" << endl;

                                //0-20
                                int vaccinated_number = 0;
                                int total = 0;
                                vaccinated_number = vir_cntr->skip_vaccinated->count_by_age_dates(country_temp->cntr, 0, 20, c3, c4);                                                // Count vaccinated records of country for given age and dates
                                total = vir_cntr->skip_vaccinated->count_by_age(country_temp->cntr, 0, 20) + vir_cntr->skip_non_vaccinated->count_by_age(country_temp->cntr, 0, 20); // Count all records of country for given age
                                float percentage = 0;
                                if (total != 0)
                                {
                                    percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                                }
                                cout << "0-20 " << vaccinated_number << " ";
                                printf("%.2f", percentage);
                                cout << "%" << endl;

                                //20-40
                                vaccinated_number = 0;
                                total = 0;
                                vaccinated_number = vir_cntr->skip_vaccinated->count_by_age_dates(country_temp->cntr, 20, 40, c3, c4);                                                 // Count vaccinated records of country for given age and dates
                                total = vir_cntr->skip_vaccinated->count_by_age(country_temp->cntr, 20, 40) + vir_cntr->skip_non_vaccinated->count_by_age(country_temp->cntr, 20, 40); // Count all records of country for given age
                                percentage = 0;
                                if (total != 0)
                                {
                                    percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                                }
                                cout << "20-40 " << vaccinated_number << " ";
                                printf("%.2f", percentage);
                                cout << "%" << endl;

                                //40-60
                                vaccinated_number = 0;
                                total = 0;
                                vaccinated_number = vir_cntr->skip_vaccinated->count_by_age_dates(country_temp->cntr, 40, 60, c3, c4);                                                 // Count vaccinated records of country for given age and dates
                                total = vir_cntr->skip_vaccinated->count_by_age(country_temp->cntr, 40, 60) + vir_cntr->skip_non_vaccinated->count_by_age(country_temp->cntr, 40, 60); // Count all records of country for given age
                                percentage = 0;
                                if (total != 0)
                                {
                                    percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                                }
                                cout << "40-60 " << vaccinated_number << " ";
                                printf("%.2f", percentage);
                                cout << "%" << endl;

                                //60+
                                vaccinated_number = 0;
                                total = 0;
                                vaccinated_number = vir_cntr->skip_vaccinated->count_by_age_dates(country_temp->cntr, 60, 121, c3, c4);                                                  // Count vaccinated records of country for given age and dates
                                total = vir_cntr->skip_vaccinated->count_by_age(country_temp->cntr, 60, 121) + vir_cntr->skip_non_vaccinated->count_by_age(country_temp->cntr, 60, 121); // Count all records of country for given age
                                percentage = 0;
                                if (total != 0)
                                {
                                    percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                                }
                                cout << "60+ " << vaccinated_number << " ";
                                printf("%.2f", percentage);
                                cout << "%" << endl;
                                cout << endl;
                                country_temp = country_temp->next;
                            }
                        }
                    }
                    else
                    {
                        cout << "Virus " << c2 << " does not exist" << endl;
                    }
                }
                else // Not valid parameters
                {
                    cout << endl;
                    cout << "Correct Usage:" << endl;
                    cout << "/popStatusByAge [country] virusName [date1 date2]" << endl;
                    cout << endl;
                }
            }
            else // All parameters given
            {
                if (valid_date(c4) && valid_date(c5)) // If dates are valid
                {
                    virus *vir_cntr = hash_viruses[sdbm(c3, virus_buckets)].search(c3); // Find given virus
                    if (vir_cntr)                                                       // If virus exists
                    {
                        cout << c2 << endl;
                        cout << "---------" << endl;

                        //0-20
                        int vaccinated_number = 0;
                        int total = 0;
                        vaccinated_number = vir_cntr->skip_vaccinated->count_by_age_dates(c2, 0, 20, c4, c5);                                // Count vaccinated records of country for given age and dates
                        total = vir_cntr->skip_vaccinated->count_by_age(c2, 0, 20) + vir_cntr->skip_non_vaccinated->count_by_age(c2, 0, 20); // Count all records of country for given age
                        float percentage = 0;
                        if (total != 0)
                        {
                            percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                        }
                        cout << "0-20 " << vaccinated_number << " ";
                        printf("%.2f", percentage);
                        cout << "%" << endl;

                        //20-40
                        vaccinated_number = 0;
                        total = 0;
                        vaccinated_number = vir_cntr->skip_vaccinated->count_by_age_dates(c2, 20, 40, c4, c5);                                 // Count vaccinated records of country for given age and dates
                        total = vir_cntr->skip_vaccinated->count_by_age(c2, 20, 40) + vir_cntr->skip_non_vaccinated->count_by_age(c2, 20, 40); // Count all records of country for given age
                        percentage = 0;
                        if (total != 0)
                        {
                            percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                        }
                        cout << "20-40 " << vaccinated_number << " ";
                        printf("%.2f", percentage);
                        cout << "%" << endl;

                        //40-60
                        vaccinated_number = 0;
                        total = 0;
                        vaccinated_number = vir_cntr->skip_vaccinated->count_by_age_dates(c2, 40, 60, c4, c5);                                 // Count vaccinated records of country for given age and dates
                        total = vir_cntr->skip_vaccinated->count_by_age(c2, 40, 60) + vir_cntr->skip_non_vaccinated->count_by_age(c2, 40, 60); // Count all records of country for given age
                        percentage = 0;
                        if (total != 0)
                        {
                            percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                        }
                        cout << "40-60 " << vaccinated_number << " ";
                        printf("%.2f", percentage);
                        cout << "%" << endl;

                        //60+
                        vaccinated_number = 0;
                        total = 0;
                        vaccinated_number = vir_cntr->skip_vaccinated->count_by_age_dates(c2, 60, 121, c4, c5);                                  // Count vaccinated records of country for given age and dates
                        total = vir_cntr->skip_vaccinated->count_by_age(c2, 60, 121) + vir_cntr->skip_non_vaccinated->count_by_age(c2, 60, 121); // Count all records of country for given age
                        percentage = 0;
                        if (total != 0)
                        {
                            percentage = ((float)vaccinated_number / (float)total) * 100; // Calculate vaccinated percentage for given age
                        }
                        cout << "60+ " << vaccinated_number << " ";
                        printf("%.2f", percentage);
                        cout << "%" << endl;
                        cout << endl;
                    }
                    else
                    {
                        cout << "Virus "<< c3 << " does not exist"<< endl;
                    }
                }
                else // Not valid parameters
                {
                    cout << endl;
                    cout << "Correct Usage:" << endl;
                    cout << "/popStatusByAge [country] virusName [date1 date2]" << endl;
                    cout << endl;
                }
            }
        }
        else if (strcmp(c1, "/insertCitizenRecord") == 0)
        {
            c2 = strtok(NULL, "\n ");
            c3 = strtok(NULL, "\n ");
            c4 = strtok(NULL, "\n ");
            c5 = strtok(NULL, "\n ");
            c6 = strtok(NULL, "\n ");
            c7 = strtok(NULL, "\n ");
            c8 = strtok(NULL, "\n ");
            c9 = strtok(NULL, "\n ");
            c10 = strtok(NULL, "\n ");

            if (!isnum(c2) || !isnum(c6) || !c8 || c10) // Check if parameters are correct
            {
                cout << endl;
                cout << "Correct Usage:" << endl;
                cout << "/insertCitizenRecord citizenID firstName lastName country age virusName NO" << endl;
                cout << "OR" << endl;
                cout << "/insertCitizenRecord citizenID firstName lastName country age virusName YES dateVaccinated" << endl;
                cout << endl;
            }
            else if (c9) // If there is a date
            {
                if ((strcmp(c8, "YES") == 0 && valid_date(c9))) // If vaccinated is YES and date is valid
                {
                    citizen *cit_pointer = hash_citizens[myhash(atoi(c2), record_buckets)].insert(atoi(c2), c3, c4, hash_countries[sdbm(c5, country_buckets)].insert_return(c5), atoi(c6));
                    if (cit_pointer) // Record is valid
                    {
                        hash_viruses[sdbm(c7, virus_buckets)].new_insert(c7, bloomsize, cit_pointer, c8, c9, c5);
                    }
                    else // Record is invalid
                    {
                        cout << "ERROR IN RECORD " << c2 << " " << c3 << " " << c4 << " " << c5 << " " << c6 << " " << c7 << " " << c8 << c9 << endl;
                    }
                }
                else // Vaccinated is falsely NO or date is not valid
                {
                    cout << endl;
                    cout << "Correct Usage:" << endl;
                    cout << "/insertCitizenRecord citizenID firstName lastName country age virusName NO" << endl;
                    cout << "OR" << endl;
                    cout << "/insertCitizenRecord citizenID firstName lastName country age virusName YES dateVaccinated" << endl;
                    cout << endl;
                }
            }
            else if ((strcmp(c8, "NO") == 0)) // If there is no date and vaccinated is NO
            {
                citizen *cit_pointer = hash_citizens[myhash(atoi(c2), record_buckets)].insert(atoi(c2), c3, c4, hash_countries[sdbm(c5, country_buckets)].insert_return(c5), atoi(c6));
                if (cit_pointer) // Record is valid
                {
                    hash_viruses[sdbm(c7, virus_buckets)].new_insert(c7, bloomsize, cit_pointer, c8, "0", c5);
                }
                else // Record is invalid
                {
                    cout << "ERROR IN RECORD " << c2 << " " << c3 << " " << c4 << " " << c5 << " " << c6 << " " << c7 << " " << c8 << endl;
                }
            }
            else // Vaccinated is falsely YES
            {
                cout << endl;
                cout << "Correct Usage:" << endl;
                cout << "/insertCitizenRecord citizenID firstName lastName country age virusName NO" << endl;
                cout << "OR" << endl;
                cout << "/insertCitizenRecord citizenID firstName lastName country age virusName YES dateVaccinated" << endl;
                cout << endl;
            }
        }
        else if (strcmp(c1, "/vaccinateNow") == 0)
        {
            c2 = strtok(NULL, "\n ");
            c3 = strtok(NULL, "\n ");
            c4 = strtok(NULL, "\n ");
            c5 = strtok(NULL, "\n ");
            c6 = strtok(NULL, "\n ");
            c7 = strtok(NULL, "\n ");
            c8 = strtok(NULL, "\n ");

            if (!isnum(c2) || !isnum(c6) || !c7 || c8) // Check if parameters are correct
            {
                cout << endl;
                cout << "Correct Usage:" << endl;
                cout << "/vaccinateNow citizenID firstName lastName country age virusName" << endl;
                cout << endl;
            }
            else
            {
                citizen *cit_pointer = hash_citizens[myhash(atoi(c2), record_buckets)].insert(atoi(c2), c3, c4, hash_countries[sdbm(c5, country_buckets)].insert_return(c5), atoi(c6));
                if (cit_pointer) // Record is valid
                {
                    time_t t = time(0);
                    struct tm *now = localtime(&t);
                    string dd;
                    string mm;
                    int day = now->tm_mday;      // Get day
                    int month = now->tm_mon + 1; // Get month
                    if (day < 10)
                        dd = "0" + to_string(day);
                    else
                        dd = to_string(day);
                    if (month < 10)
                        mm = "0" + to_string(month);
                    else
                        mm = to_string(month);
                    string yyyy = to_string(now->tm_year + 1900);                                                                           // Get year
                    hash_viruses[sdbm(c7, virus_buckets)].vaccinate_now(c7, bloomsize, cit_pointer, "YES", dd + "-" + mm + "-" + yyyy, c5); // Insert with today's date
                }
                else // Record is invalid
                {
                    cout << "ERROR IN RECORD " << c2 << " " << c3 << " " << c4 << " " << c5 << " " << c6 << " " << c7 << endl;
                }
            }
        }
        else if (strcmp(c1, "/list-nonVaccinated-Persons") == 0)
        {
            c2 = strtok(NULL, "\n ");
            c3 = strtok(NULL, "\n ");

            if (!c2 || c3) // Check if parameters are correct
            {
                cout << endl;
                cout << "Correct Usage:" << endl;
                cout << "list-nonVaccinated-Persons virusName" << endl;
                cout << endl;
            }
            else // Print non vaccinated records of the given virus
            {
                hash_viruses[sdbm(c2, virus_buckets)].printnonVaccinated(c2);
            }
        }
        else if ((strcmp(c1, "/help") == 0)) // Show help menu
        {
            cout << "--------------------" << endl;
            cout << "Queries" << endl;
            cout << endl;
            cout << "/vaccineStatusBloom citizenID virusName" << endl;
            cout << "/vaccineStatus citizenID virusName OR /vaccineStatus citizenID" << endl;
            cout << "/populationStatus [country] virusName [date1 date2]" << endl;
            cout << "/popStatusByAge [country] virusName [date1 date2]" << endl;
            cout << "/insertCitizenRecord citizenID firstName lastName country age virusName NO" << endl;
            cout << "OR" << endl;
            cout << "/insertCitizenRecord citizenID firstName lastName country age virusName YES date" << endl;
            cout << "/vaccinateNow citizenID firstName lastName country age virusName" << endl;
            cout << "/list-nonVaccinated-Persons virusName" << endl;
            cout << "/exit" << endl;
            cout << "--------------------" << endl;
        }
        else // Not a query
        {
            cout << "Query not found, type /help for instructions" << endl;
        }
        cout << "TYPE YOUR QUERY" << endl;
        fgets(command, 120, stdin);
    }
}