#!/bin/bash

# Run header.sh -c to copy the header to the individual test directory.
# This is needed for arduino project/sketch directory structure.

HEADER=pin_definitions.h

if [ -f $HEADER ]
then
  if [ $1 == "-c" ]
  then
    for f in ./*
    do
      if [ -d "$f" ]
      then
        echo "copying $HEADER to $f"
        cp $HEADER $f
      fi
    done
  elif [ $1 == "-r" ]
  then
    for f in ./*
    do
      if [ -d "$f" ]
      then
        echo "Removing $HEADER from $f"
        rm $f/$HEADER
      fi
    done
  fi
else
  echo "File: $HEADER not found"
  exit
fi
