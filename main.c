#include "librarias.h"
#include "auxiliares.h"

int main(int argc, char const *argv[]) {
  char **guardar=malloc(sizeof(*guardar));
  int quantidade;
  quantidade=separarPor("ola eu sou ll"," ",&guardar);
  mergeSortArrayString(&guardar,quantidade);
  for(int i=0;i<quantidade;i++)
  {
    printf("%s\n",guardar[i]);
  }
  printf("%d\n",binarySearchArrayString(guardar,quantidade,"ola")+1);
  printf("%d\n",searchArrayString(guardar,quantidade,"la")+1);
  return 0;
}
