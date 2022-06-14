#include <Arduino.h>

  // ____ librari incluse 
  //  #include <LiquidCrystal_I2C.h> // Librarie LCD i2c
  #include <LiquidCrystal.h>      // Livrarie LDC simplu
  #include <Adafruit_NeoPixel.h> // Librarie Banda NeoPixel
  
  // definiti pentru functi 
  //#define VAR_ASTEAPTA unsigned long int

  // ____ setrai ale librariilor 

  // __LCD i2c
  //  LiquidCrystal_I2C lcd (0x27, 16,2); // stabilim adresa si dimensiunea LCD
  //__ LCD simplu
  LiquidCrystal lcd(8,9,4,5,6,7); 
    
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
  int32_t culoare = 0  ; // culoare // ******in caz de eroare punem unsigned long*****8
  int lumina = 55 ;// sabilim luminozitatea pixelilor
  int saturatie = 255 ; // saturatia culori
  int led_pos = 0 ; // pozitia ledului aprins
  int led_fill = 54 ; // cate leduri sunt umplute dupa led pos
  int viteza = 1; // stabilim o variabila pt viteza
  int intarziere = 100 ; // stabilim o variabila pt delay 
  //
  int jocPos=1; // variabila pt selectare jocuri
  int op_disp [11] ; 
  int nr_optiuni_alocat = 11 ;  // numarul de setari alocate de joc 
  char* NumeJoc; // stabilim o variabila pentru numele jocului actual
  const int nrJocuri = 4 ; // Stabilim numarul de jocuri pe care le avem !!! se va modifica de fiecare data cand creem un joc !!!
  int multiplu =1; // variabila creata pt a modifica mai usor setarile 
  int sel_settings = 0; // variabila pentru selectarea si afisarea setari
  
  // ____ prototipuri functi
 
  //input de la taste
  char taste (void) ;
  //afisarea meniului de setari 
  int meniu (void) ;
  // functia pentru schimbarea  jocurilor de lumini
  void jocuri (int select);
  // functie asteapta
  //int asteapta (int timp , unsigned long previousMillis ) ;
  
  //functie control leduri---culoare---saturatie----luminozitate---pozitie led --- led fill
  //necesara pentru a simplifica scrierea jocurilor pt ambele parti ale benzi de leduri
  void bandaLed (unsigned long col , unsigned sat , unsigned lux , unsigned pos , unsigned fil ) ; 
  void set_setting_available (int od1, int od2, int od3, int od4, int od5, int od6, int od7, int od8, int od9, int od10);


  int var_taste = 0 ;


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

    }
  
  void loop() 
    {

     
    jocuri(jocPos); // incepem cu primul joc
    
    unsigned long timp_curent_meniu = millis();  // creem o functie de delay care nu blocheaza procesorul 
    static unsigned long delay_meniu ; 
    if (timp_curent_meniu - delay_meniu >= 100 )  // avem nevoie de delay pt taste si lcd clear
        {
        taste();
        
        delay_meniu = timp_curent_meniu;
        }
    else 
    {
      var_taste = 0 ;
    }

   meniu ();
    }


//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------



      char taste (void) 
  {
         
          
         int val_tasta = analogRead (0);  

     
         if      (val_tasta < 60)  
             {
                 var_taste = 1  ;
              
               // Dreapta
             }
         else if (val_tasta < 200)  
             {

                 var_taste = 2 ;
                
              // Sus
             }
         else if (val_tasta < 400) 
             {
                 var_taste = 3 ;
                
              // Jos
             }
         else if (val_tasta < 600)   
             {
                 var_taste = 4 ;

              // Stanga
             }
         else if (val_tasta < 800)  
             {
 
                 var_taste = 5 ;
                
               // Select
             }
         else                       
             {
              var_taste = 0 ; 
              // Stare initiala
             }
       
  }    
 


//----------------------------------------------------------------------------------------
  

/*  // exemplu folosire functie asteapta 
*   
*    VAR_ASTEAPTA variabila ; 
*    if (asteapta (interval , variabila ))
*       {variabila=asteapta ( interval ,variabila);  
*       
*       // aici scri ce vrei sa faca la un interval de timp  
*
*       }
*/    
 // int asteapta ( int timp , unsigned long previousMillis  ) 
 // {
 //    //static unsigned long previousMillis ;
 //   unsigned long currentMillis = millis();
 // if (currentMillis - previousMillis >= timp )
 //     {
  //    previousMillis = currentMillis;
  //    return previousMillis ;
 //    }
 //     else return 0 ;
       
 // }

//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------

