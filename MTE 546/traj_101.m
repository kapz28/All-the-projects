close all
clear
clc
%%
%Simulation Setup

%state vector = [Xx Xy Vx Vy]'
%Sampling time
dt = 0.01;
Tf = 18;
T = 0:dt:Tf;
len = length(T);

%state storage
X = zeros(4,len+1);
%measurement Storage
Y = zeros(2,len);
%gyro Storage
Z = zeros(2,len);

%System model noise
Q = 1e-3*eye(4);
%Sensor model noise 
%using covariance values obtained for lab 1
R = [0.2 0 ; 0 0.02];
% gyro sensor noise
S = [0.001 0.001];

A = [1 0 dt 0; 0 1 0 dt; 0 0 1 0; 0 0 0 1];


init = [0 0 10 1]';

%%
% Assume start with x velocity of 2 y of 1   

X(:,1) = init;
Z(:,1) = [0 0]';
Pk = eye(4);

%{
start velocity : [2 , 1]
end velocity : [1 , 2]
%}

%% Simulate the sensor model 
for i=1:len
    %Simulating Object Motion and Sensors
    Xk = X(:, i);
    %angle
    Zk = Z(:, i);
    %
%     if i*dt> 3.5
%         Xk = [Xk(1) Xk(2) 2 1]';
%     end
%     if i*dt>7
%         Xk = [Xk(1) Xk(2) 1 2]';
%     end
    %}
    w = [Q(1,1)*randn(1);Q(2,2)*randn(1);Q(3,3)*randn(1);Q(4,4)*randn(1)];
    v = [R(1,1)*randn(1);R(2,2)*randn(1)];
    u = [S(1)*randn(1);S(2)*randn(1)];
    % for first 3 seconds gyro model z axis is 0 plus gaussian noise
    % when rotation happens gyro model kicks in or from 3.5 to time till
    % gyro reads 90 degrees
    % continue zero gryo readings plus gaussian noise
    
    %
    if i*dt>3.5 && Zk(1)<(pi/2)
        Zk1 = Zk+u+[0.4*dt; 0.4];
        Xk = [Xk(1) Xk(2) 9*cos(Zk(1))+1 9*sin(Zk(1))+ 1]';
    elseif i*dt > 3.5 && Zk(1)>(pi/2)
        Xk = [Xk(1) Xk(2) init(4) init(3)]';
        Zk1 = Zk+u;
    else
        %Xk = [Xk(1) Xk(2) 2 1]';
        Zk1 = Zk+u;
    end
    
    if i*dt>12 && Zk(1) > 0        
        Zk1 = Zk+u+[-0.4*dt; -0.4];
        Xk = [Xk(1) Xk(2) 9*cos(Zk(1))+1 9*sin(Zk(1))+1]';
    elseif i*dt > 12 && Zk(1)<0
        Xk = [Xk(1) Xk(2) init(3) init(4)]';
        Zk1 = Zk+u;
    end
     %}
    
    Yk = h(Xk)+v;
    
    Xk1 = A*Xk + w;
    Yk1 = h(Xk1) + v;
    
    % store true state
    X(:, i+1) = Xk1;
    Y(:, i+1) = Yk1;
    Z(:, i+1) = Zk1;
end

%%
% Simulated truth and sensor data
x = X(1, :);
y = X(2, :);
vx = X(3, :);
vy = X(4, :);
ax = Y(1, :);
ay = Y(2, :);

X2 = zeros(4,len+1);

X2(:,1) = init;

Z2 = zeros(2,len);
Z2(:,1) = [0 0]';
KalmanGain = zeros(2,len);
KalmanGain(:,1) = [0]';
%Start the EKF
for i =1:len    
    Zk2 = Z2(:,i);
    Xk2 = X2(:,i);

   %simulate 90 degree rotation
    if i*dt>3.5 && Zk2(1)<(pi/2)
        Zk3 = Zk2+u+[0.4*dt; 0.4];
        Xk2 = [Xk2(1) Xk2(2) 9*cos(Zk2(1))+1 9*sin(Zk2(1))+1]';
    elseif i*dt > 3.5 && Zk(1)>(pi/2)
        Xk2 = [Xk2(1) Xk2(2) init(4) init(3)]';
        Zk3 = Zk2+u;
    else
        %Xk2 = [Xk2(1) Xk2(2) 2 1]';
        Zk3 = Zk2+u;
    end
    
    %simulated 90 degree rotation back to original position
    if i*dt>12 && Zk2(1) > 0        
        Zk3 = Zk2+u+[-0.4*dt; -0.4];
        Xk2 = [Xk2(1) Xk2(2) 9*cos(Zk2(1))+1 9*sin(Zk2(1))+1]';
    elseif i*dt > 12 && Zk(1)<0
        Xk2 = [Xk2(1) Xk2(2) init(3) init(4)]';
        Zk3 = Zk2+u;
    end
    %}
    %"instant rotation" change in axis velocity input
