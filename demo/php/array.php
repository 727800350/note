#!/usr/bin/env php
// Example #4 Indexed arrays without key

<?php
$array = array("foo", "bar", "hello", "world");
var_dump($array);
?>

/*
The above example will output:

array(4) {
  [0]=>
  string(3) "foo"
  [1]=>
  string(3) "bar"
  [2]=>
  string(5) "hello"
  [3]=>
  string(5) "world"
}
It is possible to specify the key only for some elements and leave it out for others:
*/

// Example #5 Keys not on all elements

<?php
$array = array(
         "a",
         "b",
    6 => "c",
         "d",
);
var_dump($array);
?>
/*
The above example will output:

array(4) {
  [0]=>
  string(1) "a"
  [1]=>
  string(1) "b"
  [6]=>
  string(1) "c"
  [7]=>
  string(1) "d"
}
As you can see the last value "d" was assigned the key 7. This is because the largest integer key before that was 6.
*/
