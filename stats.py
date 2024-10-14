import csv
import numpy as np

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
    avg = np.mean(values)
    stddev = np.std(values)
    median = np.percentile(values, 50)
    perc_90 = np.percentile(values, 90)
    return avg, stddev, median, perc_90

def total_context_switching(data):
    return sum(d['preemptions'] for d in data)

def total_runtime(data):
    return max(d['turnaround_time'] for d in data)

def main():
    # Read data from execution.csv
    algorythms = ["FCFS","PR","SRTF"]
    data = read_csv('cmake-build-debug/OUTPUTRR.csv')

    # Calculate statistics for waiting times
    avg_waiting_time, stddev_waiting_time, median_waiting_time, perc90_waiting_time = calculate_statistics(data, 'waiting_time')

    # Calculate statistics for turnaround times
    avg_turnaround_time, stddev_turnaround_time, median_turnaround_time, perc90_turnaround_time = calculate_statistics(data, 'turnaround_time')

    # Total context switches
    total_preemptions = total_context_switching(data)

    # Total runtime (throughput)
    total_runtime_val = total_runtime(data)

    # Print out the results
    print("Waiting Time:")
    print(f"  Average: {avg_waiting_time:.2f}")
    print(f"  Standard Deviation: {stddev_waiting_time:.2f}")
    print(f"  50% Percentile (Median): {median_waiting_time:.2f}")
    print(f"  90% Percentile: {perc90_waiting_time:.2f}")

    print("\nTurnaround Time:")
    print(f"  Average: {avg_turnaround_time:.2f}")
    print(f"  Standard Deviation: {stddev_turnaround_time:.2f}")
    print(f"  50% Percentile (Median): {median_turnaround_time:.2f}")
    print(f"  90% Percentile: {perc90_turnaround_time:.2f}")

    print("\nTotal Context Switches:")
    print(f"  {total_preemptions}")

    print("\nTotal Runtime (Throughput):")
    print(f"  {total_runtime_val}")

if __name__ == "__main__":
    main()
