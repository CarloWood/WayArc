#! /bin/bash

INPUT_FILE="$1"

if [[ -z "$CXX" ]]; then
  echo "CXX is not set."
  exit 1
fi

if [[ -z "$BUILDDIR" ]]; then
  echo "BUILDDIR is not set."
  exit 1
fi

if [[ -z "$REPOBASE" ]]; then
  echo "REPOBASE is not set (set to the root of the project)."
  exit 1
fi
   
${CXX} -E -D__WAYARC_NL_MACRO__=__NEWLINE__ -I "${REPOBASE}" -I "${REPOBASE}/src" "${INPUT_FILE}" | tac | sed '/namespace events/q' | tac | sed 's/^#.*//;s/__NEWLINE__/\n/g' | sed 's/ *$//'
