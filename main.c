#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 30

typedef struct{
    int dia,mes,ano;
}Data;

typedef struct{
    long int codigo;
    int idade,posicao;
    char nome[MAX];
    float salario;
    Data data;
}Cadastro;

void escrever(Cadastro *cadastro){
    char str[MAX];
    FILE *arq;
    printf("Digite o nome do arquivo a ser criado\n");
    scanf("%s", str);
    
    //abre leitura e escrita
        if((arq=fopen(str, "rb+"))==NULL){
            arq = fopen(str, "wb+");
            if(arq==NULL){
                printf("ERRO\n");
                exit(1);
            }
        }
    
    //ponteiro no fim
    fseek(arq, 0, SEEK_END);
    
    printf("Digite o codigo:\n");
    scanf("%ld", &(*cadastro).codigo);
    fflush(stdin);
    
    printf("Digite o nome:\n");
    fgets((*cadastro).nome, MAX, stdin);
    (*cadastro).nome[strcspn((*cadastro).nome, "\n")] = '\0';
    
    printf("Digite a idade:\n");
    scanf("%d", &(*cadastro).idade);
    
    printf("Digite o salario\n");
    scanf("%f", &(*cadastro).salario);
    
    printf("Digite a data de nascimento: dia/mes/ano\n");
    scanf("%d %d %d", &(*cadastro).data.dia, &(*cadastro).data.mes, &(*cadastro).data.ano);
    
    //guarda a posicao de cada struct
    (*cadastro).posicao ++;

    fwrite(cadastro, sizeof(Cadastro), 1, arq);
    fclose(arq);
    printf("Dados gravados\n");
}

void ler(Cadastro *cadastro){
    char str[MAX];
    int cont = 1;
    FILE *arq;
    printf("Digite o nome do arquivo a ser lido\n");
    scanf("%s", str);

        if((arq=fopen(str, "rb"))==NULL){
            printf("Erro ao abrir arquivo %s para modo leitura\n", str);
            exit(1);
        }

    while(fread(cadastro, sizeof(Cadastro), 1, arq)==1){
    printf("Registro de numero: %d\n", cont);
    printf("Codigo: %ld\n", (*cadastro).codigo);
    printf("Nome: %s\n", (*cadastro).nome);
    printf("Idade: %d\n", (*cadastro).idade);
    printf("Salario: %.2f\n", (*cadastro).salario);
    printf("Data de nascimento: %d/%d/%d\n", (*cadastro).data.dia, (*cadastro).data.mes, (*cadastro).data.ano);
    cont++;
    }

    fclose(arq);

}

void inserir(Cadastro *cadastro){
    int posicao = 0;
    char str[MAX];
    FILE *arq, *arq_aux;

    printf("Digite o nome do arquivo \n");
    scanf("%s", str);
    
    printf("Digite a posicao do cadastro para ser inserido:\n");
    scanf("%d", &posicao);


   if((arq=fopen(str, "rb"))==NULL){
            printf("Erro ao abrir arquivo %s para modo leitura\n", str);
            exit(1);
        }
    
        if((arq_aux=fopen("backupInsercao.bin", "wb"))==NULL){
            printf("Erro ao abrir arquivo backupInsercao.bin para modo leitura\n");
            exit(1);
        }
    //Ta gravando no final do arquivo:( e eh pra gravar na posicao que o usuario pede!!!
        while(fread(cadastro, sizeof(Cadastro), 1, arq) == 1){
        fwrite(cadastro, sizeof(Cadastro), 1, arq_aux);
            if((*cadastro).posicao == posicao) {
                escrever(cadastro); 
            }
            fwrite(cadastro, sizeof(Cadastro), 1, arq_aux);
        }
        escrever(cadastro);

    fclose(arq);
    fclose(arq_aux);

    rename("backupInsercao.bin", str);
}

void excluir(Cadastro *cadastro){
    int codigo_excluir, repet = 0;
    char str[MAX];
    char str_backup[MAX];
    char str_resp;
    FILE *arq, *arq_aux;

    printf("Digite o nome do arquivo \n");
    scanf("%s", str);
    
    printf("Digite o codigo do cadastro para ser excluido:\n");
    scanf("%d", &codigo_excluir);
    
        if((arq=fopen(str, "rb"))==NULL){
            printf("Erro ao abrir arquivo %s para modo leitura\n", str);
            exit(1);
        }
    
        if((arq_aux=fopen("backup.bin", "wb"))==NULL){
            printf("Erro ao abrir arquivo %s para modo leitura\n", str);
            exit(1);
        }
        //copia apenas a parte que nao tem o cadastro_excluir
            while(fread(cadastro, sizeof(Cadastro), 1, arq)==1){
                if((*cadastro).codigo != codigo_excluir)
                    fwrite(cadastro, sizeof(Cadastro), 1, arq_aux);
            }

    fclose(arq);
    fclose(arq_aux);
    
    do{  
    printf("Deseja excluir permanentemente o arquivo do seu computador?\n");
    printf("Digite s-sim ou n-nao\n");
    scanf(" %c", &str_resp);

        if(str_resp=='s'){
            remove(str);
            rename(str_backup, str);
            repet = 1;
        }
        else if(str_resp=='n'){
            rename(str_backup, str);
            repet = 1;
        }
        else{
            repet = 0;
        }

    }while(repet == 0);
}

int main(){
    Cadastro cadastro;

    escrever(&cadastro);
    ler(&cadastro);
    inserir(&cadastro);
    excluir(&cadastro);
}