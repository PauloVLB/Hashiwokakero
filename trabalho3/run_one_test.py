import os
import subprocess
import signal
import time
#from colorama import Fore, Back, Style

def run_command(command, input_file, output_file, timeout):
    try:
        with open(input_file, 'r') as infile, open(output_file, 'w') as outfile:
            # Inicia o processo
            process = subprocess.Popen(command, stdin=infile, stdout=outfile, preexec_fn=os.setsid)

            # Aguarda até que o processo termine ou o tempo limite seja atingido
            start_time = time.time()
            while time.time() - start_time < timeout and process.poll() is None:
                time.sleep(0.1)

            # Se o processo ainda estiver em execução, envie um sinal para interrompê-lo
            if process.poll() is None:
                os.killpg(os.getpgid(process.pid), signal.SIGTERM)
                process.wait()
                return False  # Indica que houve timeout
            else:
                return True  # Indica que o processo terminou dentro do tempo limite

    except Exception as e:
        print(f"Erro: {e}")
        return False  # Indica que ocorreu um erro


#tests_folders = ['HashiApp_Puzzles/Intro1/', 
#                 'HashiApp_Puzzles/Intro2/', 
#                 'HashiApp_Puzzles/Easy/',
#                 'Hashi_Puzzles/100/',
#                 'Hashi_Puzzles/200/',
#                 'Hashi_Puzzles/300/',
#                 'Hashi_Puzzles/400/']

pasta_raiz = '../TestCases/'

tests_folders = []
for pasta_atual, subpastas, arquivos in sorted(os.walk(pasta_raiz)):
    if(pasta_atual.count('/') == 3):
        tests_folders.append(pasta_atual.replace(pasta_raiz, "") + "/")
        #print(f'Pasta Atual: {pasta_atual.replace(pasta_raiz, "")}')

programs = ['ants', 'solution_checker']
print("Compilando programas: ")
for p in programs:
    os.system(f'make {p}')
print()
AC = 0
WA = 0

print("Escolha o conjunto de testes: ")
for i in range(1, len(tests_folders)+1):
    print(f'{i} - {tests_folders[i-1]}')

choice = int(input())
tests_folders = [tests_folders[choice-1]]

print(f'Escolher o teste (entre {1} e {len(os.listdir(pasta_raiz + tests_folders[0]))}): ')
choice = int(input())

for folder in tests_folders:
    print(f'TESTE: {os.path.dirname(folder)}')
    for i in range(choice, choice+1):
        input_file = f'../TestCases/{folder}level{i}.txt'
        output_file = f'out/{folder}out{i}.txt'
        
        output_directory = os.path.dirname(output_file)
        if not os.path.exists(output_directory):
            os.makedirs(output_directory)
        

        #os.system(f'./ants < {input_file} > {output_file}')
        command = ['./ants']
        timeout = 1800
        result = run_command(command, input_file, output_file, timeout)
        
        if not result:
            print("TLE")
            exit()

        output = os.system(f'./solution_checker < {output_file}')

        print(f'Teste {i} {"-"*21}')

        with open(output_file, 'r') as file:
            file_content = file.read()
            blocks = file_content.strip().split('\n\n')

            if len(blocks) > 0:
                last_block = blocks[-1]
                print(last_block)
            else:
                print("No board found in the file.")


        print('Status: ', end="")
        if(output == 0):
            print('ACCEPTED')
            AC = AC + 1
        else:
            print('WRONG ANSWER')
            WA = WA + 1
        
        print("-"*30)