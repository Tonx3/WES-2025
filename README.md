# WES-2025
Workshop on Embedded Systems - Hackaton
Team project by: Antonia Turkovic, Marko Srpak and Leonardo Max Golusin
Ime tima: sudo rm -rf /

## How to start

1. Download with 
```
git clone https://github.com/Tonx3/WES-2025.git
```
or with 
```
git clone git@github.com:Tonx3/WES-2025.git
```
if you are contributor.

2. **IMPORTANT** never push on main branch, immediately do
```
git checkout develop
```

3. Use
```
git submodule update --init --recursive
```
(**NOTE**: DON'T USE EDUROAM)

4. Position to esp32_dev_board/components/lvgl_esp32_drivers using 
```
cd esp32_dev_board/components/lvgl_esp32_drivers
```

5. Apply a patch using
```
git apply ../lvgl_esp32_drivers_8-3.patch
```

6. Go back to main folder od esp32_dev_board
```
cd ../../
```

7. You can now build and flash the device.

idf.py build
idf.py flash

*IMPORTANT* Koristiti verziju ESP-IDF 5.0.8., nije provjereno radi li na nižim verzijama.

## Kako spojiti senzore

1. Potrebno je pravilno isključiti switcheve:
Svi switchevi trebali bi biti u ON poziciji osim:
- LED_G
- LED_R
- TCH_IRQ

2. Ultrazvucni seznor za udaljenost
- VCC -> 5V
- TRIGGER -> LED_R
- ECHO -> LED_G
- GND -> GND

3. Ambient light sensor
- VIN -> 3V3+
- 3V -> 3v3+
- GND -> GND
- SCL -> SCL
- SDA -> SDA

4. Prototipna pločica 
- polozaj plocice je takav da se LED nalaze na dnu plocice
- sadrži pojacalo za zvucnik, zvucnik, dvije LED s pripadajucim otpornicima i potenciometar 
Žice spojiti prema natpisu na bijelim oznakama
(**NOTE**: NE DIRATI BIJELE OZNAKE DIREKTNO KAKO SE ONE NE BI IZBRISALE)
- svi su spojeni na 3V3
- napravljen je zajednicki GND
- Lijeva LED (zelena zica) -> PIN BTN_2
- Desna LED (ljubicasta zica) -> PIN BTN_3
- Napajanje pojacala (crvena zica) -> 3V3
- Ulaz u pojacalo (bijela zica) -> BTN_4
- Potenciometar (zelena zica) -> JOY_x

## User interface
- sadrzi je ukupno 5 razlicitih screenova
- preko ambient light senzora implementirano je switchanje na DARK_MODE kada svjetlost u prostoriji padne ispod određene razine


**Home screen** 
- sadrzi 6 gumbi 
1. PARKING MODE 
- prebacuje na ekran gdje je prikaz snimke stražnje kamere
2. WIFI
- short press - WiFi Connect
- long press - Wifi Provisioning 
3. SETTINGS 
- mogućnosti paljenje desne i lijeve LEDice (efektivno žmigavaca), mode za svjetla (AUTO, ON, OFF), podesavanje jacine LED_B (headlight) svijetla 
- Save and Exit gumb - spremaju postavke jacine svjetla, mode za svjetlo i jacina zvuka
4. CAR INFO 
- vrijednosti brzine u km/h - BRZINA SE MOZE SMANJIVATI/POVECAVATI pomocu potenciometra
- vrijednost prijeđene udaljenosti koja je izracunata preko brzine i vremena
- vrijednosti tilt-a i incline-a dobivene preko akcelerometra
5. MUSIC 
- klikom na gumb svira par sekundi muziku OIIA OIIA
6. MAPS 
- prebacuje na ekran prikaza karte


                         
                 




    

