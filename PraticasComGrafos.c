#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VALOR_ASCII_CARACTER_0 48
#define VALOR_ASCII_CARACTER_9 57

#define CODIGO_ENCERRAR_PROGRAMA -127
#define CODIGO_DADO_INCORRETO -126


#define TAMANHO_MAXIMO_DA_LINHA 200
#define TRUE 0x01
#define FALSE 0x00

#define PESO_QUANDO_NAO_HA_CAMINHO 0
#define PESO_CAMINHO_CONVENCIONAL 1
#define PESO_PARA_LOOP 2


typedef int Dado;
typedef unsigned char Boolean;
typedef unsigned int Peso;

typedef struct{
    Dado primeiroPonto;
    Dado segundoPonto;
}Aresta;

typedef struct{
    Peso **matrizDeAdjacencias;
    Dado *vetorDeDados;
    int quantidadeDeDados;
}Grafo;

typedef struct CelulaDaLista{
    struct CelulaDaLista *proximo;
    Dado dado;
}CelulaDaLista;

typedef struct{
    CelulaDaLista *primeiroDaLista;
    int tamanho;
}Lista;

typedef struct{
    Lista *lista;
    Peso pesoTotal;
}Caminho;

typedef struct CelulaDaListaDeCaminhos{
    struct CelulaDaListaDeCaminhos *proximoDaListaDeCaminhos;
    Caminho *caminho;
}CelulaDaListaDeCaminhos;

typedef struct{
    CelulaDaListaDeCaminhos *primeiroDaListaDeCaminhos;
    int quantidadeDeCaminhos;
}ListaDeCaminhos;

void limparVetorDeCaracteres(char *vetor,int tamanho){
    if(vetor == NULL){
        return;
    }

    int i;
    for(i=0;i<tamanho;i++){
        vetor[i] = '\0';
    }
}



void destruirGrafo(Grafo **grafo){
    if(grafo == NULL){
        return;
    }
    else if((*grafo) == NULL){
        grafo = NULL;
        return;
    }

    if((**grafo).vetorDeDados != NULL){
        free((**grafo).vetorDeDados);
        (**grafo).vetorDeDados = NULL;
    }

    if((**grafo).matrizDeAdjacencias != NULL){
        int i;
        int j;

        for(i = (**grafo).quantidadeDeDados - 1;i>=0;i--){
            if(((**grafo).matrizDeAdjacencias)[i] != NULL){
                free(((**grafo).matrizDeAdjacencias)[i]);
                ((**grafo).matrizDeAdjacencias)[i] = NULL;
            }
        }

        free((**grafo).matrizDeAdjacencias);
        (**grafo).matrizDeAdjacencias = NULL;
    }

    (**grafo).quantidadeDeDados = 0;
    free(*grafo);
    *grafo = NULL;
    grafo = NULL;
}

void criarGrafo(Grafo **grafo,int quantidadeDeDados){
    destruirGrafo(grafo);
    if(quantidadeDeDados <= 0){
        return;
    }

    (*grafo) = (Grafo*) malloc(sizeof(Grafo));

    if((*grafo) == NULL){
        return;
    }

    (**grafo).vetorDeDados = (Dado*) malloc(quantidadeDeDados*(sizeof(Dado)));
    if((**grafo).vetorDeDados == NULL){
        destruirGrafo(grafo);
        return;
    }
    (**grafo).quantidadeDeDados = quantidadeDeDados;

    (**grafo).matrizDeAdjacencias = (Peso**) malloc(((**grafo).quantidadeDeDados)*(sizeof(Peso*)));
    if((**grafo).matrizDeAdjacencias == NULL){
        destruirGrafo(grafo);
        return;
    }

    int i;
    int j;
    for(i=0;i<(**grafo).quantidadeDeDados;i++){
        ((**grafo).matrizDeAdjacencias)[i] = (Peso*) malloc(((**grafo).quantidadeDeDados)*(sizeof(Peso)));
        if(((**grafo).matrizDeAdjacencias)[i] == NULL){
            destruirGrafo(grafo);
            return;
        }

        for(j=0;j<(**grafo).quantidadeDeDados;j++){
            ((**grafo).matrizDeAdjacencias)[i][j] = PESO_QUANDO_NAO_HA_CAMINHO;
        }
    }

}

void copiarParaOVetorDeDadosDoGrafo(Grafo **grafo,Dado **vetorDeDados,int quantidadeDeDados){
    if(vetorDeDados == NULL){
        return;
    }
    else if((*vetorDeDados) == NULL){
        return;
    }
    else if(grafo == NULL){
        return;
    }
    else if((*grafo) == NULL){
        return;
    }
    else if(quantidadeDeDados <= 0){
        return;
    }
    else if((**grafo).vetorDeDados != NULL){
        free((**grafo).vetorDeDados);
        (**grafo).vetorDeDados = NULL;
    }

    (**grafo).vetorDeDados = (Dado*) malloc(quantidadeDeDados*(sizeof(Dado)));
    if((**grafo).vetorDeDados == NULL){
        destruirGrafo(grafo);
        return;
    }

    int i;
    for(i=0;i<quantidadeDeDados;i++){
        ((**grafo).vetorDeDados)[i] = (*vetorDeDados)[i];
    }
}

void destruirCelulaDaLista(CelulaDaLista **celulaDaLista){
    if(celulaDaLista == NULL){
        return;
    }
    else if((*celulaDaLista) == NULL){
        celulaDaLista = NULL;
        return;
    }
    
    (**celulaDaLista).dado = 0;
    (**celulaDaLista).proximo = NULL;

    free(*celulaDaLista);
    (*celulaDaLista) = NULL;
    celulaDaLista = NULL;
}

void destruirLista(Lista **lista){
    if(lista == NULL){
        return;
    }
    else if((*lista) == NULL){
        lista = NULL;
        return;
    }

    CelulaDaLista *celulaASerApagada = NULL;

    while((**lista).primeiroDaLista != NULL){
        celulaASerApagada = (**lista).primeiroDaLista;
        (**lista).primeiroDaLista = (*((**lista).primeiroDaLista)).proximo;
        destruirCelulaDaLista(&celulaASerApagada);
    }

    (**lista).tamanho = 0;
    free(*lista);
    (*lista) = NULL;
    lista = NULL;    
}

