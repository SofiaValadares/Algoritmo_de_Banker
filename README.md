English version below.

# Algoritimo de Banker

Algoritimo de Banker desenvolvido para a cadeira de Infraestrutura de Software na CESAR School lencionada por Erico Souza Teixeira. Foi usando como referencia o livro Operating System Concepts, Silberschatz, A. et al, 10a edição, porem a algumas diferencias:

1. O 'NUMBER_OF_CUSTOMERS' e o 'NUMBER_OF_RESOURCES' não são fixos, e serão determinados de acordo com a entrada do programa
2. O programa deve ser implementado em C e ser executável em sistemas Linux, Unix ou macOS, com a compilação feita por Makefile, através simplesmente do comando 'make' via terminal, retornando o arquivo executável com nome 'banker'
3. Cada argumento por linha de comando na chamada do programa representa o número de instância para cada tipo de recursos
<dt> 
  
    ./banker 10 5 7
    
</dt>

## Arquivos:
4. customer.txt: Arquivo de clientes com mesmo formato daquele apresentado no livro, porem com a diferença que o númeron de linhas representa o nímero de clientes. Ele pode ser modificado mas deve apresentar a mesma formatação e o número de recursos por cliente deve ser condisente com os passados por linha de comandos
5. commands.txt: No lugar de receber entradas do usuário esse arquivo é lido com os comandos a serem execultados pelo sistema, os comandos no arquivo devem seguir a mesma formatação daquela apresentada pelo livro. Ele pode ser modificado mas deve apresentar a mesma formatação. Ele pode ser modificado mas deve apresentar a mesma formatação e o número de recursos por cliente deve ser condisente com os passados por linha de comandos
6. result.txt: Como saída, o programa deverá produzir o arquivo 'result.txt'

## Compilando Arquivos:
 - Unix/Linux/macOs
<dt> 
  
    make
    
</dt>
- Windows
<dt> 
  
    gcc ./banker.c -o ./banker
    
</dt>


# Bankers Algorithm
Banker's Algorithm developed for the Software Infrastructure course at CESAR School, taught by Erico Souza Teixeira. It was referenced from the book "Operating System Concepts" by Silberschatz, A. et al, 10th edition, with some differences:

1. The 'NUMBER_OF_CUSTOMERS' and 'NUMBER_OF_RESOURCES' are not fixed; they will be determined according to the program input
2. The program must be implemented in C and executable on Linux, Unix, or macOS systems. Compilation should be done using a Makefile, with a simple 'make' command in the terminal, resulting in an executable file named 'banker'
3. Each command-line argument when calling the program represents the number of instances for each type of resource
<dt> 
  
    ./banker 10 5 7
    
</dt>

## Arquivos:
4. customer.txt: File of customers with the same format as presented in the book, but with the difference that the number of lines represents the number of customers. It can be modified but must maintain the same formatting, and the number of resources per customer should be consistent with those passed through the command line
5. commands.txt: Instead of receiving user inputs, this file is read with the commands to be executed by the system. The commands in the file should follow the same format as presented in the book. It can be modified but must maintain the same formatting. The number of resources per customer should be consistent with those passed through the command line
6. result.txt: As output, the program should produce the 'result.txt' file

## Compiling Files:
 - Unix/Linux/macOs
<dt> 
  
    make
    
</dt>
- Windows
<dt> 
  
    gcc ./banker.c -o ./banker
    
</dt>


