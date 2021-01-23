#!/bin/sh

dir="$1"

cat <<EOF
#ifndef _STAGE_DEFS_H_
#define _STAGE_DEFS_H_

//#include "stages/stage_empty.h"
EOF

for f in $dir/stage_0*.h
do
    b=`basename $f`
    n=`echo $b | cut -c7-10`
    m=`expr \( $n - 1 \) % 16`
    echo "#include \"stages/$b\""
done

cat <<EOF

static int stage_defs_init(stage_t **stages)
{
  int num = 0;

EOF

for f in $dir/stage_0*.h
do
    b=`basename $f`
    n=`echo $b | cut -c7-10`
    m=`expr \( $n - 1 \) % 16`
    echo "  stages[num++] = &stage_$n;"
done


cat <<EOF

  return num;
}

#endif /* _STAGE_DEFS_H_ */
EOF
