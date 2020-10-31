function [sigma] = cross_section_interpolation(E, E_arr, sigma_arr)


% Binary search
len = length(E_arr);
range = [1,len];
j = floor((range(1)+range(2))/2);

if (E <= E_arr(end) && E >= E_arr(1))
    while not(E_arr(j) == E | (E_arr(j) <= E && E < E_arr(j + 1)))
        if E_arr(j) < E
            range(1) = j + 1;
        elseif E < E_arr(j)
            range(2) = j - 1;
        end
        j = round((range(1)+range(2))/2);
        if E_arr(j) == E
            break;
        end
    end
end

if E_arr(j) == E
    sigma = sigma_arr(j);
elseif E < E_arr(1)
    sigma = sigma_arr(1);
elseif E > E_arr(end)
    sigma = sigma_arr(end);
else
    sigma = ((sigma_arr(j + 1) - sigma_arr(j))/(E_arr(j + 1) - E_arr(j)))*(E - E_arr(j)) + sigma_arr(j);
end

end

