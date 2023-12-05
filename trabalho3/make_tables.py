import os

# Função para processar um arquivo
def processar_arquivo(nome_arquivo):
    with open(nome_arquivo, 'r') as arquivo:
        linhas = arquivo.readlines()

    primeira_linha = linhas[0].split()
    m = primeira_linha[0]
    n = primeira_linha[1]
    
    linha_ilhas = linhas[int(n)+1].split()
    qnt_ilhas = linha_ilhas[0]
    qnt_arestas = linha_ilhas[1]
    
    try:
        linha_tempo = linhas[int(n) + int(qnt_arestas) + 3].split()
        tempo = linha_tempo[0]
    except:
        tempo = -1
        
    return n, m, qnt_ilhas, tempo



tests_folders = ['HashiApp_Puzzles/Easy/']
                 #'HashiApp_Puzzles/Intro1/', 
                 #'HashiApp_Puzzles/Intro2/']
                 #'Hashi_Puzzles/100/',
                 #'Hashi_Puzzles/200/',
                 #'Hashi_Puzzles/300/',
                 #'Hashi_Puzzles/400/']

# Processar arquivos que correspondem ao padrão
for folder in tests_folders:
    qnt_arquivos = len(os.listdir('out/' + folder))

    for i in range(1, qnt_arquivos+1):
        caminho_completo = f'out/{folder}out{i}.txt'
        n, m, qnt_ilhas, tempo = processar_arquivo(caminho_completo)

        if tempo == -1:
            print(f'{n}x{m} & {qnt_ilhas} & --- & Tempo Excedido \\\\')
        else:
            print(f'{n}x{m} & {qnt_ilhas} & {tempo} & Resposta Correta \\\\')
    print()
