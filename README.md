# vaccine-monitor

In this project we were asked to implement a program that accepts, processeses, records and answers questions about vaccinations of citizens.

I created a set of data structures(bloom filters, skip lists, hash tables) that import and query large volumes of records.

## Execution

To compile:
```
make
```
To execute(Default parameters):
```
make run
```
To execute:
```
./diseaseMonitor -c inputFile -b bloomsize
```
To delete executables:
```
make clean
```
## Implementation
The application first reads the given input file line by line, and stores the correct records in the data structures like the architecture below. If a record is not valid it is not stored and an error message is printed. When this process is finished the application waits for input from the user.

![Application Architecture](https://user-images.githubusercontent.com/49104172/169171607-b6e9d35b-d488-457f-91ee-604713243282.jpg)
## Queries
*Arguments in [ ] are optional

```
/vaccineStatusBloom citizenID virusName
```
The application checks the bloomfilter associated with the virusName given and prints "VACCINATED" or "NOT VACCINATED" accordingly to the given citizenID
<br />
<br />
```
/vaccineStatus citizenID [virusName]
```
Without virusName:<br />
The application checks every skip list(every virus) and if the given citizenID is on any of them(vaccinated or not) it prints tha name of the virus and the answer(e.g: "SARS-1 NO", "MERS-COV YES 11-01-2011")

With virusName:<br />
The application first checks the bloomfilter and if the answer is NO then it prints "NOT VACCINATED". If the answer is YES the application checks the vaccinated skip list of the given virusName and if the citizenID is found then it prints "VACCINATED ON [date]", else it also prints "NOT VACCINATED".
<br />
<br />
```
/populationStatus [country] virusName [date1 date2]
```
Only virusName given:<br />
The application returns the number and percentage of vaccinated people in each country.

Only virusName and country given:<br />
The application returns the number and percentage of vaccinated people in the given country.

Only virusName and dates given:<br />
The application returns the number and percentage of vaccinated people in each country between the given dates.

All parameters:<br />
The application returns the number and percentage of vaccinated people in the given country between the given dates.
<br />
<br />
```
/popStatusByAge [country] virusName [date1 date2]
```
Only virusName given:<br />
The application returns the number and percentage of vaccinated people in each country by age.

Only virusName and country given:<br />
The application returns the number and percentage of vaccinated people in the given country by age.

Only virusName and dates given:<br />
The application returns the number and percentage of vaccinated people in each country between the given dates by age.

All parameters:<br />
The application returns the number and percentage of vaccinated people in the given country between the given dates by age.
<br />
<br />
```
/insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date]
```
If the given record is correct then it is added to the data structures, else an error message is printed.
<br />
<br />
```
/vaccinateNow citizenID firstName lastName country age virusName
```
If the given record does not exist yet, then it works like /insertCitizenRecord with the current system date given.
If the record exists it changes vaccinationStatus to "YES" with the current system date given.
<br />
<br />
```
/list-nonVaccinated-Persons virusName
```
The application prints all records that are not vaccinated for the given virusName.
<br />
<br />
```
/help
```
The application prints instructions on how to use it.

## Bash Script
A script that takes as arguments a file with country names, a file with virus names, a number of lines, creates random records and puts them in a file.