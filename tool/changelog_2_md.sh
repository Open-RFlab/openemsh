#! /usr/bin/env bash

# $1: CHANGELOG file

CHANGELOG="${1:-/dev/stdin}"

if ! [[ ( -f "${CHANGELOG}" || -p "${CHANGELOG}" ) && -r "${CHANGELOG}" ]]
then
	echo "Error: Invalid or unreadable CHANGELOG file: ${CHANGELOG}"
	exit 1
fi

sed -E \
	-e 's/  \*/-/g'                                 `#Bullet points` \
	-e 's/    \*/  -/g'                             `#Bullet points` \
	-e 's/ -- ([^<]+)<.*> +(.*)/*\1 - \2*\n\n---/g' `#Signature/Date line` \
	-e "s/\S+ \((\S+)\)/### \1/g"                   `#Release title line` \
	-e 's/  \[ (.*) \]/#### \1/g'                   `#Category line` \
	"${CHANGELOG}"
