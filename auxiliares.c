#include "librarias.h"
int separarPor(const char *o_que_separar,const char* delimitador,char***guardar)
{
  free(*guardar);
  char *separado;
  int i=1;
  *guardar=malloc(sizeof(**guardar));
  if(*guardar==NULL)
  {
    return -1;
  }
  char str[strlen(o_que_separar)];
  char s[strlen(delimitador)];
  strcpy(str,o_que_separar);
  strcpy(s,delimitador);
  separado= strtok(str, s);
  while(separado!=NULL)
  {
    *guardar=realloc(*guardar,sizeof(**guardar)*i);
    if(*guardar==NULL)
    {
      return -1;
    }
    (*guardar)[i-1]=malloc(sizeof(char)*strlen(separado));
    if((*guardar)[i-1]==NULL)
    {
      return -1;
    }
    strcpy((*guardar)[i-1],separado);
    separado=strtok(NULL,delimitador);
    i++;

  }
  return i-1;
}

void merge(char*** array,char *** tmpArray,int l,int m,int r)
{
  int esquerdaEnd = m - 1;
  int tmpPos = l;
  int numElements = r - l + 1;
  while( l <=esquerdaEnd && m <= r)
  {
    if (strcmp((*array)[l],(*array)[m])<=0) {
      strcpy((*tmpArray)[tmpPos++],(*array)[l++]);
    } else {
      strcpy((*tmpArray)[tmpPos++],(*array)[m++]);
    }
  }

  while (l <= esquerdaEnd) {
    strcpy((*tmpArray)[tmpPos++],(*array)[l++]);
  }

  while (m <= r) {
    strcpy((*tmpArray)[tmpPos++],(*array)[m++]);
  }

  for(int i = 0; i< numElements ;i++, r--)
  {
    strcpy((*array)[r],(*tmpArray)[r]);
  }
}

void mergeSort(char*** array,char *** tmpArray, int l,int r)
{
  if (l < r)
  {
    int m = (l+r)/2;

    mergeSort(array,tmpArray, l, m);
    mergeSort(array,tmpArray, m+1, r);

    merge(array,tmpArray, l, m+1, r);
}
}

void mergeSortArrayString(char*** array,int tamanho)
{
  char** tempArray=malloc(sizeof(*tempArray)*tamanho);
  for(int i=0;i<tamanho;i++)
  {
    tempArray[i]=malloc(sizeof(char)*1000);
  }
  mergeSort(array,&tempArray,0,tamanho-1);
  free(tempArray);
}


int binarySearchArrayString(char** array,int tamanho,char* a_procurar)
{
  int esquerda=0 , direita = tamanho-1;
  while(esquerda <= direita)
  {
    int meio = (esquerda + direita) / 2;
    if(strcmp(array[meio],a_procurar)<0)
    {
      esquerda=meio+1;
    }
    else if ( strcmp(a_procurar,array[meio])<0 )
    {
      direita=meio -1;
    }
    else
    {
      return meio;
    }
  }
  return -1;
}

int searchArrayString(char** array,int tamanho,char* a_procurar)
{
  for(int i=0;i<tamanho;i++)
  {
    if(strstr(array[i],a_procurar)!=NULL)
    {
      return i;
    }
  }
  return -1;
}
