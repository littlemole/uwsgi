#!/bin/bash

UWSGI=$(which uwsgi)
UWSGI_VERSION=$( $UWSGI --version) 

if [[ "$V" > "2.0.0" ]]
then 
	echo -DMOL_UWSGI_VERSION_2
fi

