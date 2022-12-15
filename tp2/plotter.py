import os

import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots


# Directory containing the CSV files
csv_dir = "output"

# Mapping from algorithm number to subplot number
subplot_map = {}
for i in range(1, 8):
    subplot_map[i] = i

# Alphabet sizes to plot
alphabet_sizes = [2, 4, 20, 70]

# Create figure with subplots
fig = make_subplots(
    rows=3,
    cols=3,
    subplot_titles=[
        "Naïf",
        "Naïf, boucle rapide",
        "Naïf, boucle rapide, sentinelle",
        "Naïf, strncmp",
        "Naïf, boucle rapide, strncmp",
        "Naïf, boucle rapide, sentinelle, strncmp",
    ],
    # x_title="Alphabet size",
    # y_title="Average time (s)",
)

# Loop through algorithm numbers
for algo_num in range(1, 8):
    # Create subplot for this algorithm number
    subplot = subplot_map[int(algo_num)]

    # Loop through alphabet sizes
    for size in alphabet_sizes:
        # Read CSV file for this algorithm number and alphabet size into DataFrame
        df = pd.read_csv(f"{csv_dir}/averageTimes-{algo_num}-{size}.csv")

        # Add line plot for this alphabet size to subplot
        fig.add_scatter(
            x=df["alphabet_size"],
            y=df["average_time"],
            mode="lines+markers",
            name=f"size={size}",
            legendgroup=f"algo={algo_num}",
            showlegend=size == alphabet_sizes[0],
            xaxis=f"x{subplot}",
            yaxis=f"y{subplot}",
        )
        fig.update_xaxes(title_text="Taille de l'alphabet")
        fig.update_yaxes(title_text="Temps moyen (ms)")
        fig.update_yaxes(range=[0, 0.7])

# Update layout
fig.update_layout(
    title="Average time to generate a random string",
    height=1000,
    width=1000,
    legend_title="Alphabet size",
)

# Show figure
fig.show()

# Create figure with subplots
fig2 = make_subplots(
    rows=3,
    cols=3,
    subplot_titles=[
        "Naïf",
        "Naïf, boucle rapide",
        "Naïf, boucle rapide, sentinelle",
        "Naïf, strncmp",
        "Naïf, boucle rapide, strncmp",
        "Naïf, boucle rapide, sentinelle, strncmp",
    ],
    # x_title="Alphabet size",
    # y_title="Average time (s)",
)

# Loop through algorithm numbers
for algo_num in range(1, 8):
    # Create subplot for this algorithm number
    subplot = subplot_map[int(algo_num)]

    # Loop through alphabet sizes
    for size in alphabet_sizes:
        # Read CSV file for this algorithm number and alphabet size into DataFrame
        df = pd.read_csv(f"{csv_dir}/averageTimes-{algo_num}-{size}.csv")

        # Add line plot for this alphabet size to subplot
        fig2.add_scatter(
            x=df["word_length"],
            y=df["average_time"],
            mode="lines+markers",
            name=f"size={size}",
            legendgroup=f"algo={algo_num}",
            showlegend=size == alphabet_sizes[0],
            xaxis=f"x{subplot}",
            yaxis=f"y{subplot}",
        )
        fig2.update_xaxes(title_text="Longueur du mot")
        fig2.update_yaxes(title_text="Temps moyen (ms)")
        fig2.update_yaxes(range=[0, 0.7])

# Update layout
fig2.update_layout(
    title="Average time to generate a random string",
    height=1000,
    width=1000,
    legend_title="Alphabet size",
)

# Show figure
fig2.show()