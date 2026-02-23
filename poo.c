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
double standardDeviation(double x[], int n);
double* createSortedCopy(const double x[], int n);
struct statistics getStatistics(double x[], int n);
double median(double x[], int n);
double q1(double x[], int n);
double q3(double x[], int n);
double iqr(double x[], int n);
double range(double x[], int n);


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

    //new function for individual calculation combined

    if(strncmp(input, "mean", 4) == 0 || strncmp(input, "stdev", 5) == 0 || strncmp(input, "med", 3) == 0 || strncmp(input, "median", 6) == 0 || strncmp(input, "q1", 2) == 0 || strncmp(input, "q3", 2) == 0 || strncmp(input, "iqr", 3) == 0 || strncmp(input, "range", 2) == 0){
      int length;
      printf("How long is your dataset? ");
      if (scanf("%d", &length) != 1 || length <= 0) {
          printf("Invalid length\n");
          while (getchar() != '\n'); // clear input buffer
          continue;
      }

      double *data = malloc(length * sizeof(double));
      if (!data) {
          printf("Memory allocation failed\n");
          continue;
      }

      for (int i = 0; i < length; i++) {
          printf("Enter number %d: ", i+1);
          if (scanf("%lf", &data[i]) != 1) {
              printf("Invalid input\n");
              free(data);
              while (getchar() != '\n');
              continue;
          }
      }

      if(strncmp(input, "mean", 4) == 0){
        printf("The mean of that dataset is %.2f\n", mean(data, length));
      }else if(strncmp(input, "stdev", 5) == 0){
        printf("The standard deviation of that dataset is %.2f\n", standardDeviation(data, length));
      }else if(strncmp(input, "q1", 2) == 0){
        printf("The first quartile of that dataset is %.2f\n", q1(data, length));
      }else if(strncmp(input, "q3", 2) == 0){
        printf("The third quartile of that dataset is %.2f\n", q3(data, length));
      }else if(strncmp(input, "iqr", 3) == 0){
        printf("The inter quartile range of that dataset is %.2f\n", iqr(data, length));
      }else if(strncmp(input, "range", 2) == 0){
        printf("The range of that dataset is %.2f\n", range(data, length));
      }else{
        printf("The median (2nd quartile) of that dataset is %.2f\n", median(data, length));
      }

      free(data);  // very important!s
    }

    if(strncmp(input, "stats", 5) == 0){
      int length;
      printf("How long is your dataset? ");
      if (scanf("%d", &length) != 1 || length <= 0) {
          printf("Invalid length\n");
          while (getchar() != '\n'); // clear input buffer
          continue;
      }

      double *data = malloc(length * sizeof(double));
      if (!data) {
          printf("Memory allocation failed\n");
          continue;
      }

      for (int i = 0; i < length; i++) {
          printf("Enter number %d: ", i+1);
          if (scanf("%lf", &data[i]) != 1) {
              printf("Invalid input\n");
              free(data);
              while (getchar() != '\n');
              continue;
          }
      }

      struct statistics x;
      x = getStatistics(data, length);

      printf("**********************************\n");
      printf("Datapoints: ");
      for(int i = 0;  i < length - 1; i++){
        printf("%.2f, ", x.datapoints[i]);
      }
      printf("%.2f\n", x.datapoints[x.n-1]);
      printf("Number of points: %d\n", x.n);
      printf("Range: %.2f\n", x.range);
      printf("Mean: %.2f\n", x.mean);
      printf("Median: %.2f\n", x.median);
      printf("Lower quartile: %.2f\n", x.q1);
      printf("Upper quartile: %.2f\n", x.q3);
      printf("Inter quartile range: %.2f\n", x.iqr);
      printf("Standard deviation: %.2f\n", x.stdev);
      printf("**********************************\n");
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

double standardDeviation(double x[], int n){
  double y = 0;
  double m = mean(x, n);
  for(int i = 0; i < n; i++){
    y += pow((x[i]-m), 2);
  }
  return sqrt(y/(n - 1));
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
              double temp = ret[j];
              ret[j] = ret[j + 1];
              ret[j + 1] = temp;
          }
      }
    }

    return ret;
}

double median(double x[], int n) {
    double* sorted = createSortedCopy(x, n);
    if (!sorted) return 0.0; // or handle error

    double result;
    if (n % 2 == 1) {
        result = sorted[n/2];
    } else {
        result = (sorted[n/2 - 1] + sorted[n/2]) / 2.0;
    }

    free(sorted);
    return result;
}

double q1(double x[], int n){
  double* y = createSortedCopy(x, n);
  if (!y) return 0.0;

  if (n < 4) {
      printf("An array must have at least 4 values to find the quartiles\n");
      free(y);
      return 0.0;
  }

  double pos = (n + 1.0) / 4.0;
  int lower = (int)pos - 1;           // 0-based index
  double frac = pos - (lower + 1.0);

  double ret;
  if (frac < 1e-9) {                  // almost exact integer position
      ret = y[lower];
  } else {
      ret = y[lower] + frac * (y[lower + 1] - y[lower]);
  }

  free(y);
  return ret;
}

double q3(double x[], int n){
  double* y = createSortedCopy(x, n);
  if (!y) return 0.0;

  if (n < 4) {
      printf("An array must have at least 4 values to find the quartiles\n");
      free(y);
      return 0.0;
  }

  double pos = 3.0 * (n + 1.0) / 4.0;
  int lower = (int)pos - 1;           // 0-based index
  double frac = pos - (lower + 1.0);

  double ret;
  if (frac < 1e-9) {
      ret = y[lower];
  } else {
      ret = y[lower] + frac * (y[lower + 1] - y[lower]);
  }

  free(y);
  return ret;
}

double iqr(double x[], int n){
  double Q1 = q1(x, n);
  double Q3 = q3(x, n);
  return Q3 - Q1;
}

double range(double x[], int n){
  double* y = createSortedCopy(x, n);
  if (!y) return 0.0;

  return y[n-1] - y[0];
}

struct statistics getStatistics(double x[], int n){
  struct statistics y;
  y.n = n;
  memcpy(y.datapoints, createSortedCopy(x, n), sizeof(double) * n);
  y.mean = mean(x, n);
  y.median = median(x, n);
  y.q1 = q1(x, n);
  y.q3 = q3(x, n);
  y.iqr = y.q3 - y.q1;
  y.range = y.datapoints[n - 1] - y.datapoints[0];
  y.stdev = standardDeviation(x, n);
  return y;
}