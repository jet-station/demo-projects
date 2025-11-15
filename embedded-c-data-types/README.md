# Embedded C Data Types

🎯 Embedded C programming relies on a variety of data types to efficiently manage memory and interact with hardware. Understanding these data types is essential for writing reliable and portable embedded software.

<!-- Images Placeholder -->
<img src="imgs/embedded-c-data-types.png" alt="Embedded C Data Types"/>
<!-- Add more images as needed -->

## Common Data Types in C

- **char**: Used for storing character data, typically 8 bits.
- **int**: Used for integer values. Size may vary depending on the microcontroller architecture (commonly 16 or 32 bits).
- **short**: A smaller integer type, often 16 bits.
- **long**: A larger integer type, often 32 bits or more.
- **float**: Used for single-precision floating-point numbers (usually 32 bits).
- **double**: Used for double-precision floating-point numbers (usually 64 bits).
- **unsigned types**: Variants of the above types that only store non-negative values, useful for hardware registers and bit manipulation.

To design robust embedded software using data types, the following key points should be considered.

## Key Points When Using Data Types in Embedded Systems

1. **Data Size**: Always consider the size of each data type, as it directly impacts memory usage and performance. Using a larger data type than necessary can waste valuable resources, while using a smaller type may lead to overflow or data loss.

2. **Naming Convention**: Use clear and consistent naming conventions for variables, especially when dealing with hardware registers or specific bit-widths (e.g., `uint8_t`, `int16_t`). This improves code readability and helps prevent errors related to type mismatches.

3. **Floating Point Data Types**: Be cautious when using floating point types (`float`, `double`) in embedded systems. Many microcontrollers lack hardware support for floating point operations, which can result in slower execution and increased code size. Prefer integer math when possible for better efficiency.

4. **Alignment and Padding**: Some microcontrollers require data to be aligned in memory. Misaligned data can lead to inefficient access or even faults. Be aware of how your compiler handles alignment and padding, especially in structs.

5. **Portability**: Data type sizes may differ between platforms. Use standard types from `stdint.h` (such as `uint8_t`, `int16_t`) to ensure your code is portable and behaves consistently across different hardware.

6. **Volatile Keyword**: Use the `volatile` keyword for variables that can change unexpectedly, such as hardware registers or interrupt flags. This prevents the compiler from optimizing out important reads and writes.

7. **Endianness**: Be aware of byte order (big-endian vs. little-endian) when handling multi-byte data, especially for communication protocols or data exchange between different systems.

8. **Bit-fields**: Bit-fields in structs allow efficient access to individual bits, which is useful for hardware control. However, their behavior can be compiler-dependent, so use them with caution and verify their layout.


## Practice
💡 For example, when choosing a data type to store the age of a person, I follow these steps:
- It does not make sense to use a floating point type for age.
- I choose an unsigned integer type because age should always be zero or greater.
- Since a person's age typically ranges from 0 to a little over 100, using a large integer type would waste memory. Therefore, I select `unsigned char`.
- To clearly indicate how many bits are used in memory, I use the standard integer type equivalent to `unsigned char`, which is `uint8_t`.
- Finally, when naming the variable to store age, I use `age_u8` to make its type and purpose clear.

🚀 You can use this [demo project](/embedded-c-data-types/source-code/) to gain a practical understanding of data types in embedded software. In this project, I demonstrate:
- The size of C standard data types, which depends on the microcontroller (STM32F103Cx) architecture and the compiler (Keil Arm).
- The standard data types commonly used in embedded software.
- 🔨 Development Boards: [STM32F103 Blue Pill Development Board](/README.md)
- 🔧 Tools: [Keil uVision](/README.md)

👇 Below, you will find examples from the project source code that illustrate the size of C standard data types and common used embedded C data types.

```c
/* C standard data types */
const uint8_t C_DataSizeInBytes_u8[] = {
    sizeof(char),
    sizeof(unsigned char),
    sizeof(int),
    sizeof(short),
    sizeof(long),
    sizeof(float),
    sizeof(double),
    sizeof(unsigned int)
};

/* Embedded C standard data types */
const uint8_t EmbeddedC_DataSizeInBytes_u8[] = {
    sizeof(bool),
    sizeof(int8_t),
    sizeof(uint8_t),
    sizeof(int16_t),
    sizeof(uint16_t),
    sizeof(int32_t),
    sizeof(uint32_t)
};
```

👉 Actual data type sizes measured in the STM32F103Cx embedded system:
<!-- Images Placeholder -->
<img src="imgs/data-size-debug-view.png" alt="Data Size in Embedded System"/>
<!-- Add more images as needed -->

# Explore More Topics
|[👈 Previous](/stm32f103c6-demo/README.md) | [Next 👉](/struct-union-data-types/README.md)|

# Embedded C Practical Projects
🚀 [Embedded C Practical Projects](/)

# Repositories
🏠 [My Repositories](https://github.com/hothienai)

# My Website
🌐 [Ho Thien Ai](https://hothienai.github.io/)

# Contact & Discussion
If you have any thing would like to discuss or cooperate with me, please don't hesitate to contact me via:
- 📧 Email [Ho Thien Ai](mailto:thienaiho95@gmail.com)
- 💼 LinkedIn [Thien Ai Ho](https://www.linkedin.com/in/thien-ai-ho/)