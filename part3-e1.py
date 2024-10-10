import matplotlib.pyplot as plt
import numpy as np

# Function to read bandwidth data from a file and return a list of bandwidths
def read_bandwidth_data(file_path):
    bandwidths = []
    with open(file_path, 'r') as file:
        for line in file:
            # Extract the bandwidth value from each line
            parts = line.strip().split()
            if len(parts) >= 2 and parts[0] == 'bandwidth:':
                bandwidths.append(float(parts[1]))  # Convert the bandwidth to float
    return bandwidths

# Function to plot histogram of the bandwidths and print bin heights
def plot_bandwidth_histogram(bandwidths, bin_width):
    min_bandwidth = min(bandwidths)
    max_bandwidth = max(bandwidths)
    
    # Calculate the number of bins based on the specified bin width
    bins = np.arange(min_bandwidth, max_bandwidth + bin_width, bin_width)
    
    plt.figure(figsize=(10, 6))
    
    # Plot the histogram and get the values of bins
    counts, edges, patches = plt.hist(bandwidths, bins=bins, edgecolor='black', alpha=0.7)
    
    # Loop through each bin and add a label with the bin height on top
    for count, edge in zip(counts, edges):
        # Place the label at the center of the bin
        plt.text(edge + bin_width / 2, count, str(int(count)), 
                 ha='center', va='bottom', fontsize=10)
    
    plt.title('Histogram of Bandwidth Data')
    plt.xlabel('Bandwidth')
    plt.ylabel('Frequency')
    plt.grid(axis='y', alpha=0.75)
    plt.savefig('bandwidth_histogram.png')

# Main execution
if __name__ == "__main__":
    file_path = 'part3-e1-buffer'  # Replace with your actual file path
    bandwidths = read_bandwidth_data(file_path)
    bin_width = 0.25  # Set your desired bin width here
    plot_bandwidth_histogram(bandwidths, bin_width)
