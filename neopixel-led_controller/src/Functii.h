
int taste (void) 
{
    int val_tasta = analogRead (0);  

     
    if      (val_tasta < 60)  
    {
        return 1  ;
        // Dreapta
    }
    
    else if (val_tasta < 200)  
    {
        return 2 ;
        // Sus
    }

    else if (val_tasta < 400) 
    {
        return 3 ;
        // Jos
    }

    else if (val_tasta < 600)   
    {
        return 4 ;
        // Stanga
    }

    else if (val_tasta < 800)  
    {
        return 5 ;   
        // Select
    }

    else                       
    {
        return 0 ; 
        // Stare initiala
    }

}    
 




void bandaLed (unsigned long col , unsigned sat , unsigned lux , unsigned pos , unsigned fil ) 
{ 
    pixels.clear();

    // conditionam pozitia ledurilor aprinse pentru a crea o singura bucla
    // in cazul in care se ajuge la capat , o sa continue de la inceput , creen forma de cerc
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


