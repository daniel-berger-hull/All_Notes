close all;
clc;

%Create the data for a simple linear and power 2  graph 
X = [0: 0.1 : 7];
y = 2 * X;
y2 = pow2(X);
ysin = 5 * sin(X);

%The linear function will be in red 
plot(X,y,'r');
%force the next plot to be on the same graph
hold on;
%The sin curve will be draw on the same graph, in blue; 
plot(X,ysin,'b');

xlabel('X');
ylabel('Y');

legend('linear','sinus');
title('Multi Graph Demo')

hold off;

