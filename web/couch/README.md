XSPA vuln
=============

CouchDb + PHP + иммитация webProxy сервера

Сайт представляет собой веб-прокси, то есть загружает по юрлу страницы.
Написано что все ссылки которые вводишь логируются.
Логируются они в CouchDb которая висит на 5984 порту. Порт iptables'ом настроен только на доступ к нему с локал хоста, то есть обратиться снаружи нельзя

Через веб проски ломаем доступ к базе собственного сервера

=============
Логи сервера(JSON):

{

 "_id":"c49183c63fb98d2c50f5171dab0036cd",
 
 "ip":{
 "REMOTE_ADDR":"127.0.0.1",
 "HTTP_USER_AGENT":"Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.157 Safari/537.36",
 "HTTP_COOKIE":"AuthSession=cmVhZGVyOjU1RTAzMUY3OvEM3zLu1JXRfKgezUchKJQtvh7H"
 },
 
 "date":1441013176,
 
 "link":"http://ya.ru"
 
}

функция вывода флага: _design/kctf

{
	"_id":"_design/kctf",
	
	"shows":{
	
		"flag":"function(doc, req){ function some_function(s){return s.split(\"\").reduce(function(a,b){a=((a<<5)-a)+b.charCodeAt(0);return a&a},0);} var data = some_function(\"flagishere\"); return \"flag is: #\" + data.toString()}"
	
	}
}

=============
Настройки на сервере:

1. Настроить админа

2. Настроить доступ к _utils

3. База данных couch




