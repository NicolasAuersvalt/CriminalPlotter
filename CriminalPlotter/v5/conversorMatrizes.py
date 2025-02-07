from PIL import Image
import numpy as np

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
        print(f"Erro ao processar a imagem: {e}")

# Exemplo de uso
input_image_path = "imagem.png"  # Substitua pelo caminho da sua imagem
output_text_path = "rosto100.txt"  # Substitua pelo nome desejado do arquivo de saída
png_to_binary_matrix(input_image_path, output_text_path)

