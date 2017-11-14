###Математические результаты использования математики с фиксированной точкой в функции biusQuaternionCalc() в прошивке прибора БИУС

Использование fixed-point математики в biusQuaternionCalc() не предоставляется возможным.

На графике ниже представлены результаты  математического моделирования.
+ Oy: Разница между значениями компонентами кватерниона fltquaternion[4], который считался во float, и fixquaternion[4], значения которого представлены int32, где 27 разрядов занимает дробная часть, 4 - целая, 1 - знак.
+ Ox: Номер такта

![graph] (https://1.downloader.disk.yandex.ru/disk/3762a5a9ac321e521fc944bfdc56bb19feaed9273960147efeb32e8a958f4a17/59fb270e/v7LIHD257rT16OzYJsFH7nr3RT5Y0KaP6NXuCCSEL5lK1xP-e6_jEQJjG1WJ6yIoGp9jpiSc5hte25yaUvBqug%3D%3D?uid=0&filename=123.png&disposition=inline&hash=&limit=0&content_type=image%2Fpng&fsize=53640&hid=80842d14b28d8914c3c998a6c01c5fb1&media_type=image&tknv=v2&etag=a9f23432c6748f86c71bc454bd78be07)

Из графика видно, что разница между компонентами fltquaternion[2:3] и fixquaternion[2:3] ~0.24. Что является недопустимой погрешностью для рекуретного вычисления компонентов кватернионов функцией biusQuaternionCalc(). 
