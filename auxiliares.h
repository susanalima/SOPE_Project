/**
*@brief Decompoem um array de characteres pelo seu delimitador
*
*@param o_que_separar O array que eu quero separar
*@param delimitador O delimitador que vai ser usado para o array que quero separar
*@param guardar Onde eu vou guardar. É um array de array de caracteres ou um array de strings
*
*@return Retorna a quantidade de separacoes que houve ou -1 se ocorreu um erro
*
*@note O parametro guardar deve ser do tipo char** com memoria já previamente alocada(malloc(sizeof(*guardar)))
*/
int separarPor(const char *o_que_separar,const char* delimitador,char*** guardar);
/**
*@brief Ordena um array de strings pelo algoritmo de merge sort
*
*@param array O array de string que eu quero ordenar. Deve ser passado como referencia
*@param tamanho Tamanho do array de strings
*
*/
void mergeSortArrayString(char*** array,int tamanho);
/**
*@brief Procura no array de string pela string pretendida
*
*@param array O array onde eu vou querer procurar
*@param tamanho O tamanho do array de strings
*@param a_procurar A string que eu quero procurar no array
*
*@return Retorna o indice onde se encontra a string que quer procurar (0-tamanho) ou -1 se não encontrar
*
*/
int binarySearchArrayString(char** array,int tamanho,char* a_procurar);
