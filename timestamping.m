clear all;

t1 = csvread('timestamping-method-1.txt');
t2 = csvread('timestamping-method-2.txt');

dt1 = t1 - 100;
dt2 = t2 - 100;

dt1_cleared = dt1(dt1 <= 0.5);
dt2_cleared = dt2(abs(dt2) <= 0.5);

figure();
subplot(2, 1, 1);
plot(dt1);
title('Method-1');
xlabel('samples')
ylabel('ms')
subplot(2, 1, 2);
plot(dt2);
title('Method-2');
xlabel('samples')
ylabel('ms')

figure();
subplot(2, 1, 1);
hist(dt1, 40);
title('Method-1');
xlabel('ms')
subplot(2, 1, 2);
hist(dt2, 40);
title('Method-2');
xlabel('ms')

figure();
subplot(2, 1, 1);
hist(dt1_cleared, 20);
title('Method-1');
xlabel('ms')
subplot(2, 1, 2);
hist(dt2_cleared, 20);
title('Method-2');
xlabel('ms')

max(abs(dt1))
max(abs(dt2))

min(abs(dt1))
min(abs(dt2))

mean(dt1)
mean(dt2)
mean(abs(dt2))

median(dt1)
median(dt2)

std(dt1)
std(dt2)