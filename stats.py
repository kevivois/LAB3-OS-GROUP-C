import csv
import numpy as np
import matplotlib.pyplot as plt
import os

colors = {
    'waiting_time': 'cornflowerblue',
    'turnaround_time': 'lightcoral',
    'context_switches': 'orange',
    'preemptions': 'mediumseagreen',
}

def read_csv(filename):
    data = []
    with open(filename, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            process_id = int(row[0])
            turnaround_time = int(row[1])
            waiting_time = int(row[2])
            preemptions = int(row[3])
            data.append({
                'id': process_id,
                'turnaround_time': turnaround_time,
                'waiting_time': waiting_time,
                'preemptions': preemptions
            })
    return data

def calculate_statistics(data, key):
    values = np.array([d[key] for d in data])
    avg = round(np.mean(values), 3)
    stddev = round(np.std(values), 3)
    median = round(np.percentile(values, 50), 3)
    perc_90 = round(np.percentile(values, 90), 3)
    return avg, stddev, median, perc_90

def total_context_switching(data):
    return sum(d['preemptions'] for d in data)

def total_runtime(data):
    return max(d['turnaround_time'] for d in data)

def box_plot(folder, title, data):
    waiting_times = [d['waiting_time'] for d in data]
    turnaround_times = [d['turnaround_time'] for d in data]

    plt.figure(figsize=(10, 6))
    plt.boxplot([waiting_times, turnaround_times],
                tick_labels=['Waiting Time', 'Turnaround Time'],
                patch_artist=True,
                boxprops=dict(facecolor='lightblue', edgecolor='blue'),
                medianprops=dict(color='red'))

    plt.title(f'Box Plot: Waiting Time vs Turnaround Time [{title}]', fontsize=16, fontweight='bold')
    plt.ylabel('Time (ms)', fontsize=14)
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f"{folder}/graph_1_{title}.png", format='png', dpi=300)
    plt.close()

def waiting_time_histogram(folder, title, data):
    waiting_times = [d['waiting_time'] for d in data]

    plt.figure(figsize=(10, 6))
    plt.hist(waiting_times, bins=10, color=colors['waiting_time'], alpha=0.75, edgecolor='black')
    plt.axvline(np.mean(waiting_times), color='red', linestyle='dashed', linewidth=1.5,
                label=f'Mean: {round(np.mean(waiting_times), 2)}')
    plt.axvline(np.median(waiting_times), color='green', linestyle='dotted', linewidth=1.5,
                label=f'Median: {np.median(waiting_times)}')

    plt.title(f'Histogram: Waiting Times Distribution [{title}]', fontsize=16, fontweight='bold')
    plt.xlabel('Waiting Time (ms)', fontsize=14)
    plt.ylabel('Number of Processes', fontsize=14)
    plt.legend(loc='upper right')
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f"{folder}/graph_2_{title}.png", format='png', dpi=300)
    plt.close()

def context_switch_barplot(folder, title, data):
    process_ids = [d['id'] for d in data]
    preemptions = [d['preemptions'] for d in data]

    plt.figure(figsize=(10, 6))
    bars = plt.bar(process_ids, preemptions, color=colors['context_switches'], alpha=0.75, edgecolor='black')

    for bar in bars:
        yval = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2, yval + 0.2, int(yval), ha='center', va='bottom', fontsize=10)

    plt.title(f'Context Switches per Process [{title}]', fontsize=16, fontweight='bold')
    plt.xlabel('Process ID', fontsize=14)
    plt.ylabel('Number of Context Switches', fontsize=14)
    plt.xticks(rotation=45)
    plt.grid(axis='y')
    plt.tight_layout()
    plt.savefig(f"{folder}/graph_3_{title}.png", format='png', dpi=300)
    plt.close()

def grouped_bar_plot(folder, title, data):
    process_ids = [d['id'] for d in data]
    waiting_times = [d['waiting_time'] for d in data]
    turnaround_times = [d['turnaround_time'] for d in data]

    x = np.arange(len(process_ids))
    width = 0.35

    plt.figure(figsize=(12, 6))
    plt.bar(x - width/2, waiting_times, width, label='Waiting Time', color=colors['waiting_time'])
    plt.bar(x + width/2, turnaround_times, width, label='Turnaround Time', color=colors['turnaround_time'])

    plt.xlabel('Process ID', fontsize=14)
    plt.ylabel('Time (ms)', fontsize=14)
    plt.title(f'Waiting Time vs Turnaround Time [{title}]', fontsize=16, fontweight='bold')
    plt.xticks(x, process_ids)
    plt.legend(loc='upper left')
    plt.grid(axis='y')
    plt.tight_layout()
    plt.savefig(f"{folder}/graph_4_{title}.png", format='png', dpi=300)
    plt.close()

