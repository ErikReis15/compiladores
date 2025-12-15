int gcd(int u, int v)
{
if (v == 0) return u;
else return gcd(v, u - (u/v) * v);
}
void main(void)
{
int x[2]; int y;
x[0] = input();
y = x[0] + 10;
output(gcd(x[0], y));
return;
}