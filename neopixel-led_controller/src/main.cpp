#include <Arduino.h>    // necesara pentru vsCode
#include <Wire.h>       // necesara pentru vsCode
/*
##################################################################
##                                                              ## 
##        DEVICES USED :                                        ##
##                                                              ## 
##      - Arduino UNO                                           ## 
##      - Keyestudio 1602LCD Keypad Shield For Arduino LCD      ## 
##      - Adafruit NeoPixel Digital RGB LED Strip               ## 
##         ->(lenght is 108 led folded in two circles)          ##
##                                                              ##
##      Version v0.1.11   Project still in progres              ##
##                                                              ##
##################################################################
*/

// ____ librari incluse 
    //  #include <LiquidCrystal_I2C.h> // Librarie LCD i2c
    #include <LiquidCrystal.h>      // Librarie LDC simplu
    #include <Adafruit_NeoPixel.h> // Librarie Banda NeoPixel
    #include "Language_controller.h" // includem fisierul cu dictionarul de limba
    #include <string.h>
    // definiti pentru functi 
    //#define VAR_ASTEAPTA unsigned long int

// ____ setrai ale librariilor 

    // __LCD i2c
    //  LiquidCrystal_I2C lcd (0x27, 16,2); // stabilim adresa si dimensiunea LCD  // stergem bara de comentariu daca avem lcd i2c 
    
    //__ LCD simplu
    LiquidCrystal lcd(8,9,4,5,6,7);  // o comentam daca avem lcd i2c
        
    // __NeoPixel
    #ifdef __AVR__
    #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
    #endif
    #define PIN 12  //definim pinul de comanda pt neopixel
    #define NUMPIXELS      108  // definim  numarul de pixeli
    #define on 1
    #define off 0
    
    // stabilim numele benzi de leduri
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// ____ variabile globale
    uint16_t culoare = 0  ; // culoare 
    uint8_t lumina = 55 ;// sabilim luminozitatea pixelilor
    uint8_t saturatie = 255 ; // saturatia culori
    uint16_t led_pos = 0 ; // pozitia ledului aprins
    uint16_t led_fill = 54 ; // cate leduri sunt umplute dupa led pos
    uint16_t viteza = 1; // stabilim o variabila pt viteza
    uint32_t intarziere = 300 ; // stabilim o variabila pt delay 
//
    uint8_t jocPos=1; // variabila pt selectare jocuri
    const int nr_optiuni_disponibile = 11 ;  // numarul de setari disponibile           !!! se modifica in  cazul adaugari unei setari 
    int op_disp [nr_optiuni_disponibile] ; 
    char* NumeJoc ; // stabilim o variabila pentru numele jocului actual
    const int nrJocuri = 10 ; // Stabilim numarul de jocuri pe care le avem              !!! se va modifica de fiecare data cand creem un joc !!!
    uint16_t multiplu =1; // variabila creata pt a modifica mai usor setarile 
    uint8_t sel_settings = 0; // variabila pentru selectarea si afisarea setari
    uint8_t var_taste = 0 ;
    uint8_t lb_sel = 0 ;
    bool count = 0 ;
    int led_pos1 ;
    bool pix_cle = 1 ;

// ____ prototipuri functi
 
    //input de la taste
    int taste (void) ;
    //afisarea meniului de setari 
    int meniu (void) ;
    // functia pentru schimbarea  jocurilor de lumini
    void jocuri (int select);
    // functie de limitare
    int lim_plus (int val , int val2 , int min , int max );
    int lim_minus (int val , int val2 , int min , int max );
    // functie pentru comunicare prin serial monitor  
    void serail_usb (void);
    //
    void set_var (char* set , int val);
    
    //functie control leduri---culoare---saturatie----luminozitate---pozitie led --- led fill
    //necesara pentru a simplifica scrierea jocurilor pt ambele parti ale benzi de leduri
    void two_circle (uint16_t col , uint8_t sat , uint8_t lux , uint32_t pos , uint32_t fil , uint32_t nr_pixel ,bool pixel_clear); 
    void set_setting_available (bool od1, bool od2, bool od3, bool od4, bool od5, bool od6, bool od7, bool od8, bool od9, bool od10);
    

