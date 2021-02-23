- [Data Type And Functions](#data-type-and-functions)
	- [numbers](#numbers)
	- [date time](#date-time)
	- [str](#str)
	- [cast functions and operators](#cast-functions-and-operators)
	- [bit operators](#bit-operators)
- [运维](#运维)
	- [login](#login)
	- [conf](#conf)
	- [Backup](#backup)

# Data Type And Functions
## numbers
The DECIMAL and NUMERIC types store exact numeric data values. These types are used when it is important to preserve
exact precision, for example with monetary data.
In MySQL, NUMERIC is implemented as DECIMAL.

```sql
salary DECIMAL(5,2)
```
5 是有效数字个数, 2 是小数点位数, 存储的范围就是 [-999.99, 999.99]

## date time
[时间差](http://blog.csdn.net/yzsind/article/details/8831429)

datetime 直接之间作差得到结果不是时间意义上的作差, 实际是mysql做了一个隐式转换操作, 直接把年月日时分秒拼起来,如
`2013-04-21 16:59:33` 直接转换为`20130421165933`, 由于时间不是十进制,所以最后得到的结果没有意义,这也是导致出现坑爹的结果.

要得到正确的时间相减秒值,有以下3种方法:

1. `time_to_sec(timediff(t2, t1))`: timediff 得到的结果是一个时间格式
2. `timestampdiff(second, t1, t2)`
3. `unix_timestamp(t2) - unix_timestamp(t1)`

时间位移: `ADDTIME('2014-05-26 18:26:21', '0:0:2')` 求后2秒的时间.

## str
- `length()`
- `char_length()`
- `locate(substr, str)`: 如果包含, 返回 > 0 的数, 否则返回0

## cast functions and operators
- BINARY: cast a string to a binary string, `BINARY str` is a shorthand for `CAST(str AS BINARY)`.
- `CAST(expr AS type)`: cast a value as a certain type
- CONVERT(): cast a value as a certain type

The BINARY operator casts the string following it to a binary string.
This is an easy way to force a column comparison to be done byte by byte rather than character by character.
```sql
SELECT 'a' = 'A';  // 1
SELECT BINARY 'a' = 'A';  // 0
SELECT 'a' = 'a ';  // 1
SELECT BINARY 'a' = 'a ';  // 0
```

| name       | description                        |
| ---        | ---                                |
| COALESCE() | return the first non-NULL argument |

## bit operators
[Bit Functions and Operators](https://dev.mysql.com/doc/refman/8.0/en/bit-functions.html)
| Name        | Description                            |
| ---         | ---                                    |
| &           | Bitwise AND                            |
| >>          | Right shift                            |
| <<          | Left shift                             |
| ^           | Bitwise XOR                            |
| BIT_COUNT() | Return the number of bits that are set |
| \|	        | Bitwise OR                             |
| ~	          | Bitwise inversion                      |

优先级: ~ > ^ > & > >> = << > |

