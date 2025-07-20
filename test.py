nums = [1,2,3,4,5]
target = 9
def twoSum(nums: list[int], target: int) -> list[int]:
    mem = {}
    for i, n in enumerate(nums):
        other = target - n
        if other in mem:
            return [i, mem[other]]
        mem[n] = i
    return [-1,-1]
twoSum(nums, target)