#include "Functii.h" // includem fisierul cu functii 





void setup()
{
    // setarile de start pentru LCD
    /* //Doar pentru LCD i2c  
    lcd.init ();
    lcd.backlight ();
    */
    // Doar pentru LCD simplu
    lcd.begin(16,2);
    
    // setarile de start pentru banda rgb
    pixels.begin();           
    pixels.show();
    pixels.setBrightness(255);
    // pornire serrial 
    Serial.begin(9600) ;
    // printare mesaj de pornire
    lcd.clear();
    lcd.setCursor (5,0);
    lcd.print ("Welcome!"); 
    delay(2000);
    lcd.clear();
    Serial.println("Serial monitor is on");
}
  




void loop() 
{   
    jocuri(jocPos); // incepem cu primul joc
    
    // creem o functie de delay care nu blocheaza procesorul 
    static unsigned long delay_tasta ; 
    if (millis() - delay_tasta >= 100 ) 
    {
        var_taste = taste();
        delay_tasta = millis();
    }
    else 
    {
        var_taste = 0 ;
    }

    if(Serial.available()>0)
    {
        serail_usb ();
    }
    //pixels.fill( pixels.ColorHSV(culoare , saturatie , lumina ) , led_pos , led_fill);
     //  pixels.show();
    meniu ();
}

//#####################################################################################################################################
//#####################################################################################################################################
//#####################################################################################################################################
//#####################################################################################################################################



/* optiunile disponibile sunt
* 0 - joc // nu este modificabila
* 1 - culoare
* 2 - lumina
* 3 - saturatie
* 4 - pozitie led
* 5 - nr of led fill after led pos
* 6 - viteza
* 7 - intarziere
* 8 - limba
* 9 - despre
* 10-
*/

