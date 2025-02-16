import os
import json

def load_or_initialize_metadata(output_directory):
    """
    Carrega o arquivo JSON consolidado, ou inicializa um novo se não existir.
    
    Args:
        output_directory (str): Diretório onde o arquivo JSON será salvo.
    
    Returns:
        dict: Dicionário contendo os dados do metadata.
    """
    metadata_path = os.path.join(output_directory, "matrices_metadata.json")
    if os.path.exists(metadata_path):
        with open(metadata_path, "r") as f:
            print(f"Carregando metadata consolidado existente de: {metadata_path}")
            return json.load(f)
    else:
        print(f"Nenhum metadata consolidado encontrado. Criando um novo em: {metadata_path}")
        return {}

def save_metadata(output_directory, metadata):
    """
    Salva os dados de metadata consolidado em um arquivo JSON.
    
    Args:
        output_directory (str): Diretório para salvar o arquivo JSON.
        metadata (dict): Dados a serem salvos.
    """
    metadata_path = os.path.join(output_directory, "matrizes_metadata.json")
    with open(metadata_path, "w") as f:
        json.dump(metadata, f, indent=4)
    print(f"Metadata consolidado salvo em: {metadata_path}")

def process_matrix_names(directory_path, output_directory):
    """
    Lê os nomes das matrizes (arquivos .txt) em um diretório e atualiza o JSON consolidado.
    
    Args:
        directory_path (str): Diretório contendo os arquivos .txt.
        output_directory (str): Diretório para salvar o JSON consolidado.
    """
    print(f"Processando diretório: {directory_path}")
    
    if not os.path.exists(directory_path):
        print(f"Diretório {directory_path} não encontrado.")
        return
    
    # Garante que o diretório de saída existe
    os.makedirs(output_directory, exist_ok=True)

    # Carrega ou inicializa o metadata consolidado
    matrices_metadata = load_or_initialize_metadata(output_directory)

    # Processa os arquivos no diretório principal
    for file in os.listdir(directory_path):
        if file.lower().endswith(".txt"):
            # Obtém o nome do arquivo (sem extensão)
            matrix_name = os.path.splitext(file)[0]
            print(f"Encontrada matriz: {matrix_name}")
            
            # Atualiza a contagem no metadata
            if matrix_name in matrices_metadata:
                matrices_metadata[matrix_name] += 1
            else:
                matrices_metadata[matrix_name] = 1

    # Salva o metadata consolidado no final do processamento
    save_metadata(output_directory, matrices_metadata)

# Exemplo de uso
directory_path = "impressao/matrizes"  # Diretório contendo os arquivos .txt das matrizes
output_directory = "impressao"  # Diretório para salvar o JSON consolidado
process_matrix_names(directory_path, output_directory)