%     if i*dt> 5
%         Xk2 = [Xk2(1) Xk(2) 0 1]';
%     end
%     
    
    % Predict
    Xhat = A*Xk2;
    Yhat = h(Xk2);
    
    Pk = A*Pk*A' + Q;
    Hk = H(Xk2);
    
    % Sensor data
    Yk = [ax(i); ay(i)];
    
    % Correct the State
    K = (Pk*Hk')*inv(Hk*Pk*Hk' + R);

    KalmanGain(:, i+1) = abs(K(3,1)+K(4,2))/2;
    Xhat = Xhat + K*(Yk - Yhat);
    Pk = (eye(4) - K*Hk)*Pk;
    %4by2 *(2*1)
    
    % store corrected state
    X2(:, i+1) = Xhat;
    Z2(:, i+1) = Zk3;
end

%% Plot results
X2x = X2(1, :);
Xx = X(1, :);
X2x(end)=[];
Xx(end)=[];

vx = X2(3,:);
vx(end) =[];
vx2 = X(3,:);
vx2(end) =[];

vy = X2(4,:);
vy(end) =[];
vy2 = X(4,:);
vy2(end) =[];


Y2y = X2(2, :);
Yy = X(2, :);
Y2y(end)=[];
Yy(end)=[];

% EKF
figure(1);
%

subplot(4,1,1);
%
plot(T, X2x, T, Xx, '-');

title("Position")
xlabel('Time [s]');
ylabel('x position [cm]');
legend('EKF', 'True');
grid on;

subplot(4,1,2);
plot(T, Y2y, T, Yy, '-');
% plot(T(1,370:1000),Y2y(1, 370:1000));
%plot([350:700], Y2y(1, [350:700]), T, Yy(1, [350:700]), '-');
xlabel('Time [s]');
ylabel('y position [cm]');
legend('EKF', 'True');



subplot(4,1,3);
plot(T, vx, T, vx2, '-');

title("Velocity")
xlabel('Time [s]');
ylabel('Velocity x [cm/s]');
legend('EKF', 'True');

subplot(4,1,4);
plot(T, vy, T, vy2, '-');
xlabel('Time [s]');
ylabel('Velocity y [cm/s]');
legend('EKF', 'True');

%{
subplot(5,1,5);
omega = Z(1,:);
omega(end) =[];
plot(T, rad2deg(omega), '-');

title('Orientation');
xlabel('time (s)');
ylabel(' angle (deg)');

%}

%Fuzzy logic 
%atan2(X(4,800)-1,X(3,800)-1) maybe use for later
%Z(1,800)

%Z(1,i)*(1-Kalman gain) avage with (Kalman Gain)*atan(X(4,i)-1,X(3,i)-1)
input = zeros(1,len);
output = zeros(1,len);
rotornot = zeros(1,len);
deltaarr = zeros(1,len);
for i =1:len
    Xk5 = X(:,i);
    input(i) = 180/pi*(KalmanGain(i)*Z(1,i)+ (1-KalmanGain(i))*atan2((Xk5(4)-1),(Xk5(3)-1)));
    if i > 1
        deltatheta = (input(i)-input(i-1));
    else
        deltatheta = (input(i)-0);
    end
    
    valnotrot = O(abs(deltatheta));
    valrot = G(abs(deltatheta));
    
    maxval = max([valrot,valnotrot]);
    
    if maxval == valrot
        bool = 1;
    else
        bool = 0;
    end
    
    if bool
        output(i) = deltatheta+input(i);
    else
        output(i) = input(i);
    end
    rotornot(i) = bool;
    deltaarr(i) = deltatheta;
end
%
%plot error and fuzzy logic stuff
subplot(3,1,1);
%
omega = Z(1,:);
omega(end) =[];
plot(T, output,T, rad2deg(omega), '-');

title('Orientation');
legend('Post FLA', 'Pre FLA');
xlabel('Time [s]');
ylabel('deg');
grid on;
subplot(3,1,2);
plot(T, rotornot, '-');
title('Rotation Status')
xlabel('Time [s]');
ylabel('Status');
ylim([-0.1 1.1]);

subplot(3,1,3);
plot(T, abs(deltaarr), '-');
%}

%%
%Using the first provided inverse equation 
function Y = h(X)
    x = X(1);
    y = X(2);
    Y = [(8.3741*x + 0.2395)./(x + 0.0123); ...
         (8.3558*y + 1.3344)./(y + 0.1294)];
end

function J = H(X)
    x = X(1);
    y = X(2);
    J = [8.3741./(x + 0.0123) - (8.3741*x + 0.2395)./(x + 0.0123).^2, 0, 0, 0;
         0, 8.3558./(y + 0.1294) - (8.3558*y + 1.3344)./(y + 0.1294).^2, 0, 0];
end

function notrotating = O(X)
    if X <= 0.01
        notrotating = 1;
    elseif X > 0.01 && X < 0.1
        notrotating = (0.1-X)/(0.1-0.01);
    else
        notrotating = 0;
    end
end

function rotating = G(X)
    if X <= 0.1
        rotating = 0;
    elseif X > 0.1 && X < 0.13
        rotating = (X-0.1)/(0.13-0.1);
    else
        rotating = 1;
    end
end
%}