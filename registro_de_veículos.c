#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "veiculos.dat"
#define OUTPUT_FILE "respostas.txt"

typedef struct {
    int id;
    char modelo[50];
    char placa[10];
    int ano;
} Veiculo;

void adicionarVeiculo() {
    FILE *file = fopen(FILE_NAME, "ab");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    Veiculo veiculo;
    printf("Digite o ID do veículo: ");
    scanf("%d", &veiculo.id);
    printf("Digite o modelo do veículo: ");
    scanf(" %[^\n]s", veiculo.modelo);
    printf("Digite a placa do veículo: ");
    scanf("%s", veiculo.placa);
    printf("Digite o ano de fabricação do veículo: ");
    scanf("%d", &veiculo.ano);

    fwrite(&veiculo, sizeof(Veiculo), 1, file);
    fclose(file);

    printf("Veículo adicionado com sucesso!\n");
}

void alterarVeiculo() {
    FILE *file = fopen(FILE_NAME, "rb+");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    int id, encontrado = 0;
    printf("Digite o ID do veículo que deseja alterar: ");
    scanf("%d", &id);

    Veiculo veiculo;
    while (fread(&veiculo, sizeof(Veiculo), 1, file)) {
        if (veiculo.id == id) {
            encontrado = 1;
            printf("Digite o novo modelo do veículo: ");
            scanf(" %[^\n]s", veiculo.modelo);
            printf("Digite a nova placa do veículo: ");
            scanf("%s", veiculo.placa);
            printf("Digite o novo ano de fabricação do veículo: ");
            scanf("%d", &veiculo.ano);

            fseek(file, -sizeof(Veiculo), SEEK_CUR);
            fwrite(&veiculo, sizeof(Veiculo), 1, file);
            printf("Veículo alterado com sucesso!\n");
            break;
        }
    }
    if (!encontrado)
        printf("Veículo com ID %d não encontrado.\n");

    fclose(file);
}

void excluirVeiculo() {
    FILE *file = fopen(FILE_NAME, "rb");
    FILE *tempFile = fopen("temp.dat", "wb");
    if (file == NULL || tempFile == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    int id, encontrado = 0;
    printf("Digite o ID do veículo que deseja excluir: ");
    scanf("%d", &id);

    Veiculo veiculo;
    while (fread(&veiculo, sizeof(Veiculo), 1, file)) {
        if (veiculo.id != id) {
            fwrite(&veiculo, sizeof(Veiculo), 1, tempFile);
        } else {
            encontrado = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (encontrado)
        printf("Veículo excluído com sucesso!\n");
    else
        printf("Veículo com ID %d não encontrado.\n");
}

void consultarVeiculo() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char placa[10];
    int encontrado = 0;
    printf("Digite a placa do veículo que deseja consultar: ");
    scanf("%s", placa);

    Veiculo veiculo;
    while (fread(&veiculo, sizeof(Veiculo), 1, file)) {
        if (strcmp(veiculo.placa, placa) == 0) {
            printf("ID: %d\nModelo: %s\nPlaca: %s\nAno: %d\n", veiculo.id, veiculo.modelo, veiculo.placa, veiculo.ano);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado)
        printf("Veículo com placa %s não encontrado.\n", placa);

    fclose(file);
}

void listarVeiculos() {
    FILE *file = fopen(FILE_NAME, "rb");
    FILE *output = fopen(OUTPUT_FILE, "w");
    if (file == NULL || output == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    Veiculo veiculo;
    printf("Lista de veículos cadastrados:\n");
    fprintf(output, "Lista de veículos cadastrados:\n");
    while (fread(&veiculo, sizeof(Veiculo), 1, file)) {
        printf("ID: %d\nModelo: %s\nPlaca: %s\nAno: %d\n\n", veiculo.id, veiculo.modelo, veiculo.placa, veiculo.ano);
        fprintf(output, "ID: %d\nModelo: %s\nPlaca: %s\nAno: %d\n\n", veiculo.id, veiculo.modelo, veiculo.placa, veiculo.ano);
    }

    fclose(file);
    fclose(output);
}

int main() {
    int opcao;

    do {
        printf("\nMenu:\n");
        printf("1. Adicionar veículo\n");
        printf("2. Alterar veículo\n");
        printf("3. Excluir veículo\n");
        printf("4. Consultar veículo pela placa\n");
        printf("5. Listar todos os veículos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: adicionarVeiculo(); break;
            case 2: alterarVeiculo(); break;
            case 3: excluirVeiculo(); break;
            case 4: consultarVeiculo(); break;
            case 5: listarVeiculos(); break;
            case 0: printf("Encerrando o programa.\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
