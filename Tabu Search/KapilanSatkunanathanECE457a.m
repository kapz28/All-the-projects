% Kapilan Satkunanathan 
% Question 1

% Generate Flow and Distance Arrays
filename = 'Distance.csv';
Distance = xlsread(filename);
filename = 'Flow.csv';
Flow = xlsread(filename);

% Set Up Initial Variables 
initcombination = [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20];
permutationcurr = initcombination;
flowcurr = Flow;
distancecurr = Distance;
iter =1;
neighborhood = zeros(400,3);
tabulist = zeros(20,20);
currcost=0;
optimalcost=100000;

% UNCOMMENT the below lines for each initial solution swap and run it
% to see results

% % first init solution swap 1 and 2
% permutationcurr = initcombination;
% flowcurr = Flow;
% distancecurr = Distance;
% distancecurr(:,[1 2]) = distancecurr(:,[2 1]);
% distancecurr([1 2],:)= distancecurr([2 1],:);
% permutationcurr([1 2]) = permutationcurr([2 1]);
% % 
% % % second init solution swap 2 and 18
% permutationcurr = initcombination;
% flowcurr = Flow;
% distancecurr = Distance;
% distancecurr(:,[2 18]) = distancecurr(:,[18 2]);
% distancecurr([2 18],:)= distancecurr([18 2],:);
% permutationcurr([2 18]) = permutationcurr([18 2]);
% % 
% % third init solution swap 3 and 6
% permutationcurr = initcombination;
% flowcurr = Flow;
% distancecurr = Distance;
% distancecurr(:,[3 6]) = distancecurr(:,[6 3]);
% distancecurr([3 6],:)= distancecurr([6 3],:);
% permutationcurr([3 6]) = permutationcurr([6 3]);
% % 
% % fourth init solution swap 10 and 1
% permutationcurr = initcombination;
% flowcurr = Flow;
% distancecurr = Distance;
% distancecurr(:,[1 10]) = distancecurr(:,[10 1]);
% distancecurr([1 10],:)= distancecurr([10 1],:);
% permutationcurr([1 10]) = permutationcurr([10 1]);
% 
% % fifth init solution swap 15 and 1
% permutationcurr = initcombination;
% flowcurr = Flow;
% distancecurr = Distance;
% distancecurr(:,[1 15]) = distancecurr(:,[15 1]);
% distancecurr([1 15],:)= distancecurr([15 1],:);
% permutationcurr([1 15]) = permutationcurr([15 1]);
% 
% % sixth init solution swap 20 and 1
% permutationcurr = initcombination;
% flowcurr = Flow;
% distancecurr = Distance;
% distancecurr(:,[1 20]) = distancecurr(:,[20 1]);
% distancecurr([1 20],:)= distancecurr([20 1],:);
% permutationcurr([1 20]) = permutationcurr([20 1]);
% 
% % seventh init solution swap 16 and 4
% permutationcurr = initcombination;
% flowcurr = Flow;
% distancecurr = Distance;
% distancecurr(:,[4 16]) = distancecurr(:,[16 4]);
% distancecurr([4 16],:)= distancecurr([16 4],:);
% permutationcurr([4 16]) = permutationcurr([16 4]);
% 
% % eighth init solution swap 16 and 13
% permutationcurr = initcombination;
% flowcurr = Flow;
% distancecurr = Distance;
% distancecurr(:,[13 16]) = distancecurr(:,[16 13]);
% distancecurr([13 16],:)= distancecurr([16 13],:);
% permutationcurr([13 16]) = permutationcurr([16 13]);
% 
% % ninth init solution swap 16 and 10
% permutationcurr = initcombination;
% flowcurr = Flow;
% distancecurr = Distance;
% distancecurr(:,[10 16]) = distancecurr(:,[16 10]);
% distancecurr([10 16],:)= distancecurr([16 10],:);
% permutationcurr([10 16]) = permutationcurr([16 10]);
% 
% % tenth initial soln swap 9 and 6
% permutationcurr = initcombination;
% flowcurr = Flow;
% distancecurr = Distance;
% distancecurr(:,[6 9]) = distancecurr(:,[9 6]);
% distancecurr([6 9],:)= distancecurr([9 6],:);
% permutationcurr([6 9]) = permutationcurr([9 6]);


while (iter<=14000)
    row=1;
    for i = 1:20
        for j = 1:20
            distancecurr = Distance;
            distancecurr(:,[i j]) = distancecurr(:,[j i]);
            distancecurr([i j],:)= distancecurr([j i],:);
            currcost=sum(distancecurr.*flowcurr,'all');
            neighborhood(row,1)=i;
            neighborhood(row,2)=j;
            neighborhood(row,3)=currcost+tabulist(max(i,j),min(i,j));
            row=row+1;
        end 
    end
%     %UNCOMMENT HERE to solve solution with smaller neighbourhood   
%     neighborhood(100:400,3)=10000000;  
    sortedneighborhood = sortrows(neighborhood,3);
    
    
    taburow=1;
    if(tabulist(min(sortedneighborhood(1,1),sortedneighborhood(1,2)),max(sortedneighborhood(1,1),sortedneighborhood(1,2))) > 0)
        while (tabulist(min(sortedneighborhood(taburow,1),sortedneighborhood(taburow,2)),max(sortedneighborhood(taburow,1),sortedneighborhood(taburow,2)))> 0)
            % UNCOMMENT HERE ASPIRATIONS CRITERIAS 
            % first criteria - accept best soln thus far;
%             if (sortedneighborhood(taburow,3)<optimalcost)
%                 break;
%             end
            % second criteria - narrow neighbourhood best selection
%             if (taburow==200)
%                 taburow=1;
%                 break;
%             end
            taburow=taburow+1;
        end
    end
    
    tabulist = tabulist - triu(tabulist > 0);
    swapone=min(sortedneighborhood (taburow,1),sortedneighborhood (taburow,2));
    swaptwo=max(sortedneighborhood (taburow,1),sortedneighborhood (taburow,2));
    iter;
    
    for i = 1:20
        if (permutationcurr(i)==swapone)
            index1=i;
        end
        if (permutationcurr(i)==swaptwo)
            index2=i;
        end
    end
    
    currcost = sortedneighborhood (taburow,3)-tabulist(swaptwo,swapone);
    tabulist(swapone,swaptwo)= 35;
    
    % UNCOMMENT HERE to allow for frequency based Tabu
    % tabulist(swaptwo,swapone)= tabulist(swaptwo,swapone)+1;
    
    %UNCOMMENT HERE to change tabu tenure to a dynamic one using rand
    %function
%     if (mod(iter,500)==0)
%         tabulist(swapone,swaptwo)= round(20+rand*(60-20));
%     end 
    
    Distance(:,[swapone swaptwo]) = Distance(:,[swaptwo swapone]);
    Distance([swapone swaptwo],:)= Distance([swaptwo swapone],:);
    permutationcurr([index1 index2])=permutationcurr([index2 index1]);
    
    if (currcost<optimalcost)
        best_soln_iter=iter;
        optimalcost=currcost;
        permutation_best=permutationcurr;
        % USED TO FIND ITERATION WHEN OPTIMAL IS REACHED
        %if(best_cost==2570)
%             iter
        %end
    end
    iter=iter+1;
end 

% Results
optimalcost
currcost