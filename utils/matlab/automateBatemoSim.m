% Define column names for the table
colNames = {'V', 'I', 'SOC', 'T_surf'};

% Setup Simulation
set_param('Batemo_Model', 'StopTime', '7200')

% Initialize an empty table with column names
T = table();

% Values to change per simulations
Time = 10000;

% Define SOH data - % Aged (from 100-80% SOH)
SOH = 16:1:5047;

% Define Current data
Current_data = -40:1:40;

% Start Timer
tic;

% Nested for loop for iterating through all values of SOH and current for
% each simulation
for i = 1:length(SOH)
    for j = 1:length(Current_data)
        % Set aging of the cell prior to each simulation
        set_param('Batemo_Model/Molicel_INR21700P45B_221c01110','k_C_li', num2str(SOH(i)));
    
        % Select Current to Test
        Current = Current_data(j);

        if Current < 0
            % Set SOC to 100% if Current is less than 0 - Discharge Cycle
            set_param('Batemo_Model/Molicel_INR21700P45B_221c01110', 'SOC_0', '100');
        else
            % Set SOC to 0% if Current is greater than or equal to 0 -
            % Charge Cycle
            set_param('Batemo_Model/Molicel_INR21700P45B_221c01110', 'SOC_0', '0');
        end
    
        % Run Simulation with value of I, SOC
        try
            sim('Batemo_Model')
            disp(['Simulation #' num2str(i*j) '/' num2str(length(Current_data)*length(SOH)) ' succeeded for SOH of ' num2str(100 - SOH(i)) '!'])
    
            % Append Charge and Discharge Simulations to Table using vertcat()
            simulationResults = struct();
            simulationResults.V = ans.simout.V.Data; 
            simulationResults.I = ans.simout.I.Data; 
            simulationResults.SOC = ans.simout.SOC.Data; 
            simulationResults.T_surf = ans.simout.T_surf.Data; 
            simulationResults.SOH = ones(size(ans.simout.V.Data))* (100 - SOH(i));

            % Create a table from simulation results
            simulationTable = struct2table(simulationResults, 'AsArray', false);
            
            % Append the table to the main table
            T = vertcat(T, simulationTable);

        catch
            % Capture the error message
            errorMessage = lasterr;
            
            % Display the error message
            disp(['Simulation #' num2str(i*j) '/' num2str(length(Current_data)*length(SOH)) ' failed for SOH of ' num2str(100 - SOH(i)) '!'])
            disp(errorMessage)
            
            % Continue to the next iteration or add further error handling logic if needed
            continue;
        end
    end
end

% Stop Timer
elapsed = toc;

% Display Elapsed Time
disp(['Elapsed Time: ' num2str(elapsed/3600) ' hrs']);

% Write the final table to a CSV file (append if the file exists)
if exist('batemo_model_data.csv', 'file') == 2
    % File exists, append the table
    writetable(T, 'batemo_model_data.csv', 'WriteMode', 'append');
else
    % File doesn't exist, create a new file
    writetable(T, 'batemo_model_data.csv');
end
