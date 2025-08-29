#! /usr/bin/env bash

# $1: Entry
# $2: CHANGELOG file

CHANGELOG="${2:-/dev/stdin}"

if ! [[ ( -f "${CHANGELOG}" || -p "${CHANGELOG}" ) && -r "${CHANGELOG}" ]]
then
	echo "Error: Invalid or unreadable CHANGELOG file: ${CHANGELOG}"
	exit 1
fi

if [ "${1}" == "last" ]
then
	VERSION='\S+'
else
	VERSION="$(echo "${1}" | sed 's/./[&]/g')"
fi

# https://unix.stackexchange.com/a/180678
sed -nE \
	-e "/^\S+ \(${VERSION}\)$/,\${p;/^ \-\- .*$/q}" \
	"${CHANGELOG}"
