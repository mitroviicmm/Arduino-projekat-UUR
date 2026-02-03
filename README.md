Arduino Robot Car – Motor Shield, Ultrazvučni Senzor i Servo Motor

Ovaj projekat predstavlja autonomni robot-autić baziran na Arduino Uno ploči i Motor Control Shield-u (L293D). Sistem koristi dva DC motora za kretanje, ultrazvučni senzor HC-SR04 za merenje rastojanja i servo motor za pomeranje dodatka (nosač senzora).

Projekat je realizovan kao kombinacija kupljenih i postojećih komponenti, uz samostalno sklapanje hardvera i pisanje koda.

Nabavka delova:

Šasija robota i DC motori naručeni su sa AliExpress-a.  
Ultrazvučni senzor HC-SR04 i servo motor kupljeni su u lokalnoj prodavnici u Beogradu.  
Arduino Uno, Motor Control Shield, kablovi i napajanje su već bili dostupni.

Arhitektura sistema:

Sistem se sastoji od tri osnovne celine:

Arduino Uno – upravljačka logika sistema  
Motor Control Shield – upravljanje snagom i smerom motora  
Senzori i aktuatori – interakcija sa okruženjem

Arduino obrađuje podatke i donosi odluke, dok motor driver izvršava fizičko kretanje robota.

Arduino Uno:

Arduino Uno je mikrokontrolerska ploča koja izvršava program, čita podatke sa senzora i šalje upravljačke signale motor driveru. Arduino ne može direktno da napaja motore jer njegovi pinovi imaju ograničenu struju, zbog čega se koristi motor driver.

Motor Control Shield (L293D):

Motor Control Shield služi kao posrednik između Arduino ploče i DC motora.

L293D čip omogućava upravljanje motorima pomoću H-mosta. On omogućava promenu smera okretanja motora i obezbeđuje dovoljnu struju i napon za njihov rad.

74HC595 shift register omogućava upravljanje većim brojem izlaza koristeći manji broj Arduino pinova. Zbog toga su neki Arduino pinovi rezervisani i ne mogu se koristiti za druge komponente.

DC motori:

Robot koristi dva DC motora sa reduktorom, jedan za levi i jedan za desni točak. Motori su povezani na izlaze M3 i M4 na motor shield-u. Svaki motor ima dve žice, a smer okretanja se menja promenom polarnosti unutar L293D čipa.

Servo motor:

Servo motor se koristi za pomeranje dodatka nosač senzora. Servo motor ne ide preko motor drivera jer ima sopstvenu elektroniku i zahteva samo PWM signal, napajanje i masu. Upravljanje se vrši direktno sa Arduino pino-va.

Ultrazvučni senzor HC-SR04:

HC-SR04 meri rastojanje pomoću ultrazvučnih talasa. Arduino šalje kratak impuls na TRIG pin, senzor emituje ultrazvuk i prima povratni signal. Trajanje ECHO signala predstavlja vreme putovanja talasa, na osnovu čega se računa rastojanje.

Povezivanje pinova:

Arduino i Motor Shield koriste sledeće pinove:
D4 – LATCH signal za 74HC595  
D7 – ENABLE signal  
D8 – DATA signal  
D12 – CLOCK signal  
D3, D5, D6 i D11 – PWM signali za kontrolu brzine motora  

DC motori su povezani na M3 i M4 izlaze na motor shield-u.

Servo motor je povezan na:
Signal – D9  
Napajanje – 5V  
Masa – GND  

Ultrazvučni senzor HC-SR04 je povezan na:
VCC – 5V  
GND – GND  
TRIG – D2  
ECHO – D13  

Pinovi D2 i D13 su izabrani jer ih motor shield ne koristi.

Napajanje sistema:

Arduino, servo motor i senzor koriste 5V napajanje.  
DC motori se napajaju posebnim naponom preko EXT_PWR konektora na motor shield-u.  
Svi delovi sistema imaju zajedničku masu (GND), što je neophodno za stabilan rad.

Razdvajanje napajanja sprečava pad napona i smetnje izazvane motorima.

Fizičko sklapanje:

Šasija robota je sastavljena prema uputstvu proizvođača.  
DC motori su mehanički pričvršćeni na šasiju.  
Motor Control Shield je postavljen direktno na Arduino Uno.  
Motori su povezani na M3 i M4 pomoću šraf-terminala.  
Servo motor i ultrazvučni senzor su pričvršćeni na prednji deo robota.  
Sve komponente su povezane zajedničkom masom.

Softver:

Kod je napisan samostalno i uključuje:
upravljanje DC motorima preko motor shield-a  
kontrolu servo motora pomoću PWM signala  
očitanje rastojanja sa HC-SR04 senzora  
logiku kretanja robota (kretanje po kvadratu i reakcije)

Kod je organizovan u funkcije radi lakšeg razumevanja i proširenja.

Cilj projekta:

Cilj projekta je razumevanje rada mikrokontrolera, motor drivera i senzora, kao i samostalno sklapanje hardvera i pisanje upravljačkog koda.

Projekat može poslužiti kao osnova za dalje proširenje i učenje robotike.

Autor:

Projekat je realizovan u edukativne svrhe kao studentski projekat, radili Milica Mitrovic i Lazar Stojanovic.



VIDEO:

https://github.com/user-attachments/assets/d47b664f-ff49-4c26-aca0-8f663c95cecb



https://github.com/user-attachments/assets/280b1217-1f68-4a89-90ae-820ad79f4d67


IMAGES:

![image1](https://github.com/user-attachments/assets/078917d4-50d3-4613-b791-e958001bb695)
![image3](https://github.com/user-attachments/assets/4c87ffc9-e781-47c6-be33-df0dd11618a5)
![image2](https://github.com/user-attachments/assets/217109f1-bd9a-4dcb-bff6-b7c56e6b7e4a)


