import os
import pandas as pd
import matplotlib.pyplot as plt

# Cartella principale dei risultati e dei plot
results_folder = 'Results'
plots_folder = 'Plots'

# Assicurati che la cartella di output esista
os.makedirs(plots_folder, exist_ok=True)

# Itera sulla cartella principale dei risultati
for root, dirs, files in os.walk(results_folder):
    for dir_name in dirs:
        if 'OMP_CUDA' in dir_name:
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

                    # Filter data for OMP+CUDA and OMP+CUDA_L1 separately
                    df_cuda = df[df['Modality'] == 'OMP+CUDA']
                    df_cuda_l1 = df[df['Modality'] == 'OMP+CUDA_L1']

                    plot_title = os.path.splitext(csv_file)[0]

                    # Inizializza il plot con risoluzione più alta
                    plt.figure(figsize=(12, 8), dpi=300)

                    # Plotting
                    # Plot OMP+CUDA speedup curve
                    plt.plot(df_cuda['OMP'], df_cuda['speedup'], label='OMP+CUDA', marker='o')

                    # Plot OMP+CUDA_L1 speedup curve
                    plt.plot(df_cuda_l1['OMP'], df_cuda_l1['speedup'], label='OMP+CUDA_L1', marker='o')

                    # Impostazioni del plot
                    plt.title(f'{plot_title} characters', fontsize=24)  # Imposta il font size a 24
                    # Set axis labels and title
                    plt.xlabel('OMP', fontsize=14)
                    plt.ylabel('Speedup', fontsize=14)

                    # Add legend in the top-left corner
                    plt.legend(loc='upper left', fontsize=12)


                    # Imposta i limiti dell'asse y tra 0 e 2
                    plt.ylim(0, 3)

                    # Display the plot
                    plt.grid(True)

                    # Salva l'immagine nella cartella di output con formato jpg
                    output_folder = os.path.join(plots_folder, dir_name, opt_folder)
                    os.makedirs(output_folder, exist_ok=True)
                    output_file_path = os.path.join(output_folder, f"{plot_title}.jpg")
                    plt.savefig(output_file_path, format='jpg')
                    plt.close()
