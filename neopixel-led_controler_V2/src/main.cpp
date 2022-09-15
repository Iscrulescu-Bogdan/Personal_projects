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
##      Version v0.2.11   Project still in progres              ##
##                                                              ##
##################################################################
*/

// ____ librari incluse 
    //  #include <LiquidCrystal_I2C.h> // Librarie LCD i2c
    #include <LiquidCrystal.h>      // Librarie LDC simplu
    #include <Adafruit_NeoPixel.h> // Librarie Banda NeoPixel
    #include "Language_controller_V2.h" // includem fisierul cu dictionarul de limba
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
    #define NR_SETTING 20
    
    // stabilim numele benzi de leduri
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


//
    const int nr_optiuni_disponibile = 11 ;  // numarul de setari disponibile           !!! se modifica in  cazul adaugari unei setari 
    int op_disp [nr_optiuni_disponibile] ; 
    char* NumeJoc ; // stabilim o varS pentru numele jocului actual
    const int nrJocuri = 10 ; // Stabilim numarul de jocuri pe care le avem              !!! se va modifica de fiecare data cand creem un joc !!! 
    uint8_t var_taste = 0 ;
    int setare_set = 0 ;
    int multiplu_set = 1 ;
    
    bool count = 0 ;


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
    void initializare (void);
    void afisareDisplay ( char *text_a, uint32_t var_a, int multiplu_a ,int txt2, char *text2_a);
    void control (int comanda ,int &setare_c , int nr_setari, uint32_t &var_c, int min_var , uint32_t max_var , int &multiplu_c, int multiplu_max);


//

struct parametri 
{
   
   char* text_limba [2];
   uint32_t varS ;
   int min ;
   uint32_t max ;
   int multipluMax ;
   //char* text_2 [2] ; 

}set[NR_SETTING];


#include "Functii_V2.h" // includem fisierul cu functii 





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
   jocuri(set[0].varS); // incepem cu primul joc

    static int statusx = 1 ;
    if(statusx)
    {
        initializare();
        statusx -- ;
    }

    // creem o functie de delay care nu blocheaza procesorul 
    static unsigned long delay_tasta ; 
    if (millis() - delay_tasta >= 200 ) 
    {
        var_taste = taste();
        delay_tasta = millis();
    }
    else 
    {
        var_taste = 0 ;
    }



static unsigned long delay_meniu ;
        if (millis() - delay_meniu >= 100 )
    {
        lcd.clear();
        
        delay_meniu = millis();
    }



    
    
    control (var_taste , setare_set , NR_SETTING, set[setare_set].varS , set[setare_set].max ,  multiplu_set,  set[setare_set].multipluMax);

    afisareDisplay (set[setare_set].text_limba[set[8].varS], set[setare_set].varS, multiplu_set,setare_set, NumeJoc );//, set[setare_set].text_2);


    //if(Serial.available()>0)
    //{
    //    serail_usb ();
    //}
            //pixels.fill( pixels.ColorHSV(culoare , saturatie , set[2].varS ) , set[4].varS , set[5].varS);
            //  pixels.show();
