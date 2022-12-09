# #!/bin/bash

# # Set the length of the text
# text_length=500000

# # Set the alphabet sizes
# alphabet_sizes=(2 4 20 70)

# # Loop through the alphabet sizes
# for alphabet_size in "${alphabet_sizes[@]}"
# do
#     # Execute the singleStringText_generator program with the specified arguments
#     ./singleStringText_generator $text_length $alphabet_size output_file_$alphabet_size
# done

#!/bin/bash

# Set the text length and alphabet sizes
text_length=500000
alphabet_sizes=(2 4 20 70)

# Set the list lengths and word lengths
list_lengths=(100)
word_lengths=(4 5 6 7 8 9 10 15 20 30 40 50)

# Loop through the alphabet sizes
for alphabet_size in "${alphabet_sizes[@]}"
do
    # Generate a text file using singleStringText_generator
    ./singleStringText_generator $text_length $alphabet_size text_$alphabet_size.txt

    # Loop through the list lengths
    for list_length in "${list_lengths[@]}"
    do
        # Loop through the word lengths
        for word_length in "${word_lengths[@]}"
        do
            # Generate a word list using wordList_generator
            ./wordList_generator $list_length $word_length $alphabet_size list_${list_length}_${word_length}_$alphabet_size.txt

            # Store the output of the demo program in an array
            output_array=($(./demo text_$alphabet_size.txt list_${list_length}_${word_length}_$alphabet_size.txt))

            # Loop through the elements in the output array
            for output in "${output_array[@]}"
            do
                # Append the output to the CSV file, along with the alphabet size
                echo "$alphabet_size,$output" >> output.csv
            done
        done
    done
done
