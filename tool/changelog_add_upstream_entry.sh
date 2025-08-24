#! /usr/bin/env bash

# $1: CHANGELOG file

if [ -z "${1}" ]
then
	echo "Error: No CHANGELOG file given"
	exit 1
elif [ ! -w "${1}" ]
then
	echo "Error: Invalid or unwritable CHANGELOG file: ${1}"
	exit 1
fi

CHANGELOG="$(realpath "${1}")"
DIR="$(dirname "${CHANGELOG}")"
cd "${DIR}"
GIT_DIR="$(git rev-parse --show-toplevel)"

if [ "${DIR}" != "${GIT_DIR}" ]
then
	echo "\
Error: CHANGELOG is not at top level of a git repository
	CHANGELOG path: ${CHANGELOG}
	Git directory: ${GIT_DIR}\
"
	exit 1
fi

PROJECT="$(head -1 "${CHANGELOG}" | cut -f 1 -d ' ')"
CHANGELOG_LAST_VERSION="$(head -1 "${CHANGELOG}" | sed -E 's/^\S+ \((\S+)\)$/\1/')"
GIT_HEAD_VERSION="$(git describe --tags --abbrev=7 | sed 's/-/+/g')"

if [ "${GIT_HEAD_VERSION}" != "${CHANGELOG_LAST_VERSION}" ]
then
	unset ENTRY
	ENTRY+="${PROJECT} (${GIT_HEAD_VERSION})\n\n"
	ENTRY+="  * Package from upstream sources\n\n"
	ENTRY+=" -- Thomas Lepoix <thomas.lepoix@protonmail.ch>  $(LC_TIME=POSIX date '+%d %b %Y')\n"

	sed -i "${CHANGELOG}" -e "1i ${ENTRY}"
	git --no-pager diff --color=always "${CHANGELOG}"
else
	echo "Nothing to do: Git HEAD already on last CHANGELOG entry: ${GIT_HEAD_VERSION}"
fi