void destruirCaminho(Caminho **caminho){
    if(caminho == NULL){
        return;
    }
    else if((*caminho) == NULL){
        caminho = NULL;
        return;
    }

    destruirLista(&((**caminho).lista));
    (**caminho).pesoTotal = 0;
    
    free(*caminho);
    (*caminho) = NULL;
    caminho = NULL;    
}


void destruirCelulaDaListaDeCaminhos(CelulaDaListaDeCaminhos **celulaDaListaDeCaminhos){
    if(celulaDaListaDeCaminhos == NULL){
        return;
    }
    else if((*celulaDaListaDeCaminhos) == NULL){
        celulaDaListaDeCaminhos = NULL;
        return;
    }
    
    (**celulaDaListaDeCaminhos).proximoDaListaDeCaminhos = NULL;
    destruirCaminho(&((**celulaDaListaDeCaminhos).caminho));

    free(*celulaDaListaDeCaminhos);
    (*celulaDaListaDeCaminhos) = NULL;
    celulaDaListaDeCaminhos = NULL;
}

void destruirListaDeCaminhos(ListaDeCaminhos **listaDeCaminhos){
    if(listaDeCaminhos == NULL){
        return;
    }
    else if((*listaDeCaminhos) == NULL){
        listaDeCaminhos = NULL;
        return;
    }

    CelulaDaListaDeCaminhos *celulaDaListaDeCaminhosASerApagada = NULL;

    while((**listaDeCaminhos).primeiroDaListaDeCaminhos != NULL){
        celulaDaListaDeCaminhosASerApagada = (**listaDeCaminhos).primeiroDaListaDeCaminhos;
        (**listaDeCaminhos).primeiroDaListaDeCaminhos = (*((**listaDeCaminhos).primeiroDaListaDeCaminhos)).proximoDaListaDeCaminhos;
        destruirCelulaDaListaDeCaminhos(&celulaDaListaDeCaminhosASerApagada);
    }

    (**listaDeCaminhos).quantidadeDeCaminhos = 0;
    free(*listaDeCaminhos);
    (*listaDeCaminhos) = NULL;
    listaDeCaminhos = NULL;    
}


CelulaDaLista *criarCelulaDaLista(Dado dado){
    CelulaDaLista *novaCelulaDaLista = (CelulaDaLista*) malloc(sizeof(CelulaDaLista));
    if(novaCelulaDaLista != NULL){
        (*novaCelulaDaLista).proximo = NULL;
        (*novaCelulaDaLista).dado = dado;
    }

    return novaCelulaDaLista;
}

Lista *criarLista(void){
    Lista *novaLista = (Lista*) malloc(sizeof(Lista));
    if(novaLista != NULL){
        (*novaLista).primeiroDaLista = NULL;
        (*novaLista).tamanho = 0;
    }

    return novaLista;
}

Caminho *criarCaminho(void){
    Caminho *novoCaminho = (Caminho*) malloc(sizeof(Caminho));
    if(novoCaminho != NULL){
        (*novoCaminho).lista = criarLista();
        (*novoCaminho).pesoTotal = 0;
    }

    return novoCaminho;
}

Boolean inserirNaLista(Lista **lista,Dado dado){
    if(lista == NULL){
        return FALSE;
    }
    else if((*lista) == NULL){
        (*lista) = criarLista();
        if((*lista) == NULL){
            return FALSE;
        }
    }

    if((**lista).primeiroDaLista == NULL){
        (**lista).primeiroDaLista = criarCelulaDaLista(dado);
    }
    else{
        CelulaDaLista *atualUltimoDaLista = (**lista).primeiroDaLista;
        while((*atualUltimoDaLista).proximo != NULL){
            atualUltimoDaLista = (*atualUltimoDaLista).proximo;
        }
        CelulaDaLista *novoUltimoDaLista = criarCelulaDaLista(dado);
        (*atualUltimoDaLista).proximo = novoUltimoDaLista;
    }
    
    (**lista).tamanho = (**lista).tamanho + 1;
    return TRUE;
}

void copiarLista(Lista **listaDeDestino,Lista **listaDeOrigem){
    if(listaDeOrigem == NULL){
        return;
    }
    else if(listaDeDestino == NULL){
        return;   
    }
    else if((*listaDeOrigem) == NULL){
        (*listaDeDestino) = NULL;
        listaDeDestino = NULL;
        return;
    }

    if((*listaDeDestino) != NULL){
        destruirLista(listaDeDestino);
    }

    (*listaDeDestino) = criarLista();
    
    if((*listaDeDestino) == NULL){
        return;
    }

    if((**listaDeOrigem).primeiroDaLista == NULL){
        (**listaDeDestino).primeiroDaLista = NULL;
    }
    else{
        CelulaDaLista *auxiliar = (**listaDeOrigem).primeiroDaLista;
        Boolean resultado;
        while(auxiliar != NULL){
            resultado = inserirNaLista(listaDeDestino,(*auxiliar).dado);

            if(resultado == FALSE){
                destruirLista(listaDeDestino);
                return;
            }

            auxiliar = (*auxiliar).proximo;
        }
    }

    (**listaDeDestino).tamanho = (**listaDeOrigem).tamanho;
}

void copiarCaminho(Caminho **caminhoDeDestino,Caminho **caminhoDeOrigem){
    if(caminhoDeOrigem == NULL){
        return;
    }
    else if(caminhoDeDestino == NULL){
        return;   
    }
    else if((*caminhoDeOrigem) == NULL){
        (*caminhoDeDestino) = NULL;
        caminhoDeDestino = NULL;
        return;
    }

    if((*caminhoDeDestino) != NULL){
        destruirCaminho(caminhoDeDestino);
    }

    (*caminhoDeDestino) = criarCaminho();
    
    if((*caminhoDeDestino) == NULL){
        return;
    }

    if((**caminhoDeOrigem).lista == NULL){
        (**caminhoDeDestino).lista = NULL;
    }
    else{
        copiarLista(&((**caminhoDeDestino).lista),&((**caminhoDeOrigem).lista));
    }

    (**caminhoDeDestino).pesoTotal = (**caminhoDeOrigem).pesoTotal;
}

