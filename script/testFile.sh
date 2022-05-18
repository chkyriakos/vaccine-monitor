#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ] || [ "$3" -le 0 ] || [ -z "$4" ] || [ "$4" != 1 ] && [ "$4" != 0 ]; then # Check if parameters exist
    echo "Some parameters are missing or they are wrong"
    echo "Correct Usage:"
    echo "./testFile.sh virusesFile countriesFile numLines(number > 0) duplicatesAllowed(0 or 1)"
    exit 0
fi

viruses="$1"
countries="$2"
numlines="$3"
duplicates="$4"

if [ -f "inputFile" ]; then # Delete existing files
    rm -f inputFile
fi

if [ -f "IDs" ]; then
    rm -f IDs
fi

if [ -f "inputFileTemp" ]; then
    rm -f inputFileTemp
fi

if [ $duplicates -eq 0 -a $numlines -le 10000 ]; then # Duplicates not allowed
    shuf -i 0-9999 -n $numlines >>IDs
    while IFS= read -r line; do
        citizenID=$line
        n=$(($RANDOM % 9 + 3))
        name=$(tr -dc [:alpha:] </dev/urandom | head -c$n)
        n=$(($RANDOM % 9 + 3))
        surname=$(tr -dc [:alpha:] </dev/urandom | head -c$n)
        age=$(($RANDOM % 120 + 1))
        country=$(shuf -n 1 $countries)
        printf "$citizenID $name $surname $country $age\n" >>inputFileTemp
    done <"IDs"
elif [ $duplicates -eq 1 -a $numlines -le 10000 ]; then # Duplicates allowed
    numlines_10=$((numlines * 10 / 100))                # Duplicates are 10% of the total records
    shuf -i 0-9999 -n $((numlines - numlines_10)) >>IDs
    while IFS= read -r line; do
        citizenID=$line
        n=$(($RANDOM % 9 + 3))
        name=$(tr -dc [:alpha:] </dev/urandom | head -c$n)
        n=$(($RANDOM % 9 + 3))
        surname=$(tr -dc [:alpha:] </dev/urandom | head -c$n)
        age=$(($RANDOM % 120 + 1))
        country=$(shuf -n 1 $countries)
        printf "$citizenID $name $surname $country $age\n" >>inputFileTemp
    done <"IDs"
    shuf -n $numlines_10 "inputFileTemp" >>inputFileTemp
else # Duplicates allowed 
    if [ $duplicates -eq 0 ]; then
        printf "WARNING:\n"
        printf "Duplicates are allowed automatically for more than 10000 records\n"
    fi
    shuf -i 0-9999 -n 10000 >>IDs
    for ((i = 1; i <=$numlines; i++)); do
        citizenID=$(shuf -n 1 "IDs")
        n=$(($RANDOM % 9 + 3))
        name=$(tr -dc [:alpha:] </dev/urandom | head -c$n)
        n=$(($RANDOM % 9 + 3))
        surname=$(tr -dc [:alpha:] </dev/urandom | head -c$n)
        age=$(($RANDOM % 120 + 1))
        country=$(shuf -n 1 $countries)
        printf "$citizenID $name $surname $country $age\n" >>inputFileTemp
    done
fi

rm -f IDs

while IFS= read -r line; do # Add virus name and vaccination status
    virus=$(shuf -n 1 $viruses)
    vaccinated=$(($RANDOM % 2))
    if [ $vaccinated = 0 ]; then
        printf "$line $virus NO\n" >>inputFile
    else
        day=$(($RANDOM % 30 + 1))
        if [ $day -lt 10 ]; then
            dd="0"$day
        else
            dd=$day
        fi
        month=$(($RANDOM % 12 + 1))
        if [ $month -lt 10 ]; then
            mm="0"$month
        else
            mm=$month
        fi
        yyyy=$(($RANDOM % 21 + 2000))
        printf "$line $virus YES $dd-$mm-$yyyy\n" >>inputFile
    fi
done <"inputFileTemp"

rm -f inputFileTemp
