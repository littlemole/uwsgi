#!/bin/bash

# simple integration test.
# * starts a uwsgi server in the background
# * issues a requests
# * tests for success 200 OK
# * terminates the uwsgi server

# this function runs forked
runner() {
    uwsgi --plugin-dir=$(pwd)/ --plugin cpp --cpp-dir=$(pwd)/ --plugin http --http=127.0.0.1:3032 --http-modifier1=250 --pidfile=$PIDFILE
}

# variables
EXIT=1
PIDFILE="$(pwd)/test.pid"
echo $PIDFILE
rm $PIDFILE

# start the uwsgi server
runner &

# wait for the server to launch
sleep 1

# fetch PID of server main process
CHILD_PID=$(cat $PIDFILE)

# execute a test request, put body into variable
BODY=$(curl -XGET "http://localhost:3032/test/admi")

# curl exit code
RESULT=$?

# check if curl returned success (0)
# and set exit code
if [ "$RESULT" = "0" ]
then
    echo "SUCCESS"    
    EXIT=0
else
    echo "FAIL"
    echo "[$RESULT]"    
fi

# shutdown the uwsgi server
echo "shutdown uwsgi server - kill -INT $CHILD_PID"
kill -INT $CHILD_PID

# cleanup
rm $PIDFILE

# exit with exit code
exit $EXIT