CelulaDaListaDeCaminhos *criarCelulaDaListaDeCaminhos(Caminho **caminhoASerCopiado){
    if(caminhoASerCopiado == NULL){
        return NULL;
    }

    CelulaDaListaDeCaminhos *novaCelulaDaListaDeCaminhos = (CelulaDaListaDeCaminhos*) malloc(sizeof(CelulaDaListaDeCaminhos));
    if(novaCelulaDaListaDeCaminhos != NULL){
        (*novaCelulaDaListaDeCaminhos).proximoDaListaDeCaminhos = NULL;
        
        if((*caminhoASerCopiado) == NULL){
            (*novaCelulaDaListaDeCaminhos).caminho = NULL;
        }
        else{
            copiarCaminho(&((*novaCelulaDaListaDeCaminhos).caminho),caminhoASerCopiado);
        }
    }
    
    return novaCelulaDaListaDeCaminhos;
}

ListaDeCaminhos *criarListaDeCaminhos(void){
    ListaDeCaminhos *novaListaDeCaminhos = (ListaDeCaminhos*) malloc(sizeof(ListaDeCaminhos));
    if(novaListaDeCaminhos != NULL){
        (*novaListaDeCaminhos).primeiroDaListaDeCaminhos = NULL;
        (*novaListaDeCaminhos).quantidadeDeCaminhos = 0;
    }

    return novaListaDeCaminhos;
}

void corrigirPesoTotalSeListaForVazia(Caminho **caminho){
    if(caminho == NULL){
        return;
    }
    else if((*caminho) == NULL){
        return;
    }
    else if((**caminho).lista == NULL){
        (**caminho).pesoTotal = 0;
        return;
    }
    else if((*((**caminho).lista)).primeiroDaLista == NULL){
        (*((**caminho).lista)).tamanho = 0;
        (**caminho).pesoTotal = 0;
        return;
    }
    else if((*((**caminho).lista)).tamanho <= 0){
        destruirLista(&((**caminho).lista));
        (**caminho).pesoTotal = 0;
        return;
    }
}

Boolean dadoEstaNaLista(Lista **lista,Dado dado){
    if(lista == NULL){
        return FALSE;
    }
    else if((*lista) == NULL){
        return FALSE;
    }
    else if((**lista).primeiroDaLista == NULL){
        return FALSE;
    }
    else if((**lista).tamanho <= 0){
        destruirLista(lista);
        return FALSE;
    }

    CelulaDaLista *auxiliar = (**lista).primeiroDaLista;
    while(auxiliar != NULL){
        if((*auxiliar).dado == dado){
            return TRUE;
        }

        auxiliar = (*auxiliar).proximo;
    }

    return FALSE;
}

Boolean dadoEstaNoCaminho(Caminho **caminho,Dado dado){
    if(caminho == NULL){
        return FALSE;
    }
    else if((*caminho) == NULL){
        return FALSE;
    }
    
    Boolean resultado = dadoEstaNaLista(&((**caminho).lista),dado);
    corrigirPesoTotalSeListaForVazia(caminho);
    return resultado;
}



Boolean estaNoVetorDeDados(Dado dado,Dado **vetorDeDados,int *quantidadeDeDados){
    if(vetorDeDados == NULL){
        return FALSE;
    }
    else if((*vetorDeDados) == NULL){
        vetorDeDados = NULL;
        return FALSE;
    }
    else if((quantidadeDeDados == NULL) || ((*quantidadeDeDados) <= 0)){
        free(*vetorDeDados);
        (*vetorDeDados) = NULL;
        vetorDeDados = NULL;

        if(quantidadeDeDados != NULL){
            (*quantidadeDeDados) == 0;
        }

        return FALSE;
    }

    int i;
    for(i=0;i<(*quantidadeDeDados);i++){
        if((*vetorDeDados)[i] == dado){
            return TRUE;
        }
    }
    return FALSE;
}

void inserirNoCaminho(Caminho **caminho,Dado dado,Peso peso){
    if(caminho == NULL){
        return;
    }
    else if((*caminho) == NULL){
        return;
    }
    
    Boolean resultado = inserirNaLista(&((**caminho).lista),dado);
    if(resultado == TRUE){
        (**caminho).pesoTotal = (**caminho).pesoTotal + peso;
    }
}


int retornarPosicaoDoDadoNoVetorDeDadosDoGrafo(Grafo **grafo,Dado dado){
    if(grafo == NULL){
        return -1;
    }
    else if((*grafo) == NULL){
        return -1;
    }
    else if((**grafo).vetorDeDados == NULL){
        return -1;
    }
    int i;

    for(i=0;i<(**grafo).quantidadeDeDados;i++){
        if(((**grafo).vetorDeDados)[i] == dado){
            return i;
        }
    }

    return -1;
}

Boolean caminhoTemPontoRepetido(Caminho **caminho,Dado dadoDeOrigem){
    if(caminho == NULL){
        return FALSE;
    }
    else if((*caminho) == NULL){
        return FALSE;
    }
    else if((**caminho).lista == NULL){
        return FALSE;
    }
    
    return dadoEstaNaLista(&((**caminho).lista),dadoDeOrigem);
}

void eliminarCaminhosNulos(ListaDeCaminhos **listaDeCaminhos){
    if(listaDeCaminhos == NULL){
        return;
    }
    else if((*listaDeCaminhos) == NULL){
        return;
    }
    else if((**listaDeCaminhos).primeiroDaListaDeCaminhos == NULL){
        return;
    }
    else if((**listaDeCaminhos).quantidadeDeCaminhos <= 0){
        return;
    }
    
    if((*((**listaDeCaminhos).primeiroDaListaDeCaminhos)).proximoDaListaDeCaminhos == NULL) {
        if((*((**listaDeCaminhos).primeiroDaListaDeCaminhos)).caminho == NULL){
            destruirCelulaDaListaDeCaminhos(&((**listaDeCaminhos).primeiroDaListaDeCaminhos));
        }
    }

    CelulaDaListaDeCaminhos *auxiliar = (**listaDeCaminhos).primeiroDaListaDeCaminhos;
    CelulaDaListaDeCaminhos *anteriorAoAuxiliar = NULL;
    CelulaDaListaDeCaminhos *celulaASerExcluida = NULL;
    
    while(auxiliar != NULL){
        if((*auxiliar).caminho == NULL){
            if(anteriorAoAuxiliar == NULL){
                celulaASerExcluida = (**listaDeCaminhos).primeiroDaListaDeCaminhos;
                (**listaDeCaminhos).primeiroDaListaDeCaminhos = (*((**listaDeCaminhos).primeiroDaListaDeCaminhos)).proximoDaListaDeCaminhos;
                
            }
            else{
                celulaASerExcluida = auxiliar;
                (*anteriorAoAuxiliar).proximoDaListaDeCaminhos = (*auxiliar).proximoDaListaDeCaminhos;
                
            }
            destruirCelulaDaListaDeCaminhos(&celulaASerExcluida);
        }

        anteriorAoAuxiliar = auxiliar;
        auxiliar = (*auxiliar).proximoDaListaDeCaminhos;
    }
}

