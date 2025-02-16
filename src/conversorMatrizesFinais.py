from PIL import Image
import numpy as np
import os

def png_to_binary_matrix(input_image_path, output_directory, threshold=128):
    """
    Converte uma imagem PNG em uma matriz binária invertida e salva como arquivo de texto.
    
    Args:
        input_image_path (str): Caminho para a imagem PNG de entrada.
        output_directory (str): Diretório para salvar os arquivos de saída.
        threshold (int): Limiar para converter em binário (0-255). Default é 128.
    """
    try:
        # Obtém o nome do arquivo sem extensão
        output_file_name = os.path.splitext(os.path.basename(input_image_path))[0] + ".txt"
        output_text_path = os.path.join(output_directory, output_file_name)

        print(f"Processando imagem: {input_image_path}")
        
        # Abre a imagem e converte para tons de cinza
        image = Image.open(input_image_path).convert("L")
        
        # Converte a imagem para um array NumPy
        image_array = np.array(image)
        print(f"Dimensões da imagem: {image_array.shape}")
        
        # Aplica o limiar para gerar a matriz binária e inverte os valores
        binary_matrix = (image_array >= threshold).astype(int)
        inverted_matrix = 1 - binary_matrix
        
        # Salva a matriz binária invertida em um arquivo .txt
        np.savetxt(output_text_path, inverted_matrix, fmt='%d', delimiter='')
        
        print(f"Matriz binária invertida salva em: {output_text_path}")
    except Exception as e:
        print(f"Erro ao processar a imagem {input_image_path}: {e}")

def process_all_images(images_root, output_directory):
    """
    Percorre recursivamente todos os subdiretórios de images_root, processando imagens PNG e salvando em output_directory.
    """
    print(f"Iniciando processamento no diretório: {images_root}")
    print(f"Path completo de images_root: {os.path.abspath(images_root)}")
    
    if not os.path.exists(images_root):
        print(f"Diretório {images_root} não encontrado.")
        return
    
    # Garante que o diretório de saída existe
    os.makedirs(output_directory, exist_ok=True)
    
    for root, dirs, files in os.walk(images_root):
        print(f"Acessando diretório: {root}")
        print(f"Subdiretórios encontrados: {dirs}")
        
        for file in files:
            if file.lower().endswith(".png"):
                image_path = os.path.join(root, file)
                print(f"Encontrada imagem: {file}, convertendo...")
                png_to_binary_matrix(image_path, output_directory)

# Exemplo de uso
images_root = "impressao/bancoRetratos"   # Substitua pelo caminho correto do diretório principal de imagens
output_directory = "impressao/resultado"  # Diretório onde os arquivos serão salvos
process_all_images(images_root, output_directory)
