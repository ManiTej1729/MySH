welcome_msg="Welcome to MyShell:"

for (( i=0; i<${#welcome_msg}; i++ )); do
  printf "%s" "${welcome_msg:$i:1}"
  sleep 0.05
done

printf "\n"

for i in {0..100}; do
  printf "\rLoading: %d%%" "$i"
  sleep 0.02
done
printf "\n"

random_words=("Qsphsbn tvddfttgvmmz mpbefe!" "Rtqitco uweeguuhwnna nqcfgf!" "Surjudp vxffhvvixoob ordghg!" "Tvskveq wyggiwwjyppc psehih!" "Uwtlwfr xzhhjxxkzqqd qtfiji!" "Vxumxgs yaiikyylarre rugjkj!" "Wyvnyht zbjjlzzmbssf svhklk!" "Xzwoziu ackkmaancttg twilml!" "Yaxpajv bdllnbboduuh uxjmnm!" "Zbyqbkw cemmoccpevvi vyknon!")
for element in "${random_words[@]}"; do
  printf "\r$element"
  sleep 0.1
done

for i in {0..10}; do
  if (( i % 2 == 0 )); then
    printf "\rProgram successfully loaded!"
  else
    printf "\r                            "
  fi
  sleep 0.1
done

printf "\n"

while IFS= read -r line; do
  echo "$line"
  sleep 0.1  # Delay in seconds
done < intro.txt

exit 0