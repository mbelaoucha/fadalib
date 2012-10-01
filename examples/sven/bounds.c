int f();
int g();
void h(int, int, int);

void foo()
{
	int i, j;
  int N, M;
  int a[100][100];

  N = f();
  M = g();
  for (i = 0; i < N; ++i)
    for (j = 0; j < M; ++j)
      a[i][j] = i + j;

  N = f();
  for (i = 0; i < N; ++i)
    for (j = 0; j < M; ++j)
      h(i, j, a[i][j]);
}

/*END OF FILE*/
