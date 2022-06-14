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
    
  // stabilim numele benzi de leduri
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
  // ____ variabile globale
  int32_t culoare = 0  ; // culoare // ******in caz de eroare punem unsigned long*****8
  int lumina = 255 ;// sabilim luminozitatea pixelilor
  int saturatie = 255 ; // saturatia culori
  int led_pos = 0 ; // pozitia ledului aprins
  int led_fill = 53 ; // cate leduri sunt umplute dupa led pos
  int viteza = 1; // stabilim o variabila pt viteza
  int intarziere = 100 ; // stabilim o variabila pt delay 
  //
  int jocPos=0; // variabila pt selectare jocuri
  int optiuni_disponibile [] = {0,1,2,3,4,5,6,7} ; // numarul total de setari disponibile !!! se va modifica de fiecare data cand creem o setare !!!
  int nr_optiuni_alocat ;  // numarul de setari alocate de joc 
  char* NumeJoc; // stabilim o variabila pentru numele jocului actual
  const int nrJocuri = 3 ; // Stabilim numarul de jocuri pe care le avem !!! se va modifica de fiecare data cand creem un joc !!!
  int multiplu =1; // variabila creata pt a modifica mai usor setarile 
  int sel_seting = 0; // variabila pentru selectarea si afisarea setari
  
  // ____ prototipuri functi
 
  //input de la taste
  int taste (void) ;
  //afisarea meniului de setari 
  int meniu (void) ;
  // functia pentru schimbarea  jocurilor de lumini
  void jocuri (int select);
  // functie asteapta
  //int asteapta (int timp , unsigned long previousMillis ) ;



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
    lcd.setCursor (0,1);
    lcd.print ("Controler Vs 1.0");
    delay(2000);
    lcd.clear();

    }
  
  void loop() 
    {
     
     
    jocuri(jocPos); // incepem cu primul joc
    
    unsigned long timp_curent_meniu = millis();
    static unsigned long delay_meniu ; 
    if (timp_curent_meniu - delay_meniu >= 80 )
        {
        meniu();
        delay_meniu = timp_curent_meniu;
        }
  
    }

  int taste (void) 
        {

         int val_tasta = analogRead (0);

            
         if      (val_tasta < 60)  
             {
              return 1 ; // Dreapta
             }
         else if (val_tasta < 200)  
             {
              return 2 ;// Sus
             }
         else if (val_tasta < 400) 
             {
              return 3 ; // Jos
             }
         else if (val_tasta < 600)   
             {
              return 4 ; // Stanga
             }
         else if (val_tasta < 800)  
             {
              return 5 ; // Select
             }
         else                       
             {
              return 0 ; // Stare initiala
             }
            
        }

  /* optiunile disponibile sunt
   * 0 - joc // nu este modificabila
   * 1 - culoare
   * 2 - lumina
   * 3 - saturatie
   * 4 - pozitie led
   * 5 - nr of led fill after led pos
   * 6 - viteza
   * 7 - intarziere
   * 
   */
  

  
  
  
  void jocuri (int select)
  {

  switch (select)
     {
//======================================================//
     case 0 :
     nr_optiuni_alocat = 6 ; 
     //optiuni_disponibile [0] = 0; este alocata automat si nu se va modifica 
     optiuni_disponibile [1] = 1;
     optiuni_disponibile [2] = 2;
     optiuni_disponibile [3] = 3;
     optiuni_disponibile [4] = 4;
     optiuni_disponibile [5] = 5;
     
     pixels.clear();

   
        
     pixels.fill( pixels.ColorHSV(culoare , saturatie , lumina ) , led_pos , led_fill);
     pixels.fill( pixels.ColorHSV(culoare , saturatie , lumina ) , led_pos +54 , led_fill+54);
    
     pixels.show(); 
     NumeJoc = "Jocu1" ;
     break;
//======================================================//     
     case 1 :
     nr_optiuni_alocat = 6 ;
     optiuni_disponibile [1] = 2;
     optiuni_disponibile [2] = 3;
     optiuni_disponibile [3] = 4;
     optiuni_disponibile [4] = 5;
     optiuni_disponibile [5] = 6;

     while (select == 1 )
        {
        culoare += viteza ;
        break;
        }    
     
     pixels.clear();
  
     pixels.fill( pixels.ColorHSV(culoare , saturatie , lumina ) , led_pos , led_fill);
     pixels.fill( pixels.ColorHSV(culoare , saturatie , lumina ) , led_pos +54 , led_fill+54);
    
     pixels.show(); 
     NumeJoc = "Jocu2" ;
     break;
//======================================================//     
     case 2 :
     nr_optiuni_alocat =2 ;
     optiuni_disponibile [0] = 0;
     optiuni_disponibile [1] = 2;

     
     pixels.clear();
  
     pixels.fill( pixels.ColorHSV(culoare , saturatie , lumina ) , led_pos , led_fill);
     pixels.fill( pixels.ColorHSV(culoare , saturatie , lumina ) , led_pos +54 , led_fill+54);
    
     pixels.show(); 
     NumeJoc = "Jocu3" ; 
     break;
//======================================================//   
     }
  
  



  return 0 ;
  }   

  int meniu (void)
  {
      lcd.clear();

  if ( taste() == 3 ) 
      {
      if (sel_seting  < nr_optiuni_alocat-1)
          {
          sel_seting  ++ ;

          }
      }
      else if (taste() == 2)
      {
      if(sel_seting  >0 ) 
          {
          sel_seting  -- ; 
          }
       
      } 
    
  switch ( optiuni_disponibile[sel_seting ]) 
    {
//======================================================//  
    case 0 :
    
      lcd.setCursor (0, 0);
      lcd.print("Joc:") ;
      lcd.setCursor (4, 0);
      lcd.print (NumeJoc) ;

      // citire taste   
      if (taste() == 1) 
          {
          if(jocPos<nrJocuri) jocPos++ ; // in cazul in care nu depasim nr de jocuri , trecem la urmatorul 
          }
      else if (taste() == 4)
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
      
      if (taste() == 1) 
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
      else if (taste() == 4)
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
      else if (taste() == 5)
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

        
      if (taste() == 1) 
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
      else if (taste() == 4)
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
      else if (taste() == 5)
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
      
        
      if (taste() == 1) 
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
      else if (taste() == 4)
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
      else if (taste() == 5)
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
          
        
      if (taste() == 1) 
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
      else if (taste() == 4)
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
      else if (taste() == 5)
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

        
      if (taste() == 1) 
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
      else if (taste() == 4)
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
      else if (taste() == 5)
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

    case 6 :
      lcd.setCursor (0, 0);
      lcd.print("Viteza:") ;
      lcd.print(viteza) ;
      lcd.setCursor (10,1);
      lcd.print("+");
      lcd.print (multiplu);

        
      if (taste() == 1) 
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
      else if (taste() == 4)
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
      else if (taste() == 5)
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
    case 7 :
      lcd.setCursor (0, 0);
      lcd.print("Intarziere:") ;
      lcd.print(intarziere) ;
      lcd.setCursor (10,1);
      lcd.print("+");
      lcd.print (multiplu);

        
      if (taste() == 1) 
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
      else if (taste() == 4)
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
      else if (taste() == 5)
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
   
    default :
      lcd.clear();
      lcd.setCursor (0, 0);
      lcd.print("eroare") ;
    break;
    }
//======================================================//





 return 0 ; 
}  