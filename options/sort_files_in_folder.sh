for i in *.opt; do
  echo sort file ${i%%}
  sort ${i%%} -o ${i%%}
done