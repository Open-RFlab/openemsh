#! /usr/bin/env bash

# $1: CHANGELOG file

if [ -z "${1}" ]
then
	echo "Error: No CHANGELOG file given"
	exit 1
elif [ ! -f "${1}" ] || [ ! -r "${1}" ]
then
	echo "Error: Invalid or unreadable CHANGELOG file: ${1}"
	exit 1
fi

CHANGELOG="$(realpath "${1}")"

sed -E \
	-e 's/  \*/-/g'                                 `#Bullet points` \
	-e 's/    \*/  -/g'                             `#Bullet points` \
	-e 's/ -- ([^<]+)<.*> +(.*)/*\1 - \2*\n\n---/g' `#Signature/Date line` \
	-e "s/(\S+ \([0-9.]+\))/### \1/g"               `#Release title line` \
	-e 's/  \[ (.*) \]/#### \1/g'                   `#Category line` \
	"${CHANGELOG}"
