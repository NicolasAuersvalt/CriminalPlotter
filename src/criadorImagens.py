import os
import numpy as np
from PIL import Image

def matriz_para_png(caminho_arquivo, caminho_saida):
    matriz = []
    
    with open(caminho_arquivo, "r") as f:
        for linha in f:
            matriz.append([int(char) for char in linha.strip()])
    
    matriz = np.array(matriz, dtype=np.uint8) * 255  # Converte 1 -> 255 (branco) e 0 -> 0 (preto)
    matriz = 255 - matriz  # Inverte preto e branco
    imagem = Image.fromarray(matriz, mode='L')  # 'L' para escala de cinza
    imagem.save(caminho_saida)
    
    print(f"Imagem salva em {caminho_saida}")

# Exemplo de uso:
matriz_para_png("impressao/matriz_resultante.txt", "impressao/matriz_resultante.png")
