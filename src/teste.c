int gcd(int u, int v)
{
int x;
if (v == 0) return u;
else return gcd(v, u - (u/v) * v);
}

/*Lex comentário :D*/

void main(void)
{
int x; int y;
x = 1;
y = 2;
if(x > 2){
    int z;
    y = 3;
}
output(gcd(x, y));
}
