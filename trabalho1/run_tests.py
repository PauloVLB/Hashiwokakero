import os
from colorama import Fore, Back, Style

tests_folders = ['HashiApp_Puzzles/Intro1/', 'HashiApp_Puzzles/Intro2/']

programs = ['hashi', 'solution_checker']
print("Compilando programas: ")
for p in programs:
    os.system(f'make {p}')
print()

for folder in tests_folders:
    qnt_tests = len(os.listdir(folder))
    
    print(f'BATERIA DE TESTES: {os.path.dirname(folder)}')
    for i in range(1, qnt_tests+1):
        input_file = f'{folder}level{i}.txt'
        output_file = f'out/{folder}out{i}.txt'
        
        output_directory = os.path.dirname(output_file)
        if not os.path.exists(output_directory):
            os.makedirs(output_directory)
        
        os.system(f'./hashi < {input_file} > {output_file}')
        output = os.system(f'./solution_checker < {output_file}')

        print(Fore.BLACK + Back.BLUE + Style.BRIGHT + f'Teste {i} {"-"*21}', end="")
        print(Style.RESET_ALL)

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
            print(Fore.BLACK + Back.GREEN + Style.BRIGHT + 'ACCEPTED', end="")
            print(Style.RESET_ALL, end="")
            print()
        else:
            print(Fore.BLACK + Back.RED + Style.BRIGHT + 'WRONG ANSWER', end="")
            print(Style.RESET_ALL, end="")
            print()
        
        print(Fore.BLACK + Back.BLUE + Style.BRIGHT + "-"*30, end="")
        print(Style.RESET_ALL)