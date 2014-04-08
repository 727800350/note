#!/bin/sh
# ntopng init script

# **NOTE** bash will exit immediately if any command exits with non-zero.
set -e

PACKAGE_NAME=ntopng
PACKAGE_DESC="ntopng server"
PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:${PATH}

start() {
    echo "Starting ${PACKAGE_DESC}: "
## nohup: Run a Command or Shell-Script Even after You Logout
## redirect stdout to a log file and redirect stderr to stdout
    nohup /usr/local/bin/ntopng -i wlan0 -w 3000 -U Eric -l > /var/log/ntopng.log 2>&1 &
    echo $! > /var/tmp/${PACKAGE_NAME}.pid
    echo "${PACKAGE_NAME}."
}

stop() {
    echo "Stopping ${PACKAGE_DESC}: "
    kill -9 `cat /var/tmp/${PACKAGE_NAME}.pid` || true
    echo "${PACKAGE_NAME}."
}

restart() {
    stop
    sleep 1
    start
}

usage() {
    N=$(basename "$0")
    echo "Usage: [sudo] $N {start|stop|restart}" >&2
    exit 1
}

if [ "$(id -u)" != "0" ]; then
    echo "please use sudo to run ${PACKAGE_NAME}"
    exit 0
fi

case "$1" in
    # If no arg is given, start the goagent.
    # If arg `start` is given, also start goagent.
    '' | start)
        start
        ;;
    stop)
        stop
        ;;
    restart | force-reload)
        restart
        ;;
    *)
        usage
        ;;
esac

exit 0
