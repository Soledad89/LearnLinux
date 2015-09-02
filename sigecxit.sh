#!/bin/bash

function signal_handler {
	echo "hello, bob"
}

trap signal_handler 0
