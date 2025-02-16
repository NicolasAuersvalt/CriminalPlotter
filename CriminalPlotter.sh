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

# Verificar se o Python está instalado
check_python

# Executar os scripts Python
python3 src/somadorMatrizes.py
