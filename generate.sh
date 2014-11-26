#!/bin/bash

all_names=()

for cfile in src/programs/*.c; do
  base="$(basename "$cfile")"
  name="$(basename "$base" ".c")"
  all_names+=("$name")
  echo "extern int impl_${name}_main(int argc, char** argv);"
done

echo
echo "static const struct ProgramPair all_programs[] = {"
for name in ${all_names[*]}; do
  echo "    {\"$name\", \"description-for-$name\", impl_${name}_main}, "
done
echo "    {NULL, NULL, NULL},"
echo "};"
echo
