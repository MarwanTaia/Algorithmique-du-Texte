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
    subplot_titles=["Algorithm 1", "Algorithm 2", "Algorithm 3", "Algorithm 4", "Algorithm 5", "Algorithm 6", "Algorithm 7"],
    x_title="Alphabet size",
    y_title="Average time (s)",
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