x = [-10:0.1:10];
y = [-10:0.1:10];
result = zeros(length(x), length(y));


for i = 1 : length(x)
  for j = 1 : length(y)
     result(i,j) = 2*x(i) + 1.2*y(j);
  endfor
endfor
surf(x,y,result);
title('3D surface');
xlabel('X Axis');
ylabel('Y Axis');


 