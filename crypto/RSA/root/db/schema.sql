drop table if exists items;
CREATE TABLE items (
  id integer primary key autoincrement,
  cost text not null,
  title text not null,
  sign text not null
);
drop table if exists users;
CREATE TABLE `users` (
	`id`	INTEGER PRIMARY KEY AUTOINCREMENT,
	`login`	TEXT unique,
	`password`	TEXT,
	`cash`	INTEGER DEFAULT '10000'
);