/*
int gcd(int u, int v)
{
    int x;
    if (v == 0) return u;
    else return gcd(v, u - (u/v) * v);
}
*/
int funcao(int x, int y){
    x = x + 2;
    return x + 1 + x / 3;
}

int teste2(int x, int y, int z){
    return x;
}

/*Lex comentário :D*/

void main(void)
{
int x;
int y;
/*y = 2 * 1 + 3 / 5 -2;*/


/*
output(gcd(x[0], y + 2 + x));
*/
/*
output(teste(x, y));
*/
x = funcao(10 + funcao(4+x)) +funcao(1,4,0);
x = funcao(10 - x, y-3+5, 10) + 3 +funcao(1,4,0);
y = teste(x+2, 5 + teste(x+teste(y)) + 4);
return 1 + 2;
}
