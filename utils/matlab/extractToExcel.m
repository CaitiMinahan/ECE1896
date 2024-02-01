% Define column names for the table
colNames = {'V', 'V_ocv', 'V_loss', 'I', 'SOC', 'Q', 'T_Surf', 'J_Surf', 'T_inner', 'signal___'};

% Initialize an empty table with column names
T = table();

for index = 1:110
    % Extract data from simout
    data = {
        simout{1, index}.simout.V.Data,
        simout{1, index}.simout.V_ocv.Data,
        simout{1, index}.simout.V_loss.Data,
        simout{1, index}.simout.I.Data,
        simout{1, index}.simout.SOC.Data,
        simout{1, index}.simout.Q.Data,
        simout{1, index}.simout.T_surf.Data,
        simout{1, index}.simout.J_surf.Data,
        simout{1, index}.simout.T_inner.Data,
        simout{1, index}.simout.signal___.Data
    };
    
    % Create a temporary table for the current iteration
    k = table(data{:});
    
    % Append k to T
    T = vertcat(T, k);
end

% Write the final table to a CSV file
writetable(T, 'model_data.csv');