void bandaLed (unsigned long col , unsigned sat , unsigned lux , unsigned pos , unsigned fil ) 
    { 
     pixels.clear();

     // conditionam pozitia ledurilor aprinse pentru a crea o singura bucla
     // in cazul in care se ajuge la capat , o sa continue de la inceput , creen fenomenul de cerc
     if ( led_fill + led_pos > (NUMPIXELS/2) )
        {  
        pixels.fill( pixels.ColorHSV(col , sat , lux ) , pos , (NUMPIXELS/2) - pos);
        pixels.fill( pixels.ColorHSV(col , sat , lux ) , 0 , fil-((NUMPIXELS/2)-pos)); 
        }
     else
        {
        pixels.fill( pixels.ColorHSV(col , sat , lux ) , pos , fil); 
        }
        
     if ( led_fill + led_pos > (NUMPIXELS/2) )
        {
        pixels.fill( pixels.ColorHSV(col , sat , lux) , pos + (NUMPIXELS/2) , (NUMPIXELS/2) - pos);
        pixels.fill( pixels.ColorHSV(col , sat , lux ) , (NUMPIXELS/2) , fil-((NUMPIXELS/2)-pos)); 
        }
     else
        {
        pixels.fill( pixels.ColorHSV(col , sat , lux ) , pos +(NUMPIXELS/2) , fil);
        }

    pixels.show(); 
    } 
          
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------

  void set_setting_available (int od1, int od2, int od3, int od4, int od5, int od6, int od7, int od8, int od9, int od10)
      {
      op_disp [0] = on ;  
      op_disp [1] = od1;
      op_disp [2] = od2;
      op_disp [3] = od3;
      op_disp [4] = od4;
      op_disp [5] = od5;
      op_disp [6] = od6;
      op_disp [7] = od7;
      op_disp [8] = od8;
      op_disp [9] = od9;
      op_disp [10] = od10;
      }




//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------

  /* optiunile disponibile sunt
   * 0 - joc // nu este modificabila
   * 1 - culoare
   * 2 - lumina
   * 3 - saturatie
   * 4 - pozitie led
   * 5 - nr of led fill after led pos
   * 6 - viteza
   * 7 - intarziere
   * 8 -
   * 9 - 
   * 10-
   */

  void jocuri (int select)
  {
    
  switch (select)
     {
     case 0:
     //--------------------- 1    2    3    4    5    6    7   8    9    10    
     set_setting_available (off, off, off, off, off, off, off, on, off ,off);
     
     NumeJoc = "Setting" ;

     break;
//======================================================//
     case 1 :
     //--------------------- 1   2   3   4   5   6   7   8    9    10 
     set_setting_available (on, on, on, on, on, on, on, on, off ,off);
     
     bandaLed (culoare , saturatie , lumina , led_pos , led_fill) ; 

     NumeJoc = "Jocu1" ;
     break;
//======================================================//     
     case 2 :
     //--------------------- 1    2    3    4   5   6   7   8    9    10 
     set_setting_available (off, off, off, on, on, on, on, on, off ,off);
     
     culoare += viteza ;
         if ( culoare > 65536 )
         {
          culoare = 0 ; 
         }
     else if ( culoare < 0 )
         {
          culoare = 65536 ;  
         }

           
     bandaLed (culoare , saturatie , lumina , led_pos , led_fill) ;
     
     NumeJoc = "Jocu2" ;
     break;
//======================================================//     
     case 3 :
     //--------------------- 1   2   3   4   5   6   7   8    9    10 
     set_setting_available (on, on, on, on, on, off, off, off, off ,off);
         
     bandaLed (culoare , saturatie , lumina , led_pos , led_fill) ;
     
     NumeJoc = "Jocu3" ; 
     break;
//======================================================// 



  
     }
  
  
  }

