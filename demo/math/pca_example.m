#!/usr/bin/octave -qf
## verify the PCA method

## original data, (x_1,y_1), (x_2,y_2), (x_3,y_3), (x_4, y_4)
x = [2.1000, 8.0000; 2.5000, 12.0000; 4.0000, 14.0000; 3.6000, 10.0000];
## A = x - mean(x);
A = (x - mean(x))./var(x);
## A = x;

n = 4 - 1;
M = A' * A / n;
[u,s,v] = svd(M);
printf("M = u*s*v^T\n");
u
s
v
sigma1 = sqrt(s(1,1))
sigma2 = sqrt(s(2,2))
v1 = v(:,1);
v2 = v(:,2);
A * v1
A * v2
A*v
[uu,ss,vv] = svd(A - A * v1 * v1')

u1 = A * v1 / sigma1
u2 = A * v2 / sigma2
norm(x)
