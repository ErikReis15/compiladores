#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

AST * novoNo(Tipo tipo){
    AST* no = (AST*) malloc(sizeof(AST));
    if (!no) { perror("malloc"); exit(1); }
    no->tipo = tipo;
    /* initialize union safely */
    memset(&no->dado, 0, sizeof(no->dado));
    no->esquerda = NULL;
    no->direita = NULL;
    no->meio    = NULL;
    return no;
}

/*void printAST(AST *n, int nivel) {
    if (!n) return;

    for (int i = 0; i < nivel; i++) printf("  ");

    switch (n->tipo) {
        case VAL:     printf("(VAL %d)\n", n->dado.valor); break;
        case ID:      printf("(ID \"%s\")\n", n->dado.id ? n->dado.id : "<null>"); break;
        case OP:      printf("(OP %d)\n", n->dado.operador); break;
        case IF:      printf("(IF)\n"); break;
        case IF_ELSE: printf("(IF_ELSE)\n"); break;
        case WHILE:   printf("(WHILE)\n"); break;
        case SEQ:     printf("(SEQ)\n"); break;
        case FUNCAO:  printf("(FUNCAO %s)\n", n->dado.id ? n->dado.id : "<anon>"); break;
        default:      printf("(NODE %d)\n", n->tipo); break;
    }

    printAST(n->esquerda, nivel + 1);
    printAST(n->meio,   nivel + 1);
    printAST(n->direita, nivel + 1);
}
*/

char * operadorToString(Operador op) {
    switch (op) {
        case SOMA: return "+";
        case MENOS: return "-";
        case MULTI: return "*";
        case DIVISAO: return "/";
        case ATRIBUICAO: return "=";
        case MAIOR: return ">";
        case MENOR: return "<";
        case MAIORI: return ">=";
        case MENORI: return "<=";
        case IGUAL: return "==";
        case DIFERENTE: return "!=";

        default: return "?";
    }
}

#define MAX_LINES 20
#define MAX_WIDTH 200
char canvas[MAX_LINES][MAX_WIDTH];

// 1. Calcula a altura da árvore para saber quantas linhas precisamos
int getHeight(AST* root) {
    if (!root) return 0;
    int lh = getHeight(root->esquerda);
    int rh = getHeight(root->direita);
    return (lh > rh ? lh : rh) + 1;
}

// 2. Preenche a matriz (canvas) recursivamente
void fillBuffer(AST* node, int level, int left, int right) {
    if (!node || level >= MAX_LINES) return;

    int width = right - left;
    int mid = left + width / 2;

    // Converte o dado do nó para string
    char str[10];
if (node->tipo == OP) {
    sprintf(str, "(%s)", operadorToString(node->dado.operador));
}
else if (node->tipo == ID) {
    sprintf(str, "%s", node->dado.id);
}
else if (node->tipo == VAL) {
    sprintf(str, "%d", node->dado.valor);
}
else if (node->tipo == IF) {
    sprintf(str, "IF");
}
else if (node->tipo == WHILE) {
    sprintf(str, "WHILE");
}
else if (node->tipo == RETURN) {
    sprintf(str, "RETURN");
}
else if (node->tipo == FUNCAO) {
    sprintf(str, "FUN %s", node->dado.id);
}
else if (node->tipo == PARAM) {
    sprintf(str, "PARAM %s", node->dado.id);
}
else if (node->tipo == SEQ) {
    sprintf(str, "SEQ");
}
else if (node->tipo == INT) {
    sprintf(str, "INT");
}
else if (node->tipo == VOID) {
    sprintf(str, "VOID");
}
else {
    sprintf(str, "NODE");
}

    // Calcula onde começar a escrever para ficar centralizado na posição 'mid'
    int len = strlen(str);
    int start = mid - len / 2;
    
    // Proteção para não estourar o buffer lateral
    if (start < 0) start = 0;
    if (start + len >= MAX_WIDTH) start = MAX_WIDTH - len - 1;

    // Escreve o nó no canvas
    strncpy(&canvas[level * 2][start], str, len);

    // Desenha as "pernas" para os filhos na linha seguinte (level*2 + 1)
    if (node->esquerda) {
        // Desenha o braço esquerdo '/'
        int childMid = left + (mid - left) / 2;
        // Ponto médio entre o pai e o filho
        int slashPos = (mid + childMid) / 2; 
        if (slashPos >= 0 && slashPos < MAX_WIDTH) 
             canvas[level * 2 + 1][slashPos] = '/';
             
        fillBuffer(node->esquerda, level + 1, left, mid);
    }

    if (node->direita) {
        // Desenha o braço direito '\'
        int childMid = mid + (right - mid) / 2;
        int backSlashPos = (mid + childMid) / 2;
        if (backSlashPos >= 0 && backSlashPos < MAX_WIDTH)
            canvas[level * 2 + 1][backSlashPos] = '\\';
            
        fillBuffer(node->direita, level + 1, mid, right);
    }
}

// 3. Função Principal que limpa o canvas e imprime
void printTree(AST* root) {
    // Limpa o canvas com espaços
    for (int i = 0; i < MAX_LINES; i++) {
        for (int j = 0; j < MAX_WIDTH; j++) {
            canvas[i][j] = ' ';
        }
        canvas[i][MAX_WIDTH - 1] = '\0'; // Terminador de string
    }

    int h = getHeight(root);
    fillBuffer(root, 0, 0, MAX_WIDTH);

    // Imprime apenas as linhas usadas (altura * 2, pois usamos linhas intercaladas para conectores)
    for (int i = 0; i < h * 2; i++) {
        // Remove espaços em branco sobrando à direita para o terminal ficar limpo
        int j = MAX_WIDTH - 2;
        while (j > 0 && canvas[i][j] == ' ') j--;
        canvas[i][j+1] = '\0';
        
        printf("%s\n", canvas[i]);
    }
}