def export_to_csv(filename, stats, algo_name):
    """Exports the computed statistics to a CSV file with improved clarity."""
    with open(filename, 'w', newline='') as file:
        writer = csv.writer(file, delimiter=';')
        writer.writerow([f"Scheduler Performance Metrics for {algo_name}"])
        writer.writerow(["The following metrics provide insights into the performance of the scheduling algorithm."])
        writer.writerow([])
        writer.writerow(["Metric", "Average", "Standard Deviation", "50% Percentile (Median)", "90% Percentile"])
        writer.writerow([
            "Waiting Time (ms)",
            stats['waiting_time'][0],
            stats['waiting_time'][1],
            stats['waiting_time'][2],
            stats['waiting_time'][3]
        ])
        writer.writerow([
            "Turnaround Time (ms)",
            stats['turnaround_time'][0],
            stats['turnaround_time'][1],
            stats['turnaround_time'][2],
            stats['turnaround_time'][3]
        ])
        writer.writerow([
            "Total Context Switches",
            stats['total_preemptions'],
            "", "", ""  # No stddev, percentiles for this metric
        ])
        writer.writerow([
            "Total Runtime (ms)",
            stats['total_runtime'],
            "", "", ""  # No stddev, percentiles for this metric
        ])

### New function: Compare Algorithms across Sizes ###
def compare_algorithms_graphs(folder, algos, sizes, stats_key):
    plt.figure(figsize=(10, 6))

    # Loop over all algorithms and collect their average and std deviation for the given sizes
    for algo in algos:
        avg_values = []
        stddev_values = []
        for size in sizes:
            data = read_csv(f'cmake-build-debug/outputs/{size}/output_{size}_{algo}.csv')
            avg, stddev, _, _ = calculate_statistics(data, stats_key)
            avg_values.append(avg)
            stddev_values.append(stddev)

        # Plot the average values with error bars
        plt.errorbar(sizes, avg_values, yerr=stddev_values, fmt='-o', capsize=5, label=f'{algo}',
                     color=np.random.rand(3,))

    plt.xlabel('Process Size', fontsize=14)
    plt.ylabel(f'{stats_key.capitalize()} (ms)', fontsize=14)
    plt.title(f'{stats_key.capitalize()} Comparison Across Sizes', fontsize=16, fontweight='bold')
    plt.legend(title="Algorithms", fontsize=12)
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(f"{folder}/comparison_{stats_key}_across_sizes.png", format='png', dpi=300)
    plt.close()
def main():
    algos = ["SRTF","PR","RR","FCFS"]
    amounts = [10,50,100,500,1000]


# For each size and algorithm, generate statistics and graphs
    for y in amounts:
        for i in range(len(algos)):
            data = read_csv(f'cmake-build-debug/outputs/{y}/output_{y}_{algos[i]}.csv')
            avg_waiting_time, stddev_waiting_time, median_waiting_time, perc90_waiting_time = calculate_statistics(data, 'waiting_time')
            avg_turnaround_time, stddev_turnaround_time, median_turnaround_time, perc90_turnaround_time = calculate_statistics(data, 'turnaround_time')
            total_preemptions = total_context_switching(data)
            total_runtime_val = total_runtime(data)

            stats = {
                'waiting_time': [avg_waiting_time, stddev_waiting_time, median_waiting_time, perc90_waiting_time],
                'turnaround_time': [avg_turnaround_time, stddev_turnaround_time, median_turnaround_time, perc90_turnaround_time],
                'total_preemptions': total_preemptions,
                'total_runtime': total_runtime_val
            }

            if not os.path.exists('out'):
                os.makedirs('out')
            if not os.path.exists(f'out/{y}'):
                os.makedirs(f'out/{y}')
            if not os.path.exists(f"out/{y}/{algos[i]}"):
                os.makedirs(f"out/{y}/{algos[i]}")
            box_plot(f'out/{y}/{algos[i]}',algos[i], data)
            waiting_time_histogram(f'out/{y}/{algos[i]}',algos[i], data)
            context_switch_barplot(f'out/{y}/{algos[i]}',algos[i], data)
            grouped_bar_plot(f'out/{y}/{algos[i]}',algos[i], data)
            export_to_csv(f'out/{y}/{algos[i]}/stats_output_{y}_{algos[i]}.csv', stats, algos[i])

    if not os.path.exists(f'out/{y}/comparison'):
        os.makedirs(f'out/{y}/comparison')
    compare_algorithms_graphs(f'out/{y}/comparison', ['FCFS', 'RR', 'PR', 'SRTF'], [10, 50, 100], 'waiting_time')

if __name__ == "__main__":
    main()
