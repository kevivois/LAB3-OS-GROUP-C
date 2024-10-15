
# Scheduling Algorithm Simulator 

This project simulates various CPU scheduling algorithms and generates statistics and visualizations based on their performance. The code consists of three main components:

1. **Generator**: `generator.py` generates random data for the scheduling simulation.
2. **Statistics Processor**: `stats.py` processes the generated data, calculates statistics, and creates visualizations.
3. **CSV Reader**: Used for reading input data and exporting results.

## Code Structure

The project contains the following files:

- `generator.py`: Generates random process data and exports it to CSV files.
- `stats.py`: Reads the generated CSV files, calculates statistics, and creates visualizations.
- `execution.csv`: Sample CSV input file (you may generate your own data using `generator.py`).
- `output_graphs/`: Directory where all generated graphs and statistics will be saved.

## How to Set Up

1. **Install Dependencies**: Ensure you have Python installed (version 3.6 or later). Then, install the required libraries:

   ```bash
   pip install numpy scipy matplotlib
   ```

2. **Generate Input Data**: Run `generator.py` to generate random data for different process sizes. The data will be saved in the `inputs` directory.

   ```bash
   python generator.py
   ```

3. **Run Statistics and Graph Generation**: After generating the data, run `stats.py` to read the CSV files, calculate statistics, and generate graphs.

   ```bash
   python stats.py
   ```

## What Works

- **Data Generation**: The `generator.py` file successfully generates random data including:
  - Process ID
  - Arrival Time
  - Execution Time
  - Random Priority

- **Statistics Calculation**: The `stats.py` file calculates:
  - Average waiting time
  - Standard deviation of waiting time
  - Median and 90th percentile of waiting time
  - Total context switches and total runtime

- **Graph Generation**: Various visualizations are generated:
  - Box plot for waiting time vs turnaround time
  - Histogram for waiting time distribution
  - Bar plots for context switches per process
  - Grouped bar plots for waiting time and turnaround time comparisons
  - Comparison graphs for different scheduling algorithms across various process sizes

## What Doesn't Work

- The project does not include a direct implementation of CPU scheduling algorithms; it requires pre-generated data from `generator.py`.
- Error handling is minimal; if the CSV format does not match expectations, the code may raise errors.

## Output

The generated graphs will be saved in the `output_graphs` directory, and the statistics will be exported to CSV files named according to the scheduling algorithms used and the number of processes.

### Example Output Structure

```
output_graphs/
    ├── graph_1_Scheduler.png
    ├── graph_2_Scheduler.png
    ├── graph_3_Scheduler.png
    ├── graph_4_Scheduler.png
    └── scheduler_stats.csv
```

## Example Usage

Generate data for 100 processes:

```bash
python generator.py
```

Process and visualize the data:

```bash
python stats.py
```

## Contributions

Feel free to contribute by submitting issues or pull requests to enhance the functionality or improve the codebase.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.
