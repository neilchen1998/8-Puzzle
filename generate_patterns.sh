#!/bin/bash

# some vars
BINARY="./build/apps/app"
DB="./training-patterns"
BACKUP="${DB}-backup"
COUNT=50
BAD_CNT=0
TIMEOUT_CNT=0
RESULT_FILE="result.txt"

echo "" &>> $RESULT_FILE # new line
echo "*** Current date: $(date) ***" &>> $RESULT_FILE

# configures the repo
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# first compiles the code
cmake --build build

# starts the timer
start_time=$(date +%s)

for ((i = 0; i < $COUNT; ++i)); do

    # backup the database
    cp $DB $BACKUP

    # runs the binary and captures the return code
    timeout 120s $BINARY &>> $RESULT_FILE

    # checks the return code
    exit_code=$?

    # checks if the exit code is 'timeout'
    if [ $exit_code -eq 124 ]
    then

        echo Timeout!!! &>> $RESULT_FILE

        TIMEOUT_CNT=$((TIMEOUT_CNT+1))    # arithmetic

        # restore database
        cp $BACKUP $DB

    elif [ $exit_code -ne 0 ]
    then
        echo Error!!! &>> $RESULT_FILE

        BAD_CNT=$((BAD_CNT+1))    # arithmetic

        # restore database
        cp $BACKUP $DB
    fi

done

# ends the timer
end_time=$(date +%s)

duration=$((end_time - start_time))

# prints the stats
echo Number of timeouts: $TIMEOUT_CNT &>> $RESULT_FILE
echo Number of bad counts: $BAD_CNT &>> $RESULT_FILE
echo "*** Took $duration seconds for $COUNT trainings ***" &>> $RESULT_FILE
