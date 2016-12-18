# Intro
## Example
```json
{
	"name": "John Simith",
	"age": 25,
	"address": {
		"city": "New York",
		"state": "NY",
	},
	"phoneNumbers": [
		{"type": "home", "number": "212 555-1234"},
		{"type": "fax", "number": "646 555-4567"}
	]
}
```

## Format
1. 并列的数据之间用逗号(",")分隔. 
2. 映射用冒号(":")表示. 
3. 并列数据的集合(数组)用方括号("[]")表示. 
4. 映射的集合(对象)用大括号("{}")表示.

comments in JSON are not allowed.
也就是说JSON中全是data(但是可以在json 中增加一个comment的字段来加)

# API
## python
import json

- `json.load(fp)`: Deserialize fp (a .read()-supporting file-like object containing a JSON document) to a Python object
- `json.loads(s, encoding=None)`: Deserialize s(a str instance containing a JSON document) to a Python object

If the data being deserialized is not a valid JSON document, a JSONDecodeError will be raised.

- json.dump(obj, fp): Serialize obj as a JSON formatted stream to fp (a .write()-supporting file-like object)
- json.dumps(obj): Serialize obj to a JSON formatted str

