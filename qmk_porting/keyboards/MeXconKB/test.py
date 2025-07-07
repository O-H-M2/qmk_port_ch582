import numpy as np

def generate_exponential_numbers(count=10, min_value=1, max_value=1000, base=2, scale=1):
    """
    生成符合指数函数分布的数值列表。

    参数：
        count (int): 生成的数的数量。
        min_value (float): 生成的数的最小值。
        max_value (float): 生成的数的最大值。
        base (float): 指数函数的底数（例如：2 表示 2^x）。
        scale (float): 缩放因子 a，在 a * base^x 中。

    返回：
        list: 符合要求的指数分布数值列表（排序后）。
    """

    if base <= 0 or base == 1:
        raise ValueError("base 必须大于 0 且不等于 1。")

    # 确定 x 的取值范围，使得 a*b^x 在 [min_value, max_value] 内
    x_min = np.log(min_value / scale) / np.log(base)
    x_max = np.log(max_value / scale) / np.log(base)

    # 在 x 范围内均匀采样
    x_values = np.linspace(x_min, x_max, count)

    # 计算 y = a * b^x
    numbers = scale * (base ** x_values)

    # 四舍五入为整数（如需要）
    numbers = np.round(numbers).astype(int)

    return numbers.tolist()


# 示例调用
if __name__ == "__main__":
    result = generate_exponential_numbers(
        count=101,
        min_value=1,
        max_value=100,
        base=2,
        scale=10
    )
    print("生成的指数分布数值：")
    print(result)