int gcd(int u, int v)
{
int x;
if (v == 0) return u;
else return gcd(v, u - (u/v) * v);
}

/*Lex comentário :D*/

void main(void)
{
int x;
int y;
y = 2;
y = x + y + 1;

output(gcd(x[0], y + 2 + x));
}