void jocuri (int select)
{   

    
    switch (select)
    {
        case 0:
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1    2    3    4    5    6    7   8    9    10    
                set_setting_available (off, off, off, off, off, off, off, on, on ,off);
                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }

            NumeJoc  = text_setari[lb_sel] ;
        break;


        case 1 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1   2   3   4   5   6   7   8    9    10 
                set_setting_available (on, on, on, on, on, on, on, off, off ,on);
                pix_cle = on ;
                saturatie = 255;
                led_pos = 0 ;
                led_fill = NUMPIXELS/2 ;

                count=0 ; // inchidem while
                break; // doar pentru siguranta

            }

     
            two_circle (culoare, saturatie, lumina, led_pos, led_fill, NUMPIXELS, pix_cle); 

            NumeJoc  = (char*)"Jocul 1" ;
        break;

    
        case 2 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1    2    3    4   5   6   7   8    9    10 
                set_setting_available (off, off, off, on, on, on, on, on, off ,on);
                pix_cle = on ;
                saturatie = 255;
                viteza=50;
                led_pos = 0 ;
                led_fill = NUMPIXELS/2 ;

                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }     
            culoare += viteza ;

            two_circle (culoare, saturatie, lumina, led_pos, led_fill, NUMPIXELS, pix_cle); 
     
            NumeJoc = (char*)"Jocul 2" ;
        break;

    
        case 3 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1   2   3   4   5   6   7   8    9    10 
                set_setting_available (on, on, on, on, on, on, on, off, off ,on);
                pix_cle = on ;
                saturatie = 255;
                intarziere = 50 ;
                viteza = 50 ; 
                led_pos = 0 ;
                led_fill = NUMPIXELS/4 ;
                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }

            culoare += viteza ;



            // creem o functie de delay care nu blocheaza procesorul 
            static unsigned long delay_j3 ; 
            if (millis() - delay_j3 >= intarziere )  
            {
                if (led_pos<(NUMPIXELS/2))
                led_pos ++ ;
                else 
                led_pos = 0 ;

                delay_j3 = millis();
            }
             


            two_circle (culoare, saturatie, lumina, led_pos, led_fill, NUMPIXELS, pix_cle); 
     
            NumeJoc = (char*)"Jocul 3" ; 
        break;


        case 4 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1   2   3   4   5   6   7   8    9    10 
                set_setting_available (on, on, on, on, on, on, on, off, off ,on);
                pix_cle = on ;
                saturatie = 255;
                intarziere = 50 ;
                viteza = 50 ; 
                led_pos = 0 ;
                led_fill = 1 ;

                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }

            static int led_pos_j4  ;
            culoare += viteza ;
            
            static unsigned long delay_j4 ; 
            if (millis() - delay_j4 >= intarziere ) 
            {   
                
                if (led_fill >= NUMPIXELS / 2 )
                {
                    led_pos_j4 = led_pos ;
                    led_fill= 1 ;
                    culoare += 200;
                }
                else
                {
                    led_pos_j4 = lim_minus (led_pos_j4 , 1 , 0 , (NUMPIXELS/2)-1) ;
                    led_fill +=2;
                }

                delay_j4 = millis();
            }
             

            two_circle (culoare, saturatie, lumina, led_pos_j4, led_fill, NUMPIXELS, pix_cle); 
     
            NumeJoc = (char*)"Jocul 4" ;

        break;
        
        case 5 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1   2   3   4   5   6   7   8    9    10 
                set_setting_available (off, on, off, off, off, off, on, off, off ,off);
                pix_cle = on ;
                saturatie = 255;
                intarziere = 100;
                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }

            static int x = 0 ;
            static unsigned long delay_j5 ; 
            if (millis() - delay_j5 >= intarziere )  
            {
                if(x<12)
                x++;
                else
                x=0;

                delay_j5 = millis();
            }
            two_circle (CULORI[x], saturatie, lumina, 0 , NUMPIXELS/2, NUMPIXELS, pix_cle); 
     
            NumeJoc = (char*)"Jocul 5" ;
        break;

        case 6 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1   2   3   4   5   6   7   8    9    10 
                set_setting_available (off, on, off, off, off, off, on, off, off ,off);
                saturatie = 255;
                intarziere=1;

                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }

            static uint16_t led_pos_j6 = 0 ; 
            static unsigned long delay_j6 ; 
            if (millis() - delay_j6 >= intarziere )  
            {   
                if (led_pos < (NUMPIXELS/2)-1)
                {
                    led_pos ++ ;
                }
                else
                {
                    led_pos=0;
                }
                if(led_pos< NUMPIXELS/4)
                {
                    led_pos_j6 = led_pos+NUMPIXELS/4 ;
                }
                else
                {
                    led_pos_j6 = led_pos-NUMPIXELS/4 ;
                }


                delay_j6 = millis();
            }

            two_circle (ROSU, saturatie, lumina, led_pos, NUMPIXELS/4,NUMPIXELS ,off); 
            two_circle (ALBASTRU, saturatie, lumina, led_pos_j6, NUMPIXELS/4,NUMPIXELS ,off); 

            NumeJoc = (char*)"POLICE" ;
        break;

        case 7 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
               //--------------------- 1   2   3   4   5   6   7   8    9    10 
                set_setting_available (off, on, off, off, off, off, on, off, off ,off);
                saturatie = 255;
                intarziere = 40 ;
                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }

            static uint8_t count_j7 ;

            static unsigned long delay_j7 ; 
            if (millis() - delay_j7 >= intarziere )  
            {   
                count_j7 ++ ;
                delay_j7 = millis();
            }  
                
            if ((count_j7 >= 0 && count_j7 <= 1 ) || ( count_j7 >= 4 && count_j7 <= 5 ))
            {
                two_circle (ROSU, saturatie, lumina, 0, NUMPIXELS/4,NUMPIXELS ,off); 
                two_circle (ALBASTRU, saturatie, lumina, NUMPIXELS/4, NUMPIXELS/4,NUMPIXELS ,off);

            }         
            else if (count_j7 >= 8 && count_j7 <= 9 )
            {
                two_circle (ALBASTRU, saturatie, lumina, 0, NUMPIXELS/4,NUMPIXELS ,off); 
                two_circle (ROSU, saturatie, lumina, NUMPIXELS/4, NUMPIXELS/4,NUMPIXELS ,off);

            }
            else if (count_j7 >= 11)
            {
                count_j7 = 0 ;
            }
            else
            {
                two_circle (0,0,0, 0, NUMPIXELS/2,NUMPIXELS ,on);
            }
            


 

            NumeJoc = (char*)"POLICE2" ;
        break;


        case 8 :   // are bug
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1   2   3   4   5   6   7   8    9    10
                set_setting_available (off, on, off, off, on, off, on, off, off ,off);
                led_fill = NUMPIXELS/4 ;
                intarziere= 50;

                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }

            static uint16_t culoare_j8;
            static uint16_t culoare2_j8;
            static int led_pos_j8 = 0;
            static int led_pos2_j8 = NUMPIXELS-1;

            static unsigned long delay_j8 ; 
            if (millis() - delay_j8 >= intarziere )  
            {   
                if(led_pos_j8<(NUMPIXELS/2)-1)
                {
                    led_pos_j8 ++;
                }
                else
                {
                    led_pos_j8 = 0;    
                }

                if(led_pos2_j8 >NUMPIXELS/2)
                {
                    led_pos2_j8 --;
                }
                else
                {
                    led_pos2_j8 = NUMPIXELS-1;    
                }

                culoare_j8 = random() ;
                culoare2_j8 = random() ; 

                delay_j8 = millis();
            }  

            pixels.clear();
            pixels.fill( pixels.ColorHSV(culoare_j8 , 255 , lumina ) , led_pos_j8 , led_fill);
            pixels.fill( pixels.ColorHSV(culoare2_j8 , 255 , lumina ) , led_pos2_j8 , led_fill);
            pixels.show();


            NumeJoc = (char*)"Jocul 8" ;
        break;

        case 9 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1   2   3   4   5   6   7   8    9    10
                set_setting_available (off, on, off, off, on, off, on, off, off ,on);
                intarziere = 50 ;
                led_fill=50;
                pix_cle = on ; 

                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }


            static uint16_t culoare_j9; 
            static int led_pos_j9 = 0;
            static int led_fill_j9 =1 ;

            static unsigned long delay_j9 ; 
            if (millis() - delay_j9 >= intarziere )  
            {
                culoare_j9 = random();
                led_pos_j9 = random (0,NUMPIXELS-1);
                led_fill_j9 = random (1,led_fill);

                delay_j9 = millis();
            }
            if(pix_cle)
            {
                pixels.clear();
            }

            pixels.fill( pixels.ColorHSV(culoare_j9 , 255 , lumina ) , led_pos_j9 , led_fill_j9);
            pixels.show();


        NumeJoc = (char*)"Jocul 9" ;
        break;
    

        case 10 :
            static uint16_t culoare_j10 ;
            static uint16_t culoare2_j10;
            static uint16_t led_pos_j10 ;
            static uint16_t led_pos2_j10 ;
            static bool count_j10 ;
            static uint16_t intarziere_j10;
            static uint16_t intarziere2_j10;

            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1   2   3   4   5   6   7   8    9    10
                set_setting_available (off, on, off, off, on, off, on, off, off ,on);

                pix_cle = on ;
                saturatie = 255;
                led_fill= 4;
                culoare_j10 = random();
                culoare2_j10 = random ();
                led_pos_j10 = 0 ;
                led_pos2_j10 = 10 ;
                count_j10 = 1 ;
                intarziere=30;
                intarziere_j10 = random(1,intarziere);
                intarziere2_j10 = random(1,intarziere);
    
                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }


            static unsigned long delay_j10 ; 
            if (millis() - delay_j10 >= intarziere_j10  )  
            {   
                if(count_j10)
                {
                    if(led_pos_j10 < NUMPIXELS/2-1)
                    led_pos_j10 ++;
                    else
                    led_pos_j10 = 0 ;
                }
                else
                {
                    if(led_pos_j10 > 0)
                    led_pos_j10 --;
                    else
                    led_pos_j10 = NUMPIXELS/2-1 ;  
                }

                delay_j10 = millis();
            }
            static unsigned long delay2_j10 ; 
            if (millis() - delay2_j10 >= intarziere2_j10 )  
            {   
                if(count_j10)
                {
                    if(led_pos2_j10 > 0)
                    led_pos2_j10 --;
                    else
                    led_pos2_j10 = NUMPIXELS/2-1 ; 
                }
                else
                {
                    if(led_pos2_j10 < NUMPIXELS/2-1)
                    led_pos2_j10 ++ ;
                    else
                    led_pos2_j10 = 0 ; 
                }

                delay2_j10 = millis();
            }  

            if (count_j10 == 1 && led_pos_j10 + 1 == led_pos2_j10 )
            {
                count_j10 = 0 ;
                culoare_j10 = random();
                culoare2_j10 = random ();
                intarziere_j10 = random(1,intarziere);
                intarziere2_j10 = random(1,intarziere);
            }
            else if (count_j10 == 0 && led_pos_j10 -1 == led_pos2_j10 )
            {
                count_j10 = 1 ;
                culoare_j10 = random();
                culoare2_j10 = random ();
                intarziere_j10 = random(1,intarziere);
                intarziere2_j10 = random(1,intarziere);
            }

            two_circle (culoare_j10, saturatie, lumina, led_pos_j10, led_fill, NUMPIXELS, off);
            two_circle (culoare2_j10, saturatie, lumina, led_pos2_j10, led_fill, NUMPIXELS, off);
            if(pix_cle)
                pixels.clear () ;

            NumeJoc = (char*)"Jocul 10" ;
        break;




        default :
            lcd.clear();
            lcd.setCursor (0, 0);
            lcd.print("eroare") ;
        break;
    }            
    
  
}

