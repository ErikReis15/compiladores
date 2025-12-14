/*
int gcd(int u, int v)
{
    int x;
    if (v == 0) return u;
    else return gcd(v, u - (u/v) * v);
}
*/
int teste(int x, int y){
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
y = teste(x+2, 4+1+1, y);
*/
y = teste(teste1(x), teste(x, y), 3+2, teste(x, y));
return 1 + 2;
}
