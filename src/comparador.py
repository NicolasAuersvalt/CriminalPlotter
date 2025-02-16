import os
import json

def filter_top_matrices_by_hundreds(input_file):
    """
    Lê o JSON consolidado, seleciona apenas as matrizes com as maiores contagens
    dentro de cada grupo de centenas (0XX, 1XX, ..., 6XX), e salva o resultado de volta.
    
    Args:
        input_file (str): Caminho para o arquivo JSON consolidado.
    """
    if not os.path.exists(input_file):
        print(f"Arquivo {input_file} não encontrado.")
        return

    # Carrega o JSON
    with open(input_file, "r") as f:
        matrices_metadata = json.load(f)
    
    print("Conteúdo original do arquivo:")
    print(json.dumps(matrices_metadata, indent=4))

    # Dicionário para armazenar o maior de cada centena
    top_matrices = {}

    for matrix_name, count in matrices_metadata.items():
        # Extrai a centena (primeiro dígito da matriz, como 0XX, 1XX, etc.)
        hundred = matrix_name[0]  # Primeiro caractere do nome da matriz
        count = int(count)  # Converte a contagem para inteiro
        
        # Verifica se já há um item dessa centena no dicionário
        if hundred not in top_matrices:
            top_matrices[hundred] = {"name": matrix_name, "count": count}
        else:
            # Substitui pelo nome com maior contagem
            if count > top_matrices[hundred]["count"]:
                top_matrices[hundred] = {"name": matrix_name, "count": count}

    # Cria um novo dicionário consolidado com os resultados filtrados
    filtered_metadata = {data["name"]: data["count"] for data in top_matrices.values()}

    print("Matrizes filtradas por centenas:")
    print(json.dumps(filtered_metadata, indent=4))

    # Salva o resultado de volta no mesmo arquivo
    with open(input_file, "w") as f:
        json.dump(filtered_metadata, f, indent=4)

    print(f"Matrizes filtradas salvas em: {input_file}")


# Exemplo de uso
input_file = "impressao/matrizes_metadata.json"  # Substitua pelo caminho correto do arquivo
filter_top_matrices_by_hundreds(input_file)
