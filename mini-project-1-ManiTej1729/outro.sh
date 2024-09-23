while IFS= read -r line; do
  echo "$line"
  sleep 0.1  # Delay in seconds
done < outro.txt

exit 0