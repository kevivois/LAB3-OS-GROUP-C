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
    avg = round(np.mean(values),3)
    stddev = round(np.std(values),3)
    median = round(np.percentile(values, 50),3)
    perc_90 = round(np.percentile(values, 90),3)
    return avg, stddev, median, perc_90

def total_context_switching(data):
    return sum(d['preemptions'] for d in data)

def total_runtime(data):
    return max(d['turnaround_time'] for d in data)

def export_to_csv(filename, stats):
    """Exports the computed statistics to a CSV file"""
    with open(filename, 'w', newline='') as file:
        writer = csv.writer(file,delimiter=';')

        writer.writerow(["Metric", "Average", "Standard Deviation", "50% Percentile (Median)", "90% Percentile"])
        
        writer.writerow(["Waiting Time"] + stats['waiting_time'])
        
        writer.writerow(["Turnaround Time"] + stats['turnaround_time'])
        
        writer.writerow(["Total Context Switches", stats['total_preemptions'], "", "", ""])
        
        writer.writerow(["Total Runtime (Throughput)", stats['total_runtime'], "", "", ""])

def main():
    # Read data from execution.csv
    algos = ["PR","FCFS","SRTF"]
    for i in range(0,len(algos)):
        data = read_csv(f'kevivois/output{algos[i]}.csv')
        
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
        
        export_to_csv(f'out/stats_output_{algos[i]}.csv', stats)

if __name__ == "__main__":
    main()