//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################
//####################################################################################################################################



  
int meniu (void)
{

     // creem o functie de delay care nu blocheaza procesorul 
    static unsigned long delay_meniu ; 

    if (millis() - delay_meniu >= 100 )
    {
        lcd.clear();
        
        delay_meniu = millis();
    }

      
    static int last_sel_settings ;
    last_sel_settings = sel_settings ;
  
    if ( var_taste == 3 ) 
    {
        if (sel_settings  < nr_optiuni_disponibile-1)
        {
            sel_settings  ++ ;

            while (op_disp [sel_settings ] == off )
            {
                sel_settings  ++ ;

                if (sel_settings == nr_optiuni_disponibile-1 && op_disp [sel_settings ] == off) 
                {
                    sel_settings = last_sel_settings ;
                    break ;
                }
            }         

        }
    }
      
    else if (var_taste == 2)
    {
        if(sel_settings  >0 ) 
        {
            sel_settings  -- ;
           
            while (op_disp [sel_settings ] == off )
            {
                sel_settings  -- ;
                if (sel_settings == 0 && op_disp [sel_settings ] == off) 
                {
                    sel_settings = last_sel_settings ;
                    break ;
                }
            }
        }
       
    } 
    
    
    switch (sel_settings) 
    {
 
        case 0 :
    
            lcd.setCursor (0, 0);
            if (jocPos !=0 )
            {
                lcd.print(text_joc[lb_sel]);
            }

            lcd.print (NumeJoc);

            // citire taste   
            if (var_taste == 1) 
            {
                if(jocPos<nrJocuri)
                {
                    jocPos++ ; // in cazul in care nu depasim nr de jocuri , trecem la urmatorul
                    count = 1 ; 
                }
            }
            else if (var_taste == 4)
            {
                if (jocPos>0)
                {
                    jocPos-- ;
                    count = 1 ;
                }
            } 
          
        break;

   
        case 1 :
        
            lcd.setCursor (0, 0);
            lcd.print(text_culoare[lb_sel]);
            lcd.print (culoare) ;
            lcd.setCursor (10,1);
            lcd.print("+");
            lcd.print (multiplu);
        
            if (var_taste == 1) 
            {
                culoare += multiplu ; 
            }

            else if (var_taste == 4)
            {
                culoare -= multiplu ;
            }

            else if (var_taste == 5)
            {
                if (multiplu<10000)
                {
                    multiplu *= 10 ; 
                }

                else 
                {
                    multiplu = 1 ;
                }
            }
        
        break;


        case 2 :
            lcd.setCursor (0, 0);
            lcd.print(text_stral[lb_sel]) ;
            lcd.print(lumina) ;
            lcd.setCursor (10,1);
            lcd.print("+");
            lcd.print (multiplu);

            if (var_taste == 1) 
            {
                if(lumina+multiplu <= 255)
                {
                    lumina += multiplu ; // in cazul in care nu depasim val maxima a culori  , ridicam valoarea 
                }

                else                     
                {
                    lcd.setCursor(0,1);
                    lcd.print (text_dep[lb_sel]);
                }
            }

            else if (var_taste == 4)
            {
                if (lumina-multiplu >= 0 && lumina-multiplu < 255) // in cazul in care nu depasim val minima a culori  , scadem valoarea 
                {
                    lumina -= multiplu ;
                }

                else
                {
                lcd.setCursor(0,1);
                lcd.print (text_dep[lb_sel]);
                }
            
            }

            else if (var_taste == 5)
            {
                if (multiplu<100)
                {
                    multiplu *= 10 ; 
                }

                else 
                {
                    multiplu = 1 ;
                }
            }
        
        break;


        case 3 :
            lcd.setCursor (0, 0);
            lcd.print(text_sat[lb_sel]) ;
            lcd.print(saturatie) ;  
            lcd.setCursor (10,1);
            lcd.print("+");
            lcd.print (multiplu);
        
            
            if (var_taste == 1) 
            {
                if(saturatie+multiplu <= 255)
                {
                    saturatie += multiplu ; // in cazul in care nu depasim val maxima a culori  , ridicam valoarea 
                }

                else                     
                {
                    lcd.setCursor(0,1);
                    lcd.print (text_dep[lb_sel]);
                }
            }

            else if (var_taste == 4)
            {
                if (saturatie - multiplu >= 0 && saturatie-multiplu < 255) // in cazul in care nu depasim val minima a culori  , scadem valoarea 
                {
                    saturatie -= multiplu ;
                }

                else
                {
                    lcd.setCursor(0,1);
                    lcd.print (text_dep[lb_sel]);
                }
            
            }

            else if (var_taste == 5)
            {
                if (multiplu<100)
                {
                    multiplu *= 10 ; 
                }

                else 
                {
                    multiplu = 1 ;
                }
            }

        break;


        case 4 :
            lcd.setCursor (0, 0);
            lcd.print(text_poz[lb_sel]) ;
            lcd.print(led_pos) ;
            lcd.setCursor (10,1);
            lcd.print("+");
            lcd.print (multiplu);

                
                
            if (var_taste == 1) 
            {
                if(led_pos+multiplu <= 53)
                {
                    led_pos += multiplu ; // in cazul in care nu depasim val maxima a culori  , ridicam valoarea 
                }

                else                     
                {
                    led_pos =led_pos - 54 + multiplu ; 
                }
            }

            else if (var_taste == 4)
            {
                if(led_pos-multiplu >= 0 && led_pos-multiplu < NUMPIXELS/2) // in cazul in care nu depasim val minima a culori  , scadem valoarea 
                {
                    led_pos -= multiplu ;
                }

                else
                {
                    led_pos =led_pos + 54 - multiplu ; 
                }
                
            }

            else if(var_taste == 5)
            {
                if (multiplu<10)
                {
                    multiplu *= 10 ; 
                }
                else 
                {
                    multiplu = 1 ;
                }
            }

        break;
        
  
        case 5 :
            lcd.setCursor (0, 0);
            lcd.print(text_umple[lb_sel]) ;
            lcd.print(led_fill) ;
            lcd.setCursor (10,1);
            lcd.print("+");
            lcd.print (multiplu);

                
            if (var_taste == 1) 
            {
                if(led_fill+multiplu <= 54)
                {
                    led_fill += multiplu ; // in cazul in care nu depasim val maxima a culori  , ridicam valoarea 
                }

                else                     
                {
                    lcd.setCursor(0,1);
                    lcd.print (text_dep[lb_sel]);
                }

            }

            else if (var_taste == 4)
            {
                if (led_fill-multiplu > 0 && led_fill-multiplu < NUMPIXELS/2) // in cazul in care nu depasim val minima a culori  , scadem valoarea 
                {
                    led_fill -= multiplu ;
                }

                else
                {
                    lcd.setCursor(0,1);
                    lcd.print (text_dep[lb_sel]);
                }
                
            }

            else if(var_taste == 5)
            {
                if(multiplu<10)
                {
                    multiplu *= 10 ; 
                }

                else 
                {
                    multiplu = 1 ;
                }
                
            }

        break ;

        
        case 6 :
            lcd.setCursor (0, 0);
            lcd.print(text_viteza[lb_sel]) ;
            lcd.print(viteza) ;
            lcd.setCursor (10,1);
            lcd.print("+");
            lcd.print (multiplu);

            if (var_taste == 1) 
            {
                viteza += multiplu ; 
            }
            else if (var_taste == 4)
            {
                viteza -= multiplu ;
            }

            else if (var_taste == 5)
            {
                if (multiplu<10000)
                {
                    multiplu *= 10 ; 
                }

                else 
                {
                    multiplu = 1 ;
                }

            }

        break ;


        case 7 :
            lcd.setCursor (0, 0);
            lcd.print(text_inatzie[lb_sel]) ;
            lcd.print(intarziere) ;
            lcd.setCursor (10,1);
            lcd.print("+");
            lcd.print (multiplu);

                
            if (var_taste == 1) 
            {
                    intarziere += multiplu ; // in cazul in care nu depasim val maxima a culori  , ridicam valoarea 
            }
            else if (var_taste == 4)
            {
                    intarziere -= multiplu ;
            }
            else if (var_taste == 5)
            {
                if (multiplu<10000)
                {
                    multiplu *= 10 ; 
                }

                else 
                {
                    multiplu = 1 ;
                }

            }

        break ;


        case 8 :
            lcd.setCursor (0, 0);
            lcd.print(text_lb[lb_sel] ) ;
            lcd.setCursor (0,1);
            lcd.print(text_Nume_lb[lb_sel] ) ;

                
            if (var_taste == 1 && lb_sel < 1) 
            {
                lb_sel ++ ;
            }
            
            else if (var_taste == 4 && lb_sel > 0)
            {
                lb_sel -- ;
            }

        break ;
    

        case 9 :
            static int var_about ;

            if (var_taste == 1 && var_about < 3) 
            {
                var_about ++ ; 
            }

            else if (var_taste == 4 && var_about > 0 )
            {
                var_about -- ;
            }
        
            lcd.setCursor (0, 0);

            if(var_about == 1 )
                lcd.print(text_ultima_m_i[lb_sel]);

            else if (var_about == 2 )
                lcd.print(text_ultima_m_l[lb_sel]);

            else if (var_about == 3 )
                lcd.print(text_vers_curenta[lb_sel]);

            else           
                lcd.print(text_despre[lb_sel]) ; 

            lcd.setCursor (0,1);

            if(var_about == 1 )
                lcd.print(__DATE__);    
            else if (var_about == 2 )
                lcd.print(__TIME__);
            else if (var_about == 3)
                lcd.print(text_nr_vers[lb_sel]);
            else           
                lcd.print(text_apasa[lb_sel]) ;


        break ;


        case 10 :

            lcd.setCursor (0, 0);
            lcd.print(text_px_cl[lb_sel] ) ;
            if(pix_cle)
            {
                lcd.print(":ON");
            }
            else
            {
                lcd.print(":OFF");
            }

                
            if (var_taste == 1 && pix_cle < 1) 
            {
                pix_cle = 1 ;
            }
            
            else if (var_taste == 4 && pix_cle > 0)
            {
                pix_cle = 0 ;
            }

        break;
        

    
        default :
            lcd.clear();
            lcd.setCursor (0, 0);
            lcd.print("eroare") ;
        break;

    }
 
    return 0 ; 

}

