from PIL import Image
import numpy as np
import os

def png_to_binary_matrix(input_image_path, output_text_path, threshold=128):
    """
    Converte uma imagem PNG em uma matriz binária e salva como arquivo de texto.
    
    Args:
        input_image_path (str): Caminho para a imagem PNG de entrada.
        output_text_path (str): Caminho para salvar o arquivo de texto de saída.
        threshold (int): Limiar para converter em binário (0-255). Default é 128.
    """
    try:
        # Abre a imagem e converte para tons de cinza
        image = Image.open(input_image_path).convert("L")
        
        # Converte a imagem para um array NumPy
        image_array = np.array(image)
        
        # Aplica o limiar para gerar a matriz binária
        binary_matrix = (image_array >= threshold).astype(int)
        
        # Salva a matriz binária em um arquivo .txt
        np.savetxt(output_text_path, binary_matrix, fmt='%d', delimiter='')
        
        print(f"Matriz binária salva em: {output_text_path}")
    except Exception as e:
        print(f"Erro ao processar a imagem {input_image_path}: {e}")

def process_all_images(images_root):
    """
    Percorre recursivamente todos os subdiretórios de images_root, processando imagens PNG.
    """
    for root, _, files in os.walk(images_root):
        for file in files:
            if file.lower().endswith(".png"):
                image_path = os.path.join(root, file)
                output_path = os.path.join(root, os.path.splitext(file)[0] + ".txt")
                png_to_binary_matrix(image_path, output_path)

# Exemplo de uso
images_root = "images"  # Substitua pelo caminho correto do diretório principal de imagens
process_all_images(images_root)