void eliminarCaminhosComPontosRepetidos(ListaDeCaminhos **listaDeCaminhos,Dado dadoDeOrigem){
    if(listaDeCaminhos == NULL){
        return;
    }
    else if((*listaDeCaminhos) == NULL){
        return;
    }
    else if((**listaDeCaminhos).primeiroDaListaDeCaminhos == NULL){
        return;
    }
    else if((**listaDeCaminhos).quantidadeDeCaminhos <= 0){
        return;
    }

    CelulaDaListaDeCaminhos *auxiliar = (**listaDeCaminhos).primeiroDaListaDeCaminhos;
    Boolean resultado;
    while(auxiliar != NULL){
        if((*auxiliar).caminho != NULL){
            resultado = caminhoTemPontoRepetido(&((*auxiliar).caminho),dadoDeOrigem);
            if(resultado == TRUE){
                destruirCaminho(&((*auxiliar).caminho));
            }
        }

        auxiliar = (*auxiliar).proximoDaListaDeCaminhos;
    }

    eliminarCaminhosNulos(listaDeCaminhos);
}

void inserirCaminhoNaListaDeCaminhos(ListaDeCaminhos **listaDeCaminhos,Caminho **caminhoASerInserido){
    if(caminhoASerInserido == NULL){
        return;
    }
    else if((*caminhoASerInserido) == NULL){
        return;
    }

    if(listaDeCaminhos == NULL){
        return;
    }
    else if((*listaDeCaminhos) == NULL){
        (*listaDeCaminhos) = criarListaDeCaminhos();
    }
    CelulaDaListaDeCaminhos *novaCelulaDaListaDeCaminhos = criarCelulaDaListaDeCaminhos(caminhoASerInserido);
    if((**listaDeCaminhos).primeiroDaListaDeCaminhos == NULL){
        (**listaDeCaminhos).primeiroDaListaDeCaminhos = novaCelulaDaListaDeCaminhos;    
    }
    else{
        CelulaDaListaDeCaminhos *auxiliar = (**listaDeCaminhos).primeiroDaListaDeCaminhos;
        while((*auxiliar).proximoDaListaDeCaminhos != NULL){
            auxiliar = (*auxiliar).proximoDaListaDeCaminhos;
        }
        (*auxiliar).proximoDaListaDeCaminhos = novaCelulaDaListaDeCaminhos;
    }
    
    (**listaDeCaminhos).quantidadeDeCaminhos = (**listaDeCaminhos).quantidadeDeCaminhos + 1;


}

void inserirDadoEmTodosOsCaminhosDaLista(ListaDeCaminhos **listaDeCaminhos,Dado dadoDeOrigem,Peso peso){
    if(listaDeCaminhos == NULL){
        return;
    }
    else if((*listaDeCaminhos) == NULL){
        (*listaDeCaminhos) = criarListaDeCaminhos();
    }

    Caminho *novoCaminho = criarCaminho();
    if(novoCaminho == NULL){
        return;
    }
    inserirNoCaminho(&novoCaminho,dadoDeOrigem,peso);
    inserirCaminhoNaListaDeCaminhos(listaDeCaminhos,&novoCaminho);
    destruirCaminho(&novoCaminho);
}

void imprimirMensagemCaminhoVazio(void){
    printf("\n\n\n///////////////////// CAMINHO VAZIO ///////////////////\n\n\n");
}

void imprimirMensagemCaminhoNulo(void){
    printf("\n\n\n///////////////////// CAMINHO NULO ///////////////////\n\n\n");
}

void imprimirCaminho(Caminho **caminho){
    if(caminho == NULL){
        imprimirMensagemCaminhoNulo();
    }
    else if((*caminho) == NULL){
        imprimirMensagemCaminhoNulo();
    }
    else if((**caminho).pesoTotal <= 0){
        imprimirMensagemCaminhoVazio();
    }
    else if((**caminho).lista == NULL){
        imprimirMensagemCaminhoVazio();
    }
    else if((*((**caminho).lista)).primeiroDaLista == NULL){
        imprimirMensagemCaminhoVazio();
    }
    else if((*((**caminho).lista)).tamanho <= 0){
        imprimirMensagemCaminhoVazio();
    }
    else{
        if((*((**caminho).lista)).tamanho == 1){
            printf("\n######################## CAMINHO COM 1 ELEMENTO E PESO %d #########################\n",(**caminho).pesoTotal);
        }
        else{
            printf("\n######################## CAMINHO COM %d ELEMENTOS E PESO %d #########################\n",(*((**caminho).lista)).tamanho,(**caminho).pesoTotal);
        }

        int contadorDeEtapa=0;
        CelulaDaLista *auxiliar = (*((**caminho).lista)).primeiroDaLista;
        while(auxiliar != NULL){
            printf("Etapa %d: Dado %d\n",contadorDeEtapa,(*auxiliar).dado);

            contadorDeEtapa++;
            auxiliar = (*auxiliar).proximo;
        }

        printf("\n######################## FIM DO CAMINHO #########################\n\n\n");
    }
}

void imprimirMensagemListaDeCaminhosVazia(void){
    printf("\n\n\n///////////////////// LISTA DE CAMINHOS VAZIA ///////////////////\n\n\n");
}

void imprimirMensagemListaDeCaminhosNula(void){
    printf("\n\n\n///////////////////// LISTA DE CAMINHOS NULA ///////////////////\n\n\n");
}

