#! /usr/bin/env bash

# $1: CHANGELOG file
# $2: Distributions
# $3: Metadata

CHANGELOG="${1:-/dev/stdin}"
DISTRIBUTIONS="${2:-unstable}"
METADATA="${3:-urgency=medium}"

if ! [[ ( -f "${CHANGELOG}" || -p "${CHANGELOG}" ) && -r "${CHANGELOG}" ]]
then
	echo "Error: Invalid or unreadable CHANGELOG file: ${CHANGELOG}"
	exit 1
fi

# https://manpages.debian.org/testing/dpkg-dev/deb-changelog.5.en.html
sed -E \
	-e "s/^(\S+ \(\S+\))$/\1 ${DISTRIBUTIONS}; ${METADATA}/g"                       `#Apt distributions/metadata` \
	-e 's/^( -- .+ <\S+@\S+>  )([0-9]{1,2} [a-zA-Z]{3} [0-9]{4})$/echo "\1$(date -R --date "\2")"/e' `#Timestamp` \
	"${CHANGELOG}"
