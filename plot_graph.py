import matplotlib.pyplot as plt
import numpy as np

# Function to read data from values file
def read_data(filename):
    with open(filename, "r") as file:
        lines = file.readlines()[1:]  # Skip the header

    text_len = []
    comparisons = []
    time_taken = []

    for line in lines:
        _, t_len, cmp, time_ns = map(int, line.strip().split(","))
        text_len.append(t_len)
        comparisons.append(cmp)
        time_taken.append(time_ns)

    return np.array(text_len), np.array(comparisons), np.array(time_taken)

# Read data from both files
text_h, cmp_h, time_h = read_data("horspool_values.txt")
text_bm, cmp_bm, time_bm = read_data("boyermoore_values.txt")

# Plot Text Length vs Execution Time Graph
plt.figure(figsize=(10, 5))
plt.plot(text_h, time_h, 'ro-', label="Horspool Execution Time")
plt.plot(text_bm, time_bm, 'bo-', label="Boyer-Moore Execution Time")
plt.xlabel("Text Length")
plt.ylabel("Execution Time (ns)")
plt.title("Text Length vs Execution Time")
plt.legend()
plt.grid(True)
plt.show()

# Plot Text Length vs Number of Comparisons Graph
plt.figure(figsize=(10, 5))
plt.plot(text_h, cmp_h, 'r*-', label="Horspool Comparisons")
plt.plot(text_bm, cmp_bm, 'b*-', label="Boyer-Moore Comparisons")
plt.xlabel("Text Length")
plt.ylabel("Number of Comparisons")
plt.title("Text Length vs Number of Comparisons")
plt.legend()
plt.grid(True)
plt.show()