void imprimirListaDeCaminhos(ListaDeCaminhos **listaDeCaminhos){
    if(listaDeCaminhos == NULL){
        imprimirMensagemListaDeCaminhosNula();
    }
    else if((*listaDeCaminhos) == NULL){
        imprimirMensagemListaDeCaminhosNula();
    }
    else if((**listaDeCaminhos).quantidadeDeCaminhos <= 0){
        imprimirMensagemListaDeCaminhosVazia();
    }
    else if((**listaDeCaminhos).primeiroDaListaDeCaminhos == NULL){
        imprimirMensagemListaDeCaminhosVazia();
    }
    else{
        if((**listaDeCaminhos).quantidadeDeCaminhos == 1){
            printf("\n######################## LISTA DE CAMINHOS COM 1 CAMINHO POSSIVEL #########################\n");
        }
        else{
            printf("\n######################## LISTA DE CAMINHOS COM %d CAMINHOS POSSIVEIS #########################\n",(**listaDeCaminhos).quantidadeDeCaminhos);
        }

        CelulaDaListaDeCaminhos *auxiliar = (**listaDeCaminhos).primeiroDaListaDeCaminhos;
        while(auxiliar != NULL){
            
            imprimirCaminho(&((*auxiliar).caminho));
            auxiliar = (*auxiliar).proximoDaListaDeCaminhos;
        }

        printf("\n######################## FIM DA LISTA DE CAMINHOS #########################\n\n\n");
    }
}

void levantarListaDeCaminhosPossiveis(ListaDeCaminhos **listaDeCaminhos,Caminho **caminhoAtual,Grafo **grafo,int posicaoDeOrigem, int posicaoDeDestino,Peso peso){
    if(listaDeCaminhos == NULL){
        return;
    }
    else if((*listaDeCaminhos) == NULL){
        return;
    }
    else if(grafo == NULL){
        return;
    }
    else if((*grafo) == NULL){
        return;
    }
    else if((posicaoDeOrigem < 0) || (posicaoDeOrigem >= (**grafo).quantidadeDeDados)){
        return;
    }
    else if((posicaoDeDestino < 0) || (posicaoDeDestino >= (**grafo).quantidadeDeDados)){
        return;
    }
    else if(caminhoAtual == NULL){
        return;
    }
    else if((*caminhoAtual) == NULL){
        (*caminhoAtual) = criarCaminho();
    }

    Dado dadoDeOrigem = (**grafo).vetorDeDados[posicaoDeOrigem];
    inserirNoCaminho(caminhoAtual,dadoDeOrigem,peso);
    
    if(posicaoDeOrigem == posicaoDeDestino){
        inserirCaminhoNaListaDeCaminhos(listaDeCaminhos,caminhoAtual);
    }
    else{
        int j;
        Peso novoPeso;
        Caminho *copiaDoCaminhoAtual = NULL;
        Dado dadoCandidato;
        
        for(j=0;j<(**grafo).quantidadeDeDados;j++){
            dadoCandidato = (**grafo).vetorDeDados[j];
            
            if((caminhoTemPontoRepetido(caminhoAtual,dadoCandidato) == FALSE) && ((**grafo).matrizDeAdjacencias[posicaoDeOrigem][j] > 0)){
                Peso novoPeso = ((**grafo).matrizDeAdjacencias[posicaoDeOrigem][j]);
                copiarCaminho(&copiaDoCaminhoAtual,caminhoAtual);
                
                levantarListaDeCaminhosPossiveis(listaDeCaminhos,&copiaDoCaminhoAtual,grafo,j,posicaoDeDestino,novoPeso);
                destruirCaminho(&copiaDoCaminhoAtual);
                copiaDoCaminhoAtual = NULL;
            }
        }
    }
    destruirCaminho(caminhoAtual);
    eliminarCaminhosNulos(listaDeCaminhos);
}

Caminho *retornarCaminhoMaisCurto(Grafo **grafo,Dado dado1,Dado dado2){
    if(grafo == NULL){
        return NULL;
    }
    else if((*grafo) == NULL){
        return NULL;
    }
    
    int posicaoDado1 = retornarPosicaoDoDadoNoVetorDeDadosDoGrafo(grafo,dado1);
    int posicaoDado2 = retornarPosicaoDoDadoNoVetorDeDadosDoGrafo(grafo,dado2);
    if((posicaoDado1 == -1) || (posicaoDado2 == -1)){
        return NULL;
    }
    
    ListaDeCaminhos *listaDeCaminhos = criarListaDeCaminhos();
    if(listaDeCaminhos == NULL){
        return NULL;
    }
    Caminho *ponteiroCaminho = NULL;
    Caminho *caminhoMaisCurto = NULL;
    
    levantarListaDeCaminhosPossiveis(&listaDeCaminhos,&ponteiroCaminho,grafo,posicaoDado1,posicaoDado2,0);
    if(((*listaDeCaminhos).quantidadeDeCaminhos > 0) && ((*listaDeCaminhos).primeiroDaListaDeCaminhos != NULL)){
        CelulaDaListaDeCaminhos *auxiliar = (*listaDeCaminhos).primeiroDaListaDeCaminhos;
        Peso menorPeso = (*((*((*listaDeCaminhos).primeiroDaListaDeCaminhos)).caminho)).pesoTotal;
        ponteiroCaminho = (*((*listaDeCaminhos).primeiroDaListaDeCaminhos)).caminho;
        while(auxiliar != NULL){
            if((*((*auxiliar).caminho)).pesoTotal < menorPeso){
                menorPeso = (*((*auxiliar).caminho)).pesoTotal;
                ponteiroCaminho = (*auxiliar).caminho;
            }
            auxiliar = (*auxiliar).proximoDaListaDeCaminhos;
        }
        
        if(ponteiroCaminho != NULL){
            copiarCaminho(&caminhoMaisCurto,&ponteiroCaminho);
            ponteiroCaminho = NULL;
        }
    }

    destruirListaDeCaminhos(&listaDeCaminhos);
    
    return caminhoMaisCurto;
    
}

Boolean ehAlgarismo(char caracter){
    if((caracter >=VALOR_ASCII_CARACTER_0) && (caracter <= VALOR_ASCII_CARACTER_9)){
        return TRUE;
    }
    return FALSE;
}

Boolean ehVirgula(char caracter){
    if(caracter == ','){
        return TRUE;
    }
    return FALSE;
}

int retornarQuantidadeDeOcorrenciasNaLinha(char *linha,char caracter){
    int tamanhoDaLinha = strlen(linha);
    if(tamanhoDaLinha <= 0){
        return 0;
    }

    int i;
    int quantidadeDeOcorrencias = 0;
    for(i=0;i<tamanhoDaLinha;i++){
        if(linha[i] == caracter){
            quantidadeDeOcorrencias++;
        }
    }

    return quantidadeDeOcorrencias;
}

int retornarPosicaoDaPrimeiraOcorrencia(char *linha,char caracter){
    int tamanhoDaLinha = strlen(linha);
    if(tamanhoDaLinha <= 0){
        return -1;
    }

    int i;
    for(i=0;i<tamanhoDaLinha;i++){
        if(linha[i] == caracter){
            return i;
        }
    }

    return -1;
}

