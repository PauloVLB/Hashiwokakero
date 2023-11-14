# Hashiwokakero
Desenvolvimento de heurísticas, algoritmos exatos e meta-heurísticas para o problema NP-Completo associado ao puzzle Hashiwokakero.

## Como usar:

Para executar o trabalho 1, sobre heurísticas, execute os passo abaixo dentro da pasta ```trabalho1```. 

Para executar o trabalho 2, sobre algoritmos exatos, execute os passo abaixo dentro da pasta ```trabalho2```. 

Primeiramente será necessário ter as seguintes dependências instaladas:

- Programa `make` (normalmente já instalado em sistemas Linux);
- Compilador `g++`;
- `python3`.


O trabalho possui dois principais bancos de instâncias, localizados nas pastas ```Hashi_Puzzles``` e ```HashiApp_Puzzles```.
Ao executar 

```
python3 run_tests.py
```

O programa irá rodar todos os testes presentes no conjunto de testes escolhido pelo usuário, mostrando a solução encontrada pela heurística e escrevendo nos arquivos correspondentes na pasta `out/`.

Já executando 
```
python3 run_one_test.py
```

O programa irá perguntar ao usuário qual banco de testes usar e, logo em seguida, o número do teste específico que deseja executar. Assim, mostrará a solução encontrada e escreverá a saída também em `out/`.

As avaliações de "ACCEPTED" e "WRONG ANSWER" são feitas com o programa `solution_chekcer.cpp`.

