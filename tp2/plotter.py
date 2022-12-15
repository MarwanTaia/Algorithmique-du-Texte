import os

import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots


# Répertoire contenant les fichiers CSV
csv_dir = "output"

subplot_titles_list = [
    "Naïf",
    "Naïf, boucle rapide",
    "Naïf, boucle rapide, sentinelle",
    "Naïf, strncmp",
    "Naïf, boucle rapide, strncmp",
    "Naïf, boucle rapide, sentinelle, strncmp",
    "MP",
    "KMP",
    "Boyer-Moore",
    "Horspool"
]

# Mapper depuis le numéro de l'algorithme à la position du subplot
subplot_map = {}
for i in range(1, len(subplot_titles_list) + 1):
    subplot_map[i] = i

# Tailles de l'alphabet à tester
alphabet_sizes = [2, 4, 20, 70]

# Créer un subplot
fig = make_subplots(
    rows=4,
    cols=3,
    subplot_titles=subplot_titles_list,
    # x_title="Alphabet size",
    # y_title="Average time (s)",
)

# Itérer sur les numéros d'algorithme
for algo_num in range(1, len(subplot_titles_list) + 1):
    # Créer un subplot pour ce numéro d'algorithme
    subplot = subplot_map[int(algo_num)]

    # Itérer sur les tailles de l'alphabet
    for size in alphabet_sizes:
        # Lire le fichier CSV pour ce numéro d'algorithme et cette taille d'alphabet dans un DataFrame
        df = pd.read_csv(f"{csv_dir}/averageTimes-{algo_num}-{size}.csv")

        # Ajouter un graphique pour cette taille d'alphabet au subplot
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

# Créer un subplot
fig2 = make_subplots(
    rows=3,
    cols=3,
    subplot_titles=subplot_titles_list,
    # x_title="Alphabet size",
    # y_title="Average time (s)",
)

# Itérer sur les numéros d'algorithme
for algo_num in range(1, len(subplot_titles_list)):
    # Créer un subplot pour ce numéro d'algorithme
    subplot = subplot_map[int(algo_num)]

    # Itérer sur les tailles de l'alphabet
    for size in alphabet_sizes:
        # Lire le fichier CSV pour ce numéro d'algorithme et cette taille d'alphabet dans un DataFrame
        df = pd.read_csv(f"{csv_dir}/averageTimes-{algo_num}-{size}.csv")

        # Ajouter un graphique pour cette taille d'alphabet au subplot
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
        if algo_num <= 6:
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