Boolean linhaPossuiUmaUnicaVirgula(char *linha){
    int quantidadeDeOcorrencias = retornarQuantidadeDeOcorrenciasNaLinha(linha,',');
    if(quantidadeDeOcorrencias == 1){
        return TRUE;
    }
    return FALSE;
}

Boolean virgulaEhOPrimeiroOuUltimoCaracterDaLinha(char *linha){
    int tamanhoDaLinha = strlen(linha);
    if(tamanhoDaLinha <= 0){
        return FALSE;
    }

    int posicaoDaPrimeiraOcorrencia = retornarPosicaoDaPrimeiraOcorrencia(linha,',');
    if((posicaoDaPrimeiraOcorrencia == 0) || (posicaoDaPrimeiraOcorrencia == (tamanhoDaLinha-1))){
        return TRUE;
    }
    return FALSE;

}

Boolean linhaPossuiApenasAlgarismosOuVirgulas(char *linha){
    int tamanhoDaLinha = strlen(linha);
    if(tamanhoDaLinha <= 0){
        return FALSE;
    }

    int i;
    for(i=0;i<tamanhoDaLinha;i++){
        if((ehAlgarismo(linha[i]) == FALSE) && (ehVirgula(linha[i]) == FALSE)){
            return FALSE;
        }
    }

    return TRUE;
}

Boolean linhaEstaNoFormatoDeAresta(char *linha){
    int tamanhoDaLinha = strlen(linha);
    if(tamanhoDaLinha <= 0){
        return FALSE;
    }

    if(linhaPossuiApenasAlgarismosOuVirgulas(linha) == TRUE){
        if(linhaPossuiUmaUnicaVirgula(linha) == TRUE){
            if(virgulaEhOPrimeiroOuUltimoCaracterDaLinha(linha) == FALSE){
                return TRUE;
            }
        }
    }

    return FALSE;
}

void retirarQuebraDeLinhaAoFinalDaString(char *vetor){
    int tamanhoDaLinha = strlen(vetor);
    if(tamanhoDaLinha <= 0){
        return;
    }
    int i;
    for(i=(tamanhoDaLinha - 1);i>=0;i--){
        if(vetor[i] == '\n'){
            vetor[i] = '\0';
        }
        else{
            break;
        }
    }
}

char *retornarSubstringDadosInicioEFim(char *vetor,int inicioInclusivo,int finalExclusivo){
    if(vetor == NULL){
        return NULL;
    }
    int tamanhoDoVetorOriginal = strlen(vetor);
    if(tamanhoDoVetorOriginal <= 0){
        return NULL;
    }
    else if((inicioInclusivo < 0) || (inicioInclusivo >= tamanhoDoVetorOriginal)){
        return NULL;
    }
    else if(finalExclusivo <= 0){
        return NULL;
    }
    else if(inicioInclusivo >= finalExclusivo){
        return NULL;
    }
    else if(finalExclusivo > tamanhoDoVetorOriginal){
        finalExclusivo = tamanhoDoVetorOriginal;
    }

    int tamanhoDoNovoVetor = finalExclusivo - inicioInclusivo + 1;
    char *novoVetor = (char*) malloc(tamanhoDoNovoVetor*(sizeof(char)));
    limparVetorDeCaracteres(novoVetor,tamanhoDoNovoVetor);
    int i;
    for(i=inicioInclusivo;i<finalExclusivo;i++){
        novoVetor[i-inicioInclusivo] = vetor[i];
    }

    return novoVetor;
}

char *retornarSubstringDadoInicio(char *vetor,int inicioInclusivo){
    if(vetor == NULL){
        return NULL;
    }
    int tamanhoDoVetorOriginal = strlen(vetor);
    if(tamanhoDoVetorOriginal <= 0){
        return NULL;
    }

    return retornarSubstringDadosInicioEFim(vetor,inicioInclusivo,tamanhoDoVetorOriginal);
}

char *retornarSubstringDadoFim(char *vetor,int finalExclusivo){
    if(vetor == NULL){
        return NULL;
    }
    int tamanhoDoVetorOriginal = strlen(vetor);
    if(tamanhoDoVetorOriginal <= 0){
        return NULL;
    }

    return retornarSubstringDadosInicioEFim(vetor,0,finalExclusivo);
}

int algarismoCorrespondenteAoCaracter(char caracter){
    if(ehAlgarismo(caracter) == TRUE){
        return caracter - VALOR_ASCII_CARACTER_0;
    }
    return -1;
}

int *exponenciacao(int base,int expoente){
    if(expoente < 0){
        return NULL;
    }

    if((expoente == 0) && (base == 0)){
        return NULL;
    }

    int *ponteiroDado = (int*) malloc(sizeof(int));
    if(ponteiroDado == NULL){
        return NULL;
    }
    int i;
    (*ponteiroDado) = 1;
    for(i=0;i<expoente;i++){
        (*ponteiroDado) = (*ponteiroDado)*base;
    }
    return ponteiroDado;
}

int *converterStringParaInteiro(char *vetor){
    if(vetor == NULL){
        return NULL;
    }
    int tamanho = strlen(vetor);
    if(tamanho <= 0){
        return NULL;
    }

    int soma = 0;
    int i = 0;
    int *resultadoDaExponenciacao = NULL;
    int algarismoCorrespondente;
    while(i<tamanho){
        algarismoCorrespondente = algarismoCorrespondenteAoCaracter(vetor[i]);

        if(algarismoCorrespondente == -1){
            return NULL;
        }

        resultadoDaExponenciacao = exponenciacao(10,tamanho - i - 1);
        if(resultadoDaExponenciacao == NULL){
            return NULL;
        }

        soma = soma + ((*resultadoDaExponenciacao)*algarismoCorrespondente);
        free(resultadoDaExponenciacao);
        resultadoDaExponenciacao = NULL;

        i++;
    }

    int *resultadoGeral = (int*) malloc(sizeof(int));
    if(resultadoGeral == NULL){
        return NULL;
    }
    *resultadoGeral = soma;
    return resultadoGeral;
}

