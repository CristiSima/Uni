#!/bin/bash


while [[ "$(docker stack rm sprc3 2>&1)" != "Nothing found in stack: sprc3" ]]
do
    sleep 1
    echo Waiting..
done

if [[ $1 = "-v" ]]; then
    docker volume rm sprc3_db_data sprc3_db_config
    echo "Everything was removed (including volumes)"
    exit 0
fi

echo "Stack was removed"