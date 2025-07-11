def twoSum (array):
    target = 3
    sumMap = {}
    for i in range(len(array)):
        search = target - array[i]
        if search in sumMap:
            return [sumMap[search], i]
        sumMap[array[i]] = i

def foo(bar: int) -> bool:
    return bar == 2