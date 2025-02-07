#!/bin/bash

# Função para verificar se o Python está instalado
check_python() {
    if command -v python3 &>/dev/null; then
        PYTHON=python3
    elif command -v python &>/dev/null; then
        PYTHON=python
    else
        echo "Python não encontrado. Por favor, instale o Python para continuar."
        exit 1
    fi
}

# Função para verificar se as bibliotecas necessárias estão instaladas
check_libraries() {
    REQUIRED_LIBS=("serial" "numpy" "PIL")
    for lib in "${REQUIRED_LIBS[@]}"; do
        if ! python -c "import $lib" &>/dev/null; then
            echo "Biblioteca $lib não encontrada. Instalando..."
            $PYTHON -m pip install "$lib"
        fi
    done
}

# Verificar se o Python está instalado
check_python

# Verificar se as bibliotecas estão instaladas
check_libraries

# Navegar para o diretório src
cd /src

# Executar os scripts Python
$PYTHON somadorMatrizes.py

# Criando as imagens
$PYTHON criadorImagens.py
