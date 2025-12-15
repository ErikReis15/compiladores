int factorialrec(int n) {
  if (n < 2) return 1;
  else return n * factorialrec(n-1);
}

int factorialiter(int n) {
  int result;
  result = 1;
  while (n > 1) {
    result = result * n;
    n = n - 1;
  }
  return result;
}

void main(void) {
  int x; int y; int z;
  x = input();
  y = factorialiter(x);
  z = factorialrec(x);
  return;
}