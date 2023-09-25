#!/bin/bash

SLEEP_TIME=0.5

function register {
    echo "register" | tee >(cat >&2)    && sleep $SLEEP_TIME
}

function login {
    echo "login"    | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "a12a"     | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "aaaa"     | tee >(cat >&2)    && sleep $SLEEP_TIME
}
function login_w {
    echo "login"    | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "a22a"     | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "aaaa"     | tee >(cat >&2)    && sleep $SLEEP_TIME
}
function login_w2 {
    echo "login"    | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "a12a"     | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "aaba"     | tee >(cat >&2)    && sleep $SLEEP_TIME
}

function get_books
{
    echo "get_books"| tee >(cat >&2)    && sleep $SLEEP_TIME
}

function enter_library
{
    echo "enter_library"| tee >(cat >&2)    && sleep $SLEEP_TIME
}

function get_book
{
    echo "get_book" | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "1331" | tee >(cat >&2)    && sleep $SLEEP_TIME

}

function get_book_w
{
    echo "get_book" | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "1335" | tee >(cat >&2)    && sleep $SLEEP_TIME

}

function add_book
{
    echo "add_book" | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "titlu" | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "autor" | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "gen" | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "Humanitas" | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "43" | tee >(cat >&2)    && sleep $SLEEP_TIME
}
function add_book_no_nr
{
    echo "add_book" | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "titlu" | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "autor" | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "gen" | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "Humanitas" | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "asdf" | tee >(cat >&2)    && sleep $SLEEP_TIME
}

function delete_book
{
    echo "delete_book" | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "2422" | tee >(cat >&2)    && sleep $SLEEP_TIME
}

function delete_book_w
{
    echo "delete_book" | tee >(cat >&2)    && sleep $SLEEP_TIME
    echo "43" | tee >(cat >&2)    && sleep $SLEEP_TIME
}

function logout
{
    echo "logout" | tee >(cat >&2)    && sleep $SLEEP_TIME
}

# login
# add_book
# enter_library
#
# echo "exit" | tee >(cat >&2)
# echo ""



# enter_library

# add_book
# get_books
# get_book
# delete_book
# delete_book_w
# get_books

login_w
login_w2
login
login_w
login_w2
login

sleep 5

# add_book
# get_books
# get_book
# delete_book
# delete_book_w
# get_books

enter_library
enter_library

add_book
get_books
get_book
delete_book
delete_book_w
get_books

echo "exit" | tee >(cat >&2)
echo ""
