#!/bin/sh

# echo "Running tests..."

# echo "Config of 1 pipeline"
func_test () {
    echo -ne "\n$1" | ./zia > ../../tests/test${2}.txt
    diff ../../tests/test${2}.txt ../../tests/test${2}.ref.txt

    if [ $? -eq 0 ]; then
        echo "Test passed"
    else
        echo "Test failed"
    fi
}

func_test "config plif\nexit\n" 1
func_test "config plif\nstart plif\nstop plif\nexit\n" 2

# echo -n "toto" | nc localhost 8080 | (grep 400 && echo "Success") || echo "Failure"
# echo -n "pute\r\n" | nc localhost 8080 | (grep 400 && echo "Success") || echo "Failure"
# echo -n "TOTO / HTTP/1.0\r\n" | nc localhost 8080 | (grep 400 && echo "Success") || echo "Failure"
# echo -n "GET bite HTTP/1.0\r\n" | nc localhost 8080 | (grep 400 && echo "Success") || echo "Failure"
# echo -n "GET / HTTP/42.0\r\n" | nc localhost 8080 | (grep 400 && echo "Success") || echo "Failure"
# echo -n "GET / HTTP/1.0" | nc localhost 8080 | (grep 400 && echo "Success") || echo "Failure"
# echo -n "GET /user?id=aureline HTTP1.1\r\nContent-Type: application/json\r\n" | nc localhost 8080 | (grep 400 && echo "Success") || echo "Failure"
