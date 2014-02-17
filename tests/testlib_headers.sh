#!/bin/sh

PRIVATE_HEADERS_COUNT=$(find $1 -iname *.private.h | wc -l)

if [ "$PRIVATE_HEADERS_COUNT" -ne 0 ]
then
    find $1 -iname *.private.h;
    exit 1;
fi;

exit 0;
