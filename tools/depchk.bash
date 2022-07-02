#!/bin/bash

set -eou pipefail

PROGRAMS=(
	'python3'
)

FORMAT_COLOR_RED="\033[1;31m"
FORMAT_COLOR_GREEN="\033[1;32m"
FORMAT_BOLD="\033[1m"
FORMAT_NONE="\033[0m"

YES="${FORMAT_BOLD}${FORMAT_COLOR_GREEN}YES${FORMAT_NONE}"
NO="${FORMAT_BOLD}${FORMAT_COLOR_RED}NO${FORMAT_NONE}"

function program_chk()
{
	local path

	for program in "${PROGRAMS[@]}"; do
		if path=$(command -v "${program}"); then
			printf \
				'program %b found: %b (%s)\n' \
				"${FORMAT_BOLD}${program}${FORMAT_NONE}" \
				"$YES" \
				"$path"
		else
			printf \
				'program %b found: %b\n' \
				"${FORMAT_BOLD}${program}${FORMAT_NONE}" \
				"$NO"
		fi
	done
}

program_chk
