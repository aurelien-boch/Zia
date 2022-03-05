#!/bin/sh

echo "Running tests..."
nb_tests=0
nb_success=0
nb_failure=0

result () {
    echo "Number of tests: $nb_tests"
    echo "Number of success: $nb_success"
    echo "Number of failure: $nb_failure"
    echo "Success rate: $(($nb_success * 100 / $nb_tests))%"
}

cli_test () {
    nb_tests=$((nb_tests + 1))
    echo "$1" | ./zia > ../../tests/test${2}.txt
    diff ../../tests/test${2}.txt ../../tests/test${2}.ref.txt

    if [ $? -eq 0 ]; then
        nb_success=$((nb_success + 1))
        echo "Test passed"
    else
        nb_failure=$((nb_failure + 1))
        echo "Test failed"
    fi
}

cli_test "config plif\nexit\n" 1
cli_test "config plif\nstart plif\n\n\n\n\n\nstop plif\nexit\n" 2
cli_test "start plif\n\n\n\n\nexit\n" 3
cli_test "config toto\nexit\n" 4
cli_test "stop plif\nexit\n" 5
cli_test "start toto\nexit\n" 6
cli_test "exit\n" 7
cli_test "help\nexit\n" 8
cli_test "config plouf\nconfig plif\nstart plouf\n\n\n\n\n\nstop plouf\n\nexit\n" 9

result

# echo -n "toto" | nc localhost 8080 | (grep 400 && echo "Success") || echo "Failure"
# echo -n "pute\r\n" | nc localhost 8080 | (grep 400 && echo "Success") || echo "Failure"
# echo -n "TOTO / HTTP/1.0\r\n" | nc localhost 8080 | (grep 400 && echo "Success") || echo "Failure"
# echo -n "GET bite HTTP/1.0\r\n" | nc localhost 8080 | (grep 400 && echo "Success") || echo "Failure"
# echo -n "GET / HTTP/42.0\r\n" | nc localhost 8080 | (grep 400 && echo "Success") || echo "Failure"
# echo -n "GET / HTTP/1.0" | nc localhost 8080 | (grep 400 && echo "Success") || echo "Failure"
# echo -n "GET /user?id=aureline HTTP1.1\r\nContent-Type: application/json\r\n" | nc localhost 8080 | (grep 400 && echo "Success") || echo "Failure"