//   meniu ();

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

            NumeJoc  = text_setari[set[8].varS] ;
        break;


        case 1 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1   2   3   4   5   6   7   8    9    10 
                set_setting_available (on, on, on, on, on, on, on, off, off ,on);
                set[9].varS = on ;
                set[3].varS = 255;
                set[4].varS = 0 ;
                set[5].varS = NUMPIXELS/2 ;

                count=0 ; // inchidem while
                break; // doar pentru siguranta

            }

     
            two_circle (set[1].varS, set[3].varS, set[2].varS, set[4].varS, set[5].varS, NUMPIXELS, set[9].varS); 

            NumeJoc  = (char*)"Jocul 1" ;
        break;

    
        case 2 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1    2    3    4   5   6   7   8    9    10 
                set_setting_available (off, off, off, on, on, on, on, on, off ,on);
                set[9].varS = on ;
                set[3].varS = 255;
                set[6].varS=50;
                set[4].varS = 0 ;
                set[5].varS = NUMPIXELS/2 ;

                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }     
            set[1].varS += set[6].varS ;

            two_circle (set[1].varS, set[3].varS, set[2].varS, set[4].varS, set[5].varS, NUMPIXELS, set[9].varS); 
     
            NumeJoc = (char*)"Jocul 2" ;
        break;

    
        case 3 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1   2   3   4   5   6   7   8    9    10 
                set_setting_available (on, on, on, on, on, on, on, off, off ,on);
                set[9].varS = on ;
                set[3].varS = 255;
                set[7].varS = 50 ;
                set[6].varS = 50 ; 
                set[4].varS = 0 ;
                set[5].varS = NUMPIXELS/4 ;
                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }

            set[1].varS += set[6].varS ;



            // creem o functie de delay care nu blocheaza procesorul 
            static unsigned long delay_j3 ; 
            if (millis() - delay_j3 >= set[7].varS )  
            {
                if (set[4].varS<(NUMPIXELS/2))
                set[4].varS ++ ;
                else 
                set[4].varS = 0 ;

                delay_j3 = millis();
            }
             


            two_circle (set[1].varS, set[3].varS, set[2].varS, set[4].varS, set[5].varS, NUMPIXELS, set[9].varS); 
     
            NumeJoc = (char*)"Jocul 3" ; 
        break;


        case 4 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1   2   3   4   5   6   7   8    9    10 
                set_setting_available (on, on, on, on, on, on, on, off, off ,on);
                set[9].varS = on ;
                set[3].varS = 255;
                set[7].varS = 50 ;
                set[6].varS = 50 ; 
                set[4].varS = 0 ;
                set[5].varS = 1 ;

                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }

            static int led_pos_j4  ;
            set[1].varS += set[6].varS ;
            
            static unsigned long delay_j4 ; 
            if (millis() - delay_j4 >= set[7].varS ) 
            {   
                
                if (set[5].varS >= NUMPIXELS / 2 )
                {
                    led_pos_j4 = set[4].varS ;
                    set[5].varS= 1 ;
                    set[1].varS += 200;
                }
                else
                {
                    led_pos_j4 = lim_minus (led_pos_j4 , 1 , 0 , (NUMPIXELS/2)-1) ;
                    set[5].varS +=2;
                }

                delay_j4 = millis();
            }
             

            two_circle (set[1].varS, set[3].varS, set[2].varS, led_pos_j4, set[5].varS, NUMPIXELS, set[9].varS); 
     
            NumeJoc = (char*)"Jocul 4" ;

        break;
        
        case 5 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1   2   3   4   5   6   7   8    9    10 
                set_setting_available (off, on, off, off, off, off, on, off, off ,off);
                set[9].varS = on ;
                set[3].varS = 255;
                set[7].varS = 100;
                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }

            static int x = 0 ;
            static unsigned long delay_j5 ; 
            if (millis() - delay_j5 >= set[7].varS )  
            {
                if(x<12)
                x++;
                else
                x=0;

                delay_j5 = millis();
            }
            two_circle (CULORI[x], set[3].varS, set[2].varS, 0 , NUMPIXELS/2, NUMPIXELS, set[9].varS); 
     
            NumeJoc = (char*)"Jocul 5" ;
        break;

        case 6 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1   2   3   4   5   6   7   8    9    10 
                set_setting_available (off, on, off, off, off, off, on, off, off ,off);
                set[3].varS = 255;
                set[7].varS=1;

                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }

            static uint16_t led_pos_j6 = 0 ; 
            static unsigned long delay_j6 ; 
            if (millis() - delay_j6 >= set[7].varS )  
            {   
                if (set[4].varS < (NUMPIXELS/2)-1)
                {
                    set[4].varS ++ ;
                }
                else
                {
                    set[4].varS=0;
                }
                if(set[4].varS< NUMPIXELS/4)
                {
                    led_pos_j6 = set[4].varS+NUMPIXELS/4 ;
                }
                else
                {
                    led_pos_j6 = set[4].varS-NUMPIXELS/4 ;
                }


                delay_j6 = millis();
            }

            two_circle (ROSU, set[3].varS, set[2].varS, set[4].varS, NUMPIXELS/4,NUMPIXELS ,off); 
            two_circle (ALBASTRU, set[3].varS, set[2].varS, led_pos_j6, NUMPIXELS/4,NUMPIXELS ,off); 

            NumeJoc = (char*)"POLICE" ;
        break;

        case 7 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
               //--------------------- 1   2   3   4   5   6   7   8    9    10 
                set_setting_available (off, on, off, off, off, off, on, off, off ,off);
                set[3].varS = 255;
                set[7].varS = 40 ;
                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }

            static uint8_t count_j7 ;

            static unsigned long delay_j7 ; 
            if (millis() - delay_j7 >= set[7].varS )  
            {   
                count_j7 ++ ;
                delay_j7 = millis();
            }  
                
            if ((count_j7 >= 0 && count_j7 <= 1 ) || ( count_j7 >= 4 && count_j7 <= 5 ))
            {
                two_circle (ROSU, set[3].varS, set[2].varS, 0, NUMPIXELS/4,NUMPIXELS ,off); 
                two_circle (ALBASTRU, set[3].varS, set[2].varS, NUMPIXELS/4, NUMPIXELS/4,NUMPIXELS ,off);

            }         
            else if (count_j7 >= 8 && count_j7 <= 9 )
            {
                two_circle (ALBASTRU, set[3].varS, set[2].varS, 0, NUMPIXELS/4,NUMPIXELS ,off); 
                two_circle (ROSU, set[3].varS, set[2].varS, NUMPIXELS/4, NUMPIXELS/4,NUMPIXELS ,off);

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
                set[5].varS = NUMPIXELS/4 ;
                set[7].varS= 50;

                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }

            static uint16_t culoare_j8;
            static uint16_t culoare2_j8;
            static int led_pos_j8 = 0;
            static int led_pos2_j8 = NUMPIXELS-1;

            static unsigned long delay_j8 ; 
            if (millis() - delay_j8 >= set[7].varS )  
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
            pixels.fill( pixels.ColorHSV(culoare_j8 , 255 , set[2].varS ) , led_pos_j8 , set[5].varS);
            pixels.fill( pixels.ColorHSV(culoare2_j8 , 255 , set[2].varS ) , led_pos2_j8 , set[5].varS);
            pixels.show();


            NumeJoc = (char*)"Jocul 8" ;
        break;

        case 9 :
            while(count) // creem o functie care va seta anumite variabile doar atunci cand se schimba jocul
            {
                //--------------------- 1   2   3   4   5   6   7   8    9    10
                set_setting_available (off, on, off, off, on, off, on, off, off ,on);
                set[7].varS = 50 ;
                set[5].varS=50;
                set[9].varS = on ; 

                count=0 ; // inchidem while
                break; // doar pentru siguranta
            }


            static uint16_t culoare_j9; 
            static int led_pos_j9 = 0;
            static int led_fill_j9 =1 ;

            static unsigned long delay_j9 ; 
            if (millis() - delay_j9 >= set[7].varS )  
            {
                culoare_j9 = random();
                led_pos_j9 = random (0,NUMPIXELS-1);
                led_fill_j9 = random (1,set[5].varS);

                delay_j9 = millis();
            }
            if(set[9].varS)
            {
                pixels.clear();
            }

            pixels.fill( pixels.ColorHSV(culoare_j9 , 255 , set[2].varS ) , led_pos_j9 , led_fill_j9);
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

                set[9].varS = on ;
                set[3].varS = 255;
                set[5].varS= 4;
                culoare_j10 = random();
                culoare2_j10 = random ();
                led_pos_j10 = 0 ;
                led_pos2_j10 = 10 ;
                count_j10 = 1 ;
                set[7].varS=30;
                intarziere_j10 = random(1,set[7].varS);
                intarziere2_j10 = random(1,set[7].varS);
    
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
                intarziere_j10 = random(1,set[7].varS);
                intarziere2_j10 = random(1,set[7].varS);
            }
            else if (count_j10 == 0 && led_pos_j10 -1 == led_pos2_j10 )
            {
                count_j10 = 1 ;
                culoare_j10 = random();
                culoare2_j10 = random ();
                intarziere_j10 = random(1,set[7].varS);
                intarziere2_j10 = random(1,set[7].varS);
            }

            two_circle (culoare_j10, set[3].varS, set[2].varS, led_pos_j10, set[5].varS, NUMPIXELS, off);
            two_circle (culoare2_j10, set[3].varS, set[2].varS, led_pos2_j10, set[5].varS, NUMPIXELS, off);
            if(set[9].varS)
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
void initializare (void)
{
//          nume romana , nume englezea , varS, min , max , multiplu  , text2  
set [0] ={(char*)"Joc:",(char*)"Game:", 1, 0, nrJocuri, 1};
set [1] ={(char*)"Culoare:",(char*)"Collor:", 0, 0, 65536, 10000};
set [2] ={(char*)"Stralucire:",(char*)"Brightnes:", 0, 0, 255, 100};
set [3] ={(char*)"Saturatie:",(char*)"Saturation", 0, 0, 255, 100};
set [4] ={(char*)"Pozitie:",(char*)"Position", 0, 0, NUMPIXELS/2, 10};
set [5] ={(char*)"Umplere:",(char*)"Fill:", 0, 0, NUMPIXELS/2, 10}; 
set [6] ={(char*)"Viteza:",(char*)"Speed:", 0, 0, 255, 1000};
set [7] ={(char*)"Intarziere:",(char*)"Delay:", 0, 0, 255, 1000};
set [8] ={(char*)"Limba:Romana    ",(char*)"Language:English", 0, 0, 1, 1 };
set [9] ={(char*)"Curatare led: ",(char*)"Pixel clear: ", 0, 0, 1, 1};
/*
set[10] ={(char*)"",(char*)"", 0, 0, 255, 1000};
set[11] ={(char*)"",(char*)"", 0, 0, 255, 1000};
set[12] ={(char*)"",(char*)"", 0, 0, 255, 1000};
set[13] ={(char*)"",(char*)"", 0, 0, 255, 1000};
set[14] ={(char*)"",(char*)"", 0, 0, 255, 1000};
set[15] ={(char*)"",(char*)"", 0, 0, 255, 1000}; 
set[16] ={(char*)"",(char*)"", 0, 0, 255, 1000};
set[17] ={(char*)"",(char*)"", 0, 0, 255, 1000};
set[18] ={(char*)"",(char*)"", 0, 0, 255, 1000};
set[19] ={(char*)"",(char*)"", 0, 0, 255, 1000};
*/
}