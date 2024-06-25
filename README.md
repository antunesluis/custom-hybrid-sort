# Compilação

Para compilar o projeto, execute o seguinte comando no terminal:

```sh
make
```

# Uso

### Gerar Números Aleatórios

Para gerar um arquivo com números aleatórios:

```sh
./bin/app generate <num_values> <max_value> <output_file>
```

- `<num_values>`: Número de valores aleatórios a serem gerados.
- `<max_value>`: Valor máximo dos números gerados.
- `<output_file>`: Caminho do arquivo onde os números gerados serão salvos.

### Ordenação Interna

Para ordenar números de um arquivo usando um algoritmo de ordenação interna:

```sh
./bin/app sort_internal <input_file> <output_file> <algorithm>
```

- `<input_file>`: Caminho do arquivo de entrada contendo os números.
- `<output_file>`: Caminho do arquivo onde os números ordenados serão salvos.
- `<algorithm>`: Algoritmo de ordenação a ser usado (merge_insertion, quick_insertion).

### Ordenação Externa

Para ordenar números de um arquivo usando um algoritmo de ordenação externa:

```sh
./bin/app sort_external <input_file> <output_file> <algorithm> <memory_size>
```

- `<input_file>`: Caminho do arquivo de entrada contendo os números.
- `<output_file>`: Caminho do arquivo onde os números ordenados serão salvos.
- `<algorithm>`: Algoritmo de ordenação a ser usado (external_merge_insertion, external_quick_insertion).
- `<memory_size>`: Tamanho da memória disponível para a ordenação externa (e.g., 100K, 200M).
