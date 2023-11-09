close all;
clc;

%Create the data for the 2 graphs 
X = [0: 0.1 : 7];
ysin = 5 * sin(X);
ycos = 5 * cos(X);

%The sin curve will be in blue, while cosine in red 
figure(1);
plot(X,ysin,'b');
xlabel('X');
ylabel('Y');
legend('sinus');
title('Sinus Graph Only');


figure(2);
plot(X,ycos,'r');
xlabel('X');
ylabel('Y');
legend('cosinus');
title('Cosinus Graph Only');


