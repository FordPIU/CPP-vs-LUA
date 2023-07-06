local function partition(array, low, high)
    local pivot = array[high]
    local i = low - 1

    for j = low, high - 1 do
        if array[j] < pivot then
            i = i + 1
            array[i], array[j] = array[j], array[i]
        end
    end

    array[i + 1], array[high] = array[high], array[i + 1]
    return i + 1
end

local function quickSort(array, low, high)
    if low < high then
        local pivotIndex = partition(array, low, high)
        quickSort(array, low, pivotIndex - 1)
        quickSort(array, pivotIndex + 1, high)
    end
end

local function LUAQuickSort()
    local array = cpp_toSort

    local size = #array
    local newArray = {}
    for i = 1, size do
        newArray[i] = array[i]
    end

    quickSort(newArray, 1, size)

    for i = 1, size do
        array[i] = newArray[i]
    end
end

LUAQuickSort()
