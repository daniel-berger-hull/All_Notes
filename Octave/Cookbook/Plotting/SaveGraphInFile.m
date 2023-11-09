close all;
clc;

%Create the data for the 2 graphs 
X = [0: 0.1 : 7];
ysin = 5 * sin(X);
ycos = 5 * cos(X);

%The sin curve will be in blue, while cosine in red 
plot(X,ysin,'b');
hold on;
plot(X,ycos,'r');

xlabel('X');
ylabel('Y');

legend('sinus','cosinus');
title('Graph Saving');

%Create a PNG file format with the graph
print -dpng 'graph.png';


print -djpg 'graph.jpg';