Aresta *retornarArestaDadaLinha(char *linha){
    if(linha == NULL){
        return NULL;
    }
    retirarQuebraDeLinhaAoFinalDaString(linha);
    if(linhaEstaNoFormatoDeAresta(linha) == FALSE){
        return NULL;
    }

    int posicaoDaVirgula = retornarPosicaoDaPrimeiraOcorrencia(linha,',');
    char *vetorPrimeiroDado = retornarSubstringDadoFim(linha,posicaoDaVirgula);
    if(vetorPrimeiroDado == NULL){
        return NULL;
    }
    char *vetorSegundoDado = retornarSubstringDadoInicio(linha,posicaoDaVirgula+1);
    if(vetorSegundoDado == NULL){
        free(vetorPrimeiroDado);
        vetorPrimeiroDado = NULL;
        return NULL;
    }

    int *valorCorrespondenteAoVetorPrimeiroDado = converterStringParaInteiro(vetorPrimeiroDado);
    if(valorCorrespondenteAoVetorPrimeiroDado == NULL){
        free(vetorPrimeiroDado);
        vetorPrimeiroDado = NULL;
        free(vetorSegundoDado);
        vetorSegundoDado = NULL;
        return NULL;
    }

    int *valorCorrespondenteAoVetorSegundoDado = converterStringParaInteiro(vetorSegundoDado);
    if(valorCorrespondenteAoVetorSegundoDado == NULL){
        free(vetorPrimeiroDado);
        vetorPrimeiroDado = NULL;
        free(vetorSegundoDado);
        vetorSegundoDado = NULL;
        free(valorCorrespondenteAoVetorPrimeiroDado);
        valorCorrespondenteAoVetorPrimeiroDado = NULL;
        return NULL;
    }

    Aresta *novaAresta = (Aresta*) malloc(sizeof(Aresta));
    if(novaAresta != NULL){
        (*novaAresta).primeiroPonto = *valorCorrespondenteAoVetorPrimeiroDado;
        (*novaAresta).segundoPonto = *valorCorrespondenteAoVetorSegundoDado;
    }

    free(vetorPrimeiroDado);
    vetorPrimeiroDado = NULL;
    free(vetorSegundoDado);
    vetorSegundoDado = NULL;
    free(valorCorrespondenteAoVetorPrimeiroDado);
    valorCorrespondenteAoVetorPrimeiroDado = NULL;
    free(valorCorrespondenteAoVetorSegundoDado);
    valorCorrespondenteAoVetorSegundoDado = NULL;
    return novaAresta;
}

void processarParaOVetorDeDados(Dado dado,Dado **vetorDeDados,int *quantidadeDeDados){
    if(estaNoVetorDeDados(dado,vetorDeDados,quantidadeDeDados) == TRUE){
        return;
    }

    if(vetorDeDados == NULL){
        return;
    }
    else if(quantidadeDeDados == NULL){
        return;
    }
    else if((*quantidadeDeDados) < 0){
        (*quantidadeDeDados) = 0;
    }

    Dado *novoVetor = (Dado*) malloc(((*quantidadeDeDados) + 1)*sizeof(Dado));
    if(novoVetor != NULL){
        int i=0;
        while(i<(*quantidadeDeDados)){
            novoVetor[i] = (*vetorDeDados)[i];
            i++;
        }

        novoVetor[*quantidadeDeDados] = dado;
        (*quantidadeDeDados) = (*quantidadeDeDados) + 1;
    }

    if((*vetorDeDados) != NULL){
        free(*vetorDeDados);
        (*vetorDeDados) = NULL;
    }

    (*vetorDeDados) = novoVetor;
}



void preencherVetorDeDadosEQuantidadeDeElementos(Dado **vetorDeDados,int *quantidadeDeDados){
    FILE *arquivo = fopen("grafo.txt","r");

    if(arquivo == NULL){
        return;
    }
    else if(quantidadeDeDados == NULL){
        return;
    }
    else if(vetorDeDados == NULL){
        return;
    }

    *quantidadeDeDados = 0;
    Aresta *arestaLida = NULL;
    char buffer[TAMANHO_MAXIMO_DA_LINHA];

    while(!feof(arquivo)){
        limparVetorDeCaracteres(buffer,TAMANHO_MAXIMO_DA_LINHA);
        fgets(buffer,TAMANHO_MAXIMO_DA_LINHA-1,arquivo);
        arestaLida = retornarArestaDadaLinha(buffer);
        if(arestaLida != NULL){
            processarParaOVetorDeDados((*arestaLida).primeiroPonto,vetorDeDados,quantidadeDeDados);
            processarParaOVetorDeDados((*arestaLida).segundoPonto,vetorDeDados,quantidadeDeDados);
            free(arestaLida);
            arestaLida = NULL;
        }
    }

    fclose(arquivo);
    arquivo = NULL;
}



void lerArquivo(Grafo **grafo){
    Dado *vetorDeDados = NULL;
    int quantidadeDeDados = 0;

    preencherVetorDeDadosEQuantidadeDeElementos(&vetorDeDados,&quantidadeDeDados);
    FILE *arquivo = fopen("grafo.txt","r");
    if(arquivo == NULL){
        return;
    }
    destruirGrafo(grafo);
    if(quantidadeDeDados <= 0){
        if(vetorDeDados != NULL){
            free(vetorDeDados);
            vetorDeDados = NULL;
        }

        return;
    }

    criarGrafo(grafo,quantidadeDeDados);
    if((*grafo) == NULL){
        free(vetorDeDados);
        vetorDeDados = NULL;
        return;
    }
    copiarParaOVetorDeDadosDoGrafo(grafo,&vetorDeDados,quantidadeDeDados);

    free(vetorDeDados);
    vetorDeDados = NULL;

    Aresta *arestaLida = NULL;
    char buffer[TAMANHO_MAXIMO_DA_LINHA];
    int posicaoDoPrimeiroDado;
    int posicaoDoSegundoDado;

    while(!feof(arquivo)){
        limparVetorDeCaracteres(buffer,TAMANHO_MAXIMO_DA_LINHA);
        fgets(buffer,TAMANHO_MAXIMO_DA_LINHA-1,arquivo);
        arestaLida = retornarArestaDadaLinha(buffer);
        if(arestaLida != NULL){
            posicaoDoPrimeiroDado = retornarPosicaoDoDadoNoVetorDeDadosDoGrafo(grafo,(*arestaLida).primeiroPonto);
            posicaoDoSegundoDado = retornarPosicaoDoDadoNoVetorDeDadosDoGrafo(grafo,(*arestaLida).segundoPonto);

            if((posicaoDoPrimeiroDado == -1) || (posicaoDoSegundoDado == -1)){
                destruirGrafo(grafo);
                free(arestaLida);
                arestaLida = NULL;
                return;
            }

            if(posicaoDoPrimeiroDado == posicaoDoSegundoDado){
                (**grafo).matrizDeAdjacencias[posicaoDoPrimeiroDado][posicaoDoPrimeiroDado] += PESO_PARA_LOOP;
            }
            else{
                (**grafo).matrizDeAdjacencias[posicaoDoPrimeiroDado][posicaoDoSegundoDado] += PESO_CAMINHO_CONVENCIONAL;
                (**grafo).matrizDeAdjacencias[posicaoDoSegundoDado][posicaoDoPrimeiroDado] += PESO_CAMINHO_CONVENCIONAL;
            }

            free(arestaLida);
            arestaLida = NULL;
        }
    }

    fclose(arquivo);
    arquivo = NULL;
}



