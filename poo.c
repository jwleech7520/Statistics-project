#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

struct statistics {
  double datapoints[256];
  int n;
  double mean;
  double median;
  double q1;
  double range;
  double q3;
  double iqr;
  double stdev;
};

double mean(double z[], int n);
double standardDeviation(int x[], int n);
double* createSortedCopy(const double x[], int n);
struct statistics getStatistics(double x[], int n);
double median(double x[], int n);


int main(){
  while (1) {
    char input[128];
    printf("> ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        break;  // EOF
    }
    input[strcspn(input, "\n")] = '\0';  // remove trailing newline

    if(strcmp(input, "exit") == 0){
      break;
    }

    if (strncmp(input, "mean", 4) == 0) {
      int length;
      printf("How long is your dataset? ");
      if (scanf("%d", &length) != 1 || length <= 0) {
          printf("Invalid length\n");
          while (getchar() != '\n'); // clear input buffer
          continue;
      }

      int *data = malloc(length * sizeof(int));
      if (!data) {
          printf("Memory allocation failed\n");
          continue;
      }

      for (int i = 0; i < length; i++) {
          printf("Enter number %d: ", i+1);
          if (scanf("%d", &data[i]) != 1) {
              printf("Invalid input\n");
              free(data);
              while (getchar() != '\n');
              continue;
          }
      }

      printf("The mean of that dataset is %.2f\n", mean(data, length));

      free(data);  // very important!
    }

    if (strncmp(input, "stdev", 5) == 0) {
      int length;
      printf("How long is your dataset? ");
      if (scanf("%d", &length) != 1 || length <= 0) {
          printf("Invalid length\n");
          while (getchar() != '\n'); // clear input buffer
          continue;
      }

      int *data = malloc(length * sizeof(int));
      if (!data) {
          printf("Memory allocation failed\n");
          continue;
      }

      for (int i = 0; i < length; i++) {
          printf("Enter number %d: ", i+1);
          if (scanf("%d", &data[i]) != 1) {
              printf("Invalid input\n");
              free(data);
              while (getchar() != '\n');
              continue;
          }
      }

      printf("The mean of that dataset is %.2f\n", standardDeviation(data, length));

      free(data);  // very important!
    }
  }
}

double mean(double z[], int n){
  double total = 0;
  for( int i = 0; i < n; i++){
    total += z[i];
  }
  return (double) total/n;
}

double standardDeviation(int x[], int n){
  double y = 0;
  double m = mean(x, n);
  for(int i = 0; i < n; i++){
    y += pow((x[i]-m), 2);
  }
  return sqrt(y/n);
}

double* createSortedCopy(const double x[], int n) {
    double* ret = malloc(n * sizeof(double));
    if (!ret) return NULL;

    for (int i = 0; i < n; i++) {
        ret[i] = x[i];
    }

    for (int i = 0; i < n - 1; i++) {
      for (int j = 0; j < n - i - 1; j++) {
          if (ret[j] > ret[j + 1]) {
              int temp = ret[j];
              ret[j] = ret[j + 1];
              ret[j + 1] = temp;
          }
      }
    }

    return ret;
}

double median(double x[], int n){
  double y[256];
  memcpy(y, createSortedCopy(x, n), n*sizeof(double));
  if(n % 2 == 0){
    
  }
}

struct statistics getStatistics(double x[], int n){
  struct statistics ret;
  ret.n = n;
  memcpy(ret.datapoints, createSortedCopy(x, n), sizeof(double) * n);
  ret.mean = mean(x, n);
}