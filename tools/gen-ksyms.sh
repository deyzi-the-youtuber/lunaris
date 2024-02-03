#!/bin/sh

set -e

TMP1=$(mktemp)
TMP2=$(mktemp)
TMP3=$(mktemp)

if [ -z "$OBJDUMP" ]; then
    OBJDUMP=objdump
fi

${OBJDUMP} -t lunaris | sed '/\bd\b/d' | sort > "$TMP1"
grep "\.text" < "$TMP1" | cut -d' ' -f1 > "$TMP2"
grep "\.text" < "$TMP1" | awk 'NF{ print $NF }' > "$TMP3"

cat <<EOF >symlist.c
#include <kernel/ksyms.h>

ksyms_t symlist[] = {
EOF

paste -d'$' "$TMP2" "$TMP3" | sed 's/^/    {0x/g' | sed 's/\$/, "/g' | sed 's/$/"},/g' >> symlist.c

cat <<EOF >> symlist.c
  {0xffffffff, ""}
};
EOF

rm "$TMP1" "$TMP2" "$TMP3"