void imprimirMensagemGrafoVazio(void){
    printf("\n\n\n///////////////////// GRAFO VAZIO ///////////////////\n\n\n");
}

void imprimirMensagemGrafoNulo(void){
    printf("\n\n\n///////////////////// GRAFO NULO ///////////////////\n\n\n");
}

void imprimirGrafo(Grafo **grafo){
    if(grafo == NULL){
        imprimirMensagemGrafoNulo();
    }
    else if((*grafo) == NULL){
        imprimirMensagemGrafoNulo();
    }
    else if((**grafo).quantidadeDeDados <= 0){
        imprimirMensagemGrafoVazio();
    }
    else if((**grafo).vetorDeDados == NULL){
        imprimirMensagemGrafoVazio();
    }
    else if((**grafo).matrizDeAdjacencias == NULL){
        imprimirMensagemGrafoVazio();
    }
    else{
        if((**grafo).quantidadeDeDados == 1){
            printf("\n######################## GRAFO COM 1 UNICO ELEMENTO #########################\n");
        }
        else{
            printf("\n######################## GRAFO COM %d ELEMENTOS #########################\n",(**grafo).quantidadeDeDados);
        }

        printf("\n********** vetor de dados ****************\n");
        int i;
        for(i=0;i<(**grafo).quantidadeDeDados;i++){
            printf("[%d]",(**grafo).vetorDeDados[i]);
        }
        printf("\n\n\n");

        printf("\n********** matriz de adjacencias ****************\n");

        printf("  ");
        for(i=0;i<(**grafo).quantidadeDeDados;i++){
            printf(" %d ",(**grafo).vetorDeDados[i]);
        }
        printf("\n");


        int j;
        for(i=0;i<(**grafo).quantidadeDeDados;i++){
            printf("%d ",(**grafo).vetorDeDados[i]);
            if(((**grafo).matrizDeAdjacencias)[i] != NULL){
                for(j=0;j<(**grafo).quantidadeDeDados;j++){
                    printf("[%d]",(**grafo).matrizDeAdjacencias[i][j]);
                }
            }
            else{
                printf("LINHA NULA");
            }
            printf("\n");
        }

        printf("\n######################## FIM DO GRAFO #########################\n\n\n");
    }
}

int coletarDadoVindoDoTeclado(void){
    char buffer[TAMANHO_MAXIMO_DA_LINHA];

    limparVetorDeCaracteres(buffer,TAMANHO_MAXIMO_DA_LINHA);
    fflush(stdin);
    fgets(buffer,TAMANHO_MAXIMO_DA_LINHA-1,stdin);
    retirarQuebraDeLinhaAoFinalDaString(buffer);
    if((strcmp(buffer,"S") == 0) || (strcmp(buffer,"s") == 0)){
        return CODIGO_ENCERRAR_PROGRAMA;
    }
    
    int *dado = converterStringParaInteiro(buffer);
    if(dado == NULL){
        return CODIGO_DADO_INCORRETO;
    }
    else{
        int dadoValido = (*dado);
        free(dado);
        dado = NULL;
        return dadoValido;
    }
}

void processarBuscaPeloCaminhoMaisCurto(Grafo **grafo,int primeiroPonto,int segundoPonto){
    if(grafo == NULL){
        return;
    }
    else if((*grafo) == NULL){
        return;
    }
    
    Caminho *caminhoMaisCurto = NULL;
    
    caminhoMaisCurto = retornarCaminhoMaisCurto(grafo,primeiroPonto,segundoPonto);
    imprimirCaminho(&caminhoMaisCurto);
    destruirCaminho(&caminhoMaisCurto);
}

void executarPrograma(Grafo **grafo){
    imprimirGrafo(grafo);

    if(grafo == NULL){
        return;
    }
    else if((*grafo) == NULL){
        return;
    }

    
    
    
    int primeiroPontoDaAresta;
    int segundoPontoDaAresta;
    
    while(TRUE){
        system("cls");
        imprimirGrafo(grafo);

        while(TRUE){
            printf("Insira o inteiro positivo referente ao primeiro ponto do caminho\nOu digite s para sair:   ");
            primeiroPontoDaAresta = coletarDadoVindoDoTeclado();
            if(primeiroPontoDaAresta == CODIGO_DADO_INCORRETO){
                printf("DADO INVALIDO\n");
            }
            else if(primeiroPontoDaAresta == CODIGO_ENCERRAR_PROGRAMA){
                return;
            }
            else{
                break;
            }
        }

        while(TRUE){
            printf("Insira o inteiro positivo referente ao segundo ponto do caminho\nOu digite s para sair:   ");
            segundoPontoDaAresta = coletarDadoVindoDoTeclado();
            if(segundoPontoDaAresta == CODIGO_DADO_INCORRETO){
                printf("DADO INVALIDO\n");
            }
            else if(segundoPontoDaAresta == CODIGO_ENCERRAR_PROGRAMA){
                return;
            }
            else{
                break;
            }
        }

        processarBuscaPeloCaminhoMaisCurto(grafo,primeiroPontoDaAresta,segundoPontoDaAresta);
        system("pause");
    }    
}

void imprimirEncerramentoDaAplicacao(void){
    printf("\n\n\n\n\nENCERRANDO APLICACAO...\n\n\n\n\n\n");
}

int main(void){
    Grafo *grafo = NULL;
    lerArquivo(&grafo);
    executarPrograma(&grafo);
    destruirGrafo(&grafo);
    imprimirEncerramentoDaAplicacao();
    return 0;
}