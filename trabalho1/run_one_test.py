import os
#from colorama import Fore, Back, Style

tests_folders = ['HashiApp_Puzzles/Intro1/', 
                 'HashiApp_Puzzles/Intro2/',
                 'Hashi_Puzzles/100/',
                 'Hashi_Puzzles/200/',
                 'Hashi_Puzzles/300/',
                 'Hashi_Puzzles/400/']

programs = ['hashi', 'solution_checker']
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

print(f'Escolher o teste (entre {1} e {len(os.listdir(tests_folders[0]))}): ')
choice = int(input())

for folder in tests_folders:
    print(f'TESTE: {os.path.dirname(folder)}')
    for i in range(choice, choice+1):
        input_file = f'{folder}level{i}.txt'
        output_file = f'out/{folder}out{i}.txt'
        
        output_directory = os.path.dirname(output_file)
        if not os.path.exists(output_directory):
            os.makedirs(output_directory)
        
        os.system(f'./hashi < {input_file} > {output_file}')
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