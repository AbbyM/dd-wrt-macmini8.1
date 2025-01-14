#!/usr/bin/env bash

set -e

cd "$(dirname "${0}")" || exit 1
. ./common.sh || exit 1

# NDPI_PROTOCOL_CATEGORY_GAMBLING = 107
LIST=../lists/107_gambling.list

printf '(1) %s\n' "Scraping Illegal Gambling Sites (Belgium)"
DOMAINS="$(curl -s 'https://www.gamingcommission.be/en/gaming-commission/illegal-games-of-chance/list-of-illegal-gambling-sites' | sed -n 's/^<td[^>]\+>\(.\+\.[a-zA-Z0-9]\+\)\(\|\/.*[^<]*\)<\/td>/\1/gp' || exit 1)"
is_str_empty "${DOMAINS}" "Please check gambling sites URL and sed REGEX."

printf '(2) %s\n' "Downloading Gambling Sites (Poland)"
DOMAINS_PL="$(curl -s https://hazard.mf.gov.pl/api/Register | xmllint --xpath "/*[local-name(.)='Rejestr']/*[local-name(.)='PozycjaRejestru']/*[local-name(.)='AdresDomeny']/text()" -)"
is_str_empty "${DOMAINS_PL}" "Please check gambling sites URL and XPath."

echo "${DOMAINS}" "${DOMAINS_PL}" | sort | uniq >${LIST}

printf '(3) %s\n' "List ${LIST} is now ready"
exit 0
