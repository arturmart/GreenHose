# GreenHose


//to download OS ubuntu focal desktop linux 5.10.75

https://etcher.balena.io/#download-etcher
https://docs.viam.com/installation/prepare/orange-pi-3-lts/

--------------------------------------------------------------------------------------------------------

Включение SSH на Orange Pi 3 LTS
Способ 1: Включение SSH через командную строку (если доступен монитор и клавиатура)
Откройте терминал на Orange Pi.
Введите следующую команду, чтобы убедиться, что SSH установлен:

sudo apt update
sudo apt install openssh-server

После установки SSH-сервера убедитесь, что он запущен:

sudo systemctl enable ssh
sudo systemctl start ssh

Проверьте статус SSH:

sudo systemctl status ssh

--------------------------------------------------------------------------------------------------------
#for vscode 
https://code.visualstudio.com/docs/remote/ssh
https://www.youtube.com/watch?v=BkQ2hL0fkWM
https://help.rc.ufl.edu/doc/SSH_Using_VS_Code
https://code.visualstudio.com/docs/remote/ssh-tutorial

https://www.ssh.com/academy/ssh/command

vs code> Remote-SSH: Connect to Host

ssh orangepi@<IP-адрес Orange Pi>

--------------------------------------------------------------------------------------------------------

Advanced IP Scanner
192.168.10.1-254
PUTTY

--------------------------------------------------------------------------------------------------------

sudo apt-get update
sudo apt-get upgrade


sudo apt install python3-pip
sudo apt-get install python3-dev

sudo pip3 install Orangepi.GPIO

-sudo apt-get install wiringpi
-sudo apt-get remove wiringpi

WiringPi-ն չի աշխատում OrangePI 3 LTS -ի համար պետք ա wiringOP
--------------------------------------------------------------------------------------------------------
https://github.com/orangepi-xunlong/wiringOP/blob/next/README.md
https://blog.regimov.net/orange-pi-lite2-1-wire/
https://micro-pi.ru/%D1%81%D1%85%D0%B5%D0%BC%D0%B0-%D0%BF%D0%BE%D0%B4%D0%BA%D0%BB%D1%8E%D1%87%D0%B5%D0%BD%D0%B8%D1%8F-hc-sr04-%D0%BA-orange-pi-pc/#__HC-SR04_Orange_Pi_PCBanana_Pi_Raspberry_Pi_GPIO_40

apt-get install -y git
git clone https://github.com/orangepi-xunlong/wiringOP.git

cd wiringOP
./build clean
./build 

gpio readall

--------------------------------------------------------------------------------------------------------
http://www.orangepi.org/html/hardWare/computerAndMicrocontrollers/details/orange-pi-3-LTS.html
https://linux-sunxi.org/1-Wire

sudo nano /boot/orangepiEnv.txt
overlays=w1-gpio
param_w1_pin=PD22

sudo reboot

sudo modprobe w1-gpio
sudo modprobe w1-therm

lsmod | grep w1

ls /sys/bus/w1/devices/

--------------------------------------------------------------------------------------------------------

pip3 install --upgrade w1thermsensor

--------------------------------------------------------------------------------------------------------
sudo apt-get update
sudo apt-get install i2c-tools

sudo nano /boot/orangepiEnv.txt
overlays=i2c0 i2c1
param_i2c1_baudrate=100000 


sudo i2cdetect -y 0  # Для I2C-0
sudo i2cdetect -y 1  # Для I2C-1

cat <dir> //for visualisation
--------------------------------------------------------------------------------------------------------

OpenWeatherMap API
https://openweathermap.org/price#weather        //main

https://openweathermap.org/api                  //patetner
https://openweathermap.org/api/one-call-3
https://home.openweathermap.org/api_keys
https://openweathermap.org/weather-conditions    //icons

https://api.openweathermap.org/data/2.5/weather?lat={lat}&lon={lon}&appid={API key}&units=metric
api.openweathermap.org/data/2.5/forecast?lat={lat}&lon={lon}&appid={API key}&units=metric

marmrashen lat{40.059456} lon{44.474210}
my appid {fcb989e5668460983b3cb819569b8c1d}

https://api.openweathermap.org/data/2.5/weather?lat=40.059456&lon=44.474210&appid=fcb989e5668460983b3cb819569b8c1d&units=metric
https://api.openweathermap.org/data/2.5/forecast?lat=40.059456&lon=44.474210&appid=fcb989e5668460983b3cb819569b8c1d&units=metric

https://openweathermap.org/bulk
https://openweathermap.org/forecast5

--------------------------------------------------------------------------------------------------------
JSON for Modern C++ (Niels Lohmann)  :JSON-ի հետ աշխատելու համար
   sudo apt-get install nlohmann-json3-dev

libcurl                              :библиотеку для выполнения HTTP-запросов
    sudo apt-get install libcurl4-openssl-dev


--------------------------------------------------------------------------------------------------------

arduino to Orange pi

https://www.researchgate.net/figure/Pattern-selection-from-recorded-sample-intervals_fig2_350297410
https://www.arducoding.com/2018/05/serial-debug-pada-orange-pi-menggunakan.html
https://micro-pi.ru/rpi-pi4j-%D1%83%D1%80%D0%BE%D0%BA-6-%D1%88%D0%B8%D0%BD%D0%B0-spi/
https://psenyukov.ru/%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D0%B0-%D1%81-gpio-%D1%80%D0%B0%D0%B7%D1%8A%D0%B5%D0%BC%D0%B0%D0%BC%D0%B8-%D0%B2-orange-pi/
https://f1atb.fr/orange-pi-gpio/
https://www.amazon.com/SparkFun-Logic-Level-Converter-Bi-Directional/dp/B01N30ZCW9/ref=sr_1_1?dib=eyJ2IjoiMSJ9.qrzfvZOtn4CIylDG2bI-VZHxiJZkcBVgtb9gS1lu7s5IKVMoyg7MFJySEeBTWM3xrZEpWQSxSit2T8eRnBpXxrzDk2qphxGCPGfOf3hfQvS0mw12tWJ_-tW6ddzlrbdLBeQL--T21PelzQUKha1IiBkPjfc97AHP_qNKGe4_6-LZjzHgBDLa3SmsSdNe_qgFBqMiTubgt201ywSuMsG8cgPVh2b1JjHY8XASEPGEf8k._UkYar4XbircUF7dRs_yiUslKo3eLcuC1Z4ykHcsUhA&dib_tag=se&keywords=logic+level+converter&qid=1729101691&sr=8-1






