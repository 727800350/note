# DataFrame and Series
- DataFrame, which you can imagine as a relational data table, with rows and named columns.
- Series, which is a single column. A DataFrame contains one or more Series and a name for each Series.

## 创建
DataFrame objects can be created by passing a dict mapping string column names to their respective Series. If the Series don't match in length, missing values are filled with special `NA/NaN` values. Example:
```Python
import pandas as pd
city_names = pd.Series(['San Francisco', 'San Jose', 'Sacramento'])
population = pd.Series([852469, 1015785, 485199])
pd.DataFrame({'City name': city_names, 'Population': population})
```

`pd.DataFrame(data=None, index=None, columns=None)`: data 可以是numpy 的array, 可以是python 的dict(key 被作为df.columns, values 作为df.values)
```Python
dates = pd.date_range('20130101', periods=6)
df = pd.DataFrame(np.random.randn(6, 4), index=dates, columns=['a', 'b', 'c', 'd']) ## 创建一个6行4列的df, 行的key 为 dates, 4列的名字分别为'a', 'b', 'c', 'd'
```

But most of the time, you load an entire file into a DataFrame.
```Python
california_housing_dataframe = pd.read_csv("https://storage.googleapis.com/mledu-datasets/california_housing_train.csv", sep=",")
california_housing_dataframe.describe()
```

## Brief
- `california_housing_dataframe.describe()`
- df.shape, df.ndim, df.size
- df.head(), df.tail()
- df.mean()

## Accessing Data
You can access DataFrame data using familiar Python dict/list operations

- `cities['City name']`: 获取City name 的列
- `cities[0:2]`: 获取行, 当index 不是数字时, 就需要用对应的index, eg `df['20130102':'20130104']`

## Manipulating Data
You may apply Python's basic arithmetic operations to Series.

- `population / 1000`
- `np.log(population)`: numpy operations
- `population.apply(lambda val: val > 1000000)`: creates a new Series that indicates whether population is over one million
- `cities['Area square miles'] = pd.Series([46.87, 176.53, 97.92])`: 直接指定一个新的列
- `cities['Population density'] = cities['Population'] / cities['Area square miles']`: 根据已有列生成新列

Note: Boolean Series are combined using the bitwise, rather than the traditional boolean, operators. For example, when performing logical and, use & instead of and.

`cities['Is wide and has saint name'] = (cities['Area square miles'] > 50) & cities['City name'].apply(lambda name: name.startswith('San'))`

## index
Both Series and DataFrame objects also define an index property that assigns an identifier value to each Series item or DataFrame row.
By default, at construction, pandas assigns index values that reflect the ordering of the source data. Once created, the index values are stable; that is, they do not change when data is reordered.

- `cities.index` and `city_names.index`
- `cities.reindex([2, 0, 1])`: 变换行的顺序, 但是不改变index 的值, 也就是第一行的index 是2

## API
- df.columns, df.values

- df.loc: 通过label 来选择, `df.loc[dates[0]]`, `df.loc[:,['A','B']]`, `df.loc['20130102':'20130104',['A','B']]`
- df.iloc: 通过坐标 来选择, `df.iloc[3]`, `df.iloc[[1,2,4],[0,2]]`, `df.iloc[1:3,:]`
- df.at: 通过label 获取一个值, `df.at[dates[0],'A']`
- df.iat: 通过坐标 获取一个值, `df.iat[1,1]` 比 `df.iloc[1,1]` 速度快
- df.ix

- `df.sort_values(by=['a', 'b'], axis=0, ascending=True, inplace=False, kind='quicksort', na_position='last')`: axis {0 or 'index', 1 or 'columns'}, default 0
- `df.T`: 转置
- `df.apply(func, axis=0)` aixs=0 为列, aixs=1 为行
	- `df.apply(np.cumsum)`
	- func 可以为lambda表达式, `df.apply(lambda x: x.max() - x.min())`

