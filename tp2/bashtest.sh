#!/bin/bash

# Liste des tailles d'alphabet à utiliser
alphabet_sizes=(2 4 20 70)

# La longueur du texte à générer
text_length=500000

# La liste des longueurs de mots à utiliser
word_lengths=(4 5 6 7 8 9 10 15 20 30 40 50)

# La longueur des listes à générer
list_length=100

# La liste des numéros d'algorithmes à utiliser
algo_nums=(1 2 3 4 5 6 7)

# Le nom du répertoire de sortie
output_dir="output"

# Créer le répertoire de sortie
mkdir -p "$output_dir"

# Créer un répertoire temporaire
temp_dir=$(mktemp -d)

# Générer les fichiers de texte avec singleStringText_generator
for size in "${alphabet_sizes[@]}"; do
  ./singleStringText_generator $text_length $size "$temp_dir/text-$size.txt"
done

# Générer les fichiers de liste avec wordList_generator
for size in "${alphabet_sizes[@]}"; do
  for length in "${word_lengths[@]}"; do
    ./wordList_generator $list_length $length $size "$temp_dir/list-$length-$size.txt"
  done
done

# Pour chaque algorithme
for algo_num in "${algo_nums[@]}"; do
  # Créer un fichier CSV
  csv_file="$output_dir/algo-$algo_num.csv"
  touch "$csv_file"
  # Écrire l'en-tête du fichier CSV
  echo "wordLength,alphabetSize,result" > "$csv_file"
  for size in "${alphabet_sizes[@]}"; do
    for length in "${word_lengths[@]}"; do
      text_file="$temp_dir/text-$size.txt"
      list_file="$temp_dir/list-$length-$size.txt"
      # Exécuter l'algorithme et écrire le résultat dans le fichier CSV
      result=$(./averageTimes $text_length $size $length $list_length $algo_num "$text_file" "$list_file")
      echo "$length,$size,$result" >> "$csv_file"
    done
  done
done

# Supprimer le répertoire temporaire
rm -r "$temp_dir"