//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------

  
  int meniu (void)
  {
//  lcd.setCursor(0,1);
//  lcd.print (var_taste);

  if( var_taste != 0 )
  {
    lcd.clear(); // de verificat , are un mic bug 
  }
  
      
  int last_sel_settings = sel_settings ;
  
  if ( var_taste == 3 ) 
      {
      if (sel_settings  < nr_optiuni_alocat-1)
          {
          sel_settings  ++ ;

          while (op_disp [sel_settings ] == off )
              {
              sel_settings  ++ ;
              if (sel_settings == nr_optiuni_alocat-1 && op_disp [sel_settings ] == off) 
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
//======================================================//  
    case 0 :
    
      lcd.setCursor (0, 0);
      if (jocPos !=0 ) lcd.print("Joc:") ;
      lcd.setCursor (4, 0);
      lcd.print (NumeJoc) ;

      // citire taste   
      if (var_taste == 1) 
          {
          if(jocPos<nrJocuri) jocPos++ ; // in cazul in care nu depasim nr de jocuri , trecem la urmatorul 
          }
      else if (var_taste == 4)
          {
          if (jocPos>0) jocPos-- ;
          } 
          
    break;
//======================================================//    
    case 1 :
    
      lcd.setCursor (0, 0);
      lcd.print("Culoare:");
      lcd.print (culoare) ;
      lcd.setCursor (10,1);
      lcd.print("+");
      lcd.print (multiplu);
      
      if (var_taste == 1) 
          {
            if(culoare+multiplu <= 65535)
                {
                culoare += multiplu ; // in cazul in care nu depasim val maxima a culori  , ridicam valoarea 
                }
            else                     
                {
                culoare = culoare - 65536 + multiplu ;
                }
          }
      else if (var_taste == 4)
         {
          if (culoare-multiplu >= 0) // in cazul in care nu depasim val minima a culori  , scadem valoarea 
              {
              culoare -= multiplu ;
              }
          else
              {
              culoare =culoare + 65536 - multiplu ;
              }
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
//======================================================//    
    case 2 :
      lcd.setCursor (0, 0);
      lcd.print("lumina:") ;
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
                lcd.print ("over val");
                }
          }
      else if (var_taste == 4)
         {
          if (lumina-multiplu >= 0) // in cazul in care nu depasim val minima a culori  , scadem valoarea 
              {
              lumina -= multiplu ;
              }
          else
              {
              lcd.setCursor(0,1);
              lcd.print ("over val");
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
//======================================================//    
    case 3 :
      lcd.setCursor (0, 0);
      lcd.print("Saturatie:") ;
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
                lcd.print ("over val");
                }
          }
      else if (var_taste == 4)
         {
          if (saturatie - multiplu >= 0) // in cazul in care nu depasim val minima a culori  , scadem valoarea 
              {
              saturatie -= multiplu ;
              }
          else
              {
              lcd.setCursor(0,1);
              lcd.print ("over val");
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
//======================================================//   
    case 4 :
      lcd.setCursor (0, 0);
      lcd.print("Pozitie:") ;
      lcd.print(led_pos) ;
      lcd.setCursor (10,1);
      lcd.print("+");
      lcd.print (multiplu);

     // if (led_pos > 53)
        //  led_pos = 0 ;
     // else if (led_pos < 0)
      //    led_pos = 53 ; 
          
        
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
          if (led_pos-multiplu >= 0) // in cazul in care nu depasim val minima a culori  , scadem valoarea 
              {
              led_pos -= multiplu ;
              }
          else
              {
              led_pos =led_pos + 54 - multiplu ; 
              }
          
          }
      else if (var_taste == 5)
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
    
//======================================================//  
    case 5 :
      lcd.setCursor (0, 0);
      lcd.print("Umple:") ;
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
                lcd.print ("over val");
                }
          }
      else if (var_taste == 4)
         {
          if (led_fill-multiplu > 0) // in cazul in care nu depasim val minima a culori  , scadem valoarea 
              {
              led_fill -= multiplu ;
              }
          else
              {
              lcd.setCursor(0,1);
              lcd.print ("over val");
              }
          
          }
      else if (var_taste == 5)
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
    break ;
    

//======================================================//

    case 6 :
      lcd.setCursor (0, 0);
      lcd.print("Viteza:") ;
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

//======================================================//
    case 7 :
      lcd.setCursor (0, 0);
      lcd.print("Intarziere:") ;
      lcd.print(intarziere) ;
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
                lcd.print ("over val");
                }
          }
      else if (var_taste == 4)
         {
          if (lumina-multiplu >= 0) // in cazul in care nu depasim val minima a culori  , scadem valoarea 
              {
              lumina -= multiplu ;
              }
          else
              {
              lcd.setCursor(0,1);
              lcd.print ("over val");
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
    break ;


 
//======================================================//
    case 8 :
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
          lcd.print("Last modify on:");
      else if (var_about == 2 )
          lcd.print("Last modify at:");
      else if (var_about == 3 )
          lcd.print("Curent version:");
      else           
          lcd.print("ABOUT CONTROLLER") ; 

      lcd.setCursor (0,1);
      if(var_about == 1 )
          lcd.print(__DATE__);    
      else if (var_about == 2 )
          lcd.print(__TIME__);
      else if (var_about == 3)
          lcd.print("    v0.3.04");
      else           
          lcd.print("Pres + or  -" ) ;

    break ;
//======================================================//
   
    default :
      lcd.clear();
      lcd.setCursor (0, 0);
      lcd.print("eroare") ;
    break;
    }
//======================================================//





 return 0 ; 
}