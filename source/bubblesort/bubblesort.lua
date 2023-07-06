local n = #cpp_toSort
local swapped

for i = 1, n - 1 do
    swapped = false
    for j = 1, n - i do
        if cpp_toSort[j] > cpp_toSort[j + 1] then
            local temp = cpp_toSort[j]
            cpp_toSort[j] = cpp_toSort[j + 1]
            cpp_toSort[j + 1] = temp
            swapped = true
        end
    end

    if not swapped then
        break
    end
end
