function sum(n){
	var sum = 0;
	for(var i = 1; i <= n; i++){
		sum += i;
	}
	return sum;
}

exports.sum = sum;
// exports对象的sum 属性是我们在上面定义的sum 函数

