int gcd(int u, int v)
{
if (v == 0) return u;
else return gcd(v, u - (u/v) * v);
}

/*Lex comentário :D*/

void main(void)
{
int x[4];
int y;
y = 2;
x[0] = 10; 
x[1] = 8;
x[2] = 6;
x[3] = 4;
y = 2;
output(gcd(x[0], y));
}
