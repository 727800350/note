drop database if exists test;
create database test;
use test;

DROP TABLE IF EXISTS tab1_1;
CREATE TABLE tab1_1(
    tab1_id varchar(11)
);

DROP TABLE IF EXISTS tab2;
CREATE TABLE tab2(
    tab2_id varchar(11)
);

-- 创建触发器:t_afterinsert_on_tab1_1
-- 作用：增加tab1_1表记录后自动将记录增加到tab2表中
DROP TRIGGER IF EXISTS t_afterinsert_on_tab1_1;
CREATE TRIGGER t_afterinsert_on_tab1_1
AFTER INSERT ON tab1_1
FOR EACH ROW 
	insert into tab2(tab2_id) values(new.tab1_id);

DROP TRIGGER IF EXISTS t_afterdelete_on_tab1_1;
CREATE TRIGGER t_afterdelete_on_tab1_1
AFTER DELETE ON tab1_1
FOR EACH ROW
      delete from tab2 where tab2_id=old.tab1_id;

DROP TABLE IF EXISTS tab1_2;
CREATE TABLE tab1_2(
    tab1_id varchar(11)
);

DROP TABLE IF EXISTS tab3;
CREATE TABLE tab3(
    tab3_id varchar(11)
);

DROP TABLE IF EXISTS tab4;
CREATE TABLE tab4(
    tab4_id varchar(11)
);

-- 创建触发器:t_afterinsert_on_tab1_2
-- 作用：增加tab1_2表记录后自动将记录增加到tab3, tab4中
DROP TRIGGER IF EXISTS t_afterinsert_on_tab1_2;
DELIMITER ||
CREATE TRIGGER t_afterinsert_on_tab1_2
AFTER INSERT ON tab1_2
FOR EACH ROW 
BEGIN
	insert into tab3(tab3_id) values(new.tab1_id);
	insert into tab4(tab4_id) values(new.tab1_id);
END
||
DELIMITER ;

show tables;
show triggers\G

