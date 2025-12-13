int gcd(int u, int v)
{
int x;
if (v == 0) return u;
else return gcd(v, u - (u/v) * v);
}

/*Lex comentário :D*/

void main(void)
{
void x;
output(gcd());
}
