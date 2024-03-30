import os
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D

# Cartella principale dei risultati e dei plot
results_folder = 'Results'
plots_folder = 'Plots'

# Assicurati che la cartella di output esista
os.makedirs(plots_folder, exist_ok=True)

# Itera sulla cartella principale dei risultati
for root, dirs, files in os.walk(results_folder):
    for dir_name in dirs:
        if 'OMP_MPI' in dir_name:
            omp_mpi_folder = os.path.join(root, dir_name)

            # Itera su opt0, opt1, opt2, opt3
            for opt_folder in ['opt0', 'opt1', 'opt2', 'opt3']:
                opt_folder_path = os.path.join(omp_mpi_folder, opt_folder)

                # Lista di tutti i file .csv nella sottocartella opt
                csv_files = [f for f in os.listdir(opt_folder_path) if f.endswith('.csv')]

                for csv_file in csv_files:
                    # Leggi il file CSV
                    file_path = os.path.join(opt_folder_path, csv_file)
                    df = pd.read_csv(file_path, delimiter=';')

                    # Filtra i dati per ottenere le informazioni necessarie
                    omp_mpi_data = df[df['Modality'] == 'OMP+MPI']
                    omp_thread_counts = sorted(set(omp_mpi_data['OMP']))
                    mpi_process_counts = sorted(set(omp_mpi_data['MPI']))

                    # Ottieni il nome dal nome del CSV
                    plot_title = os.path.splitext(csv_file)[0]

                    # Inizializza il plot con risoluzione più alta
                    plt.figure(figsize=(12, 8), dpi=300)

                    # Creiamo un insieme per tenere traccia dei thread OpenMP già inclusi nella legenda
                    omp_legend_set = set()

                    # Disegna le linee per ogni numero di thread OpenMP
                    colors = ['blue','green', 'red', 'purple', 'orange']
                    legend_handles = []
                    for omp_threads, color in zip(omp_thread_counts, colors):
                        label = f'OMP={omp_threads}'  # Etichetta per il numero di thread OpenMP

                        if label not in omp_legend_set:
                            omp_legend_set.add(label)
                            line = Line2D([0], [0], marker='o', color=color, label=label)
                            legend_handles.append(line)
                        else:
                            label = "_nolegend_"  # Se l'etichetta è già stata utilizzata, evita di ripeterla nella legenda

                        for mpi_processes in mpi_process_counts:
                            subset = omp_mpi_data[
                                (omp_mpi_data['OMP'] == omp_threads) & (omp_mpi_data['MPI'] == mpi_processes)]
                            speedup_values = subset['speedup'].tolist()
                            plt.plot(subset['MPI'], speedup_values, marker='o', linestyle='-', color=color)

                    # Impostazioni del plot
                    plt.title(f'{plot_title} characters', fontsize=24)  # Imposta il font size a 24
                    plt.xlabel('Number of Processes (MPI)')
                    plt.ylabel('Speedup')
                    plt.xticks(mpi_process_counts)

                    # Imposta i limiti dell'asse y tra 0 e 6.5
                    plt.ylim(0, 6.5)

                    # Aggiungi la griglia
                    plt.grid(True)

                    # Lascia uno spazio in alto per la legenda
                    plt.subplots_adjust(top=0.9)

                    # Aggiungi la legenda
                    plt.legend(handles=legend_handles, loc='upper left')
                    for omp_threads, color in zip(omp_thread_counts, colors):
                        omp_subset = omp_mpi_data[omp_mpi_data['OMP'] == omp_threads]
                        omp_speedup_values = omp_subset.groupby('MPI')['speedup'].mean().tolist()
                        plt.plot(omp_subset.groupby('MPI')['MPI'].mean(), omp_speedup_values, marker='', linestyle='-',
                                 color=color)

                    # Salva l'immagine nella cartella di output con formato jpg
                    output_folder = os.path.join(plots_folder, dir_name, opt_folder)
                    os.makedirs(output_folder, exist_ok=True)
                    output_file_path = os.path.join(output_folder, f"{plot_title}.jpg")
                    plt.savefig(output_file_path, format='jpg')
                    plt.close()
