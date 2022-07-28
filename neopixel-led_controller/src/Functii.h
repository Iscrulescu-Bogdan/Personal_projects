
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
 



 void two_circle (uint16_t col , uint8_t sat , uint8_t lux , uint32_t pos , uint32_t fil , uint32_t nr_pixel ,bool pixel_clear) 
{ 
    if(pixel_clear) 
    {
        pixels.clear();
    }
    // conditionam pozitia ledurilor aprinse pentru a crea o singura bucla
    // in cazul in care se ajuge la capat , o sa continue de la inceput , creen forma de cerc
    if ( fil + pos > nr_pixel/2 )
    {  
        pixels.fill( pixels.ColorHSV(col , sat , lux ) , pos , (nr_pixel/2) - pos);
        pixels.fill( pixels.ColorHSV(col , sat , lux ) , 0 , fil-((nr_pixel/2)-pos)); 

        pixels.fill( pixels.ColorHSV(col , sat , lux) , pos + (nr_pixel/2) , (nr_pixel/2) - pos);
        pixels.fill( pixels.ColorHSV(col , sat , lux ) , (nr_pixel/2) , fil-((nr_pixel/2)-pos));
    }

    else
    {
        pixels.fill( pixels.ColorHSV(col , sat , lux ) , pos , fil);

        pixels.fill( pixels.ColorHSV(col , sat , lux ) , pos +(nr_pixel/2) , fil);
    }
       pixels.show();     
}         




void set_setting_available (bool od1, bool od2, bool od3, bool od4, bool od5, bool od6, bool od7, bool od8, bool od9, bool od10)
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


int lim_plus (int val , int val2 , int min , int max )
{
    if (val+val2 > max) return min+val2-val ;
    else return val+val2 ; 
}

int lim_minus (int val , int val2  , int min , int max )
{
    if (val-val2 < min) return max+val-val2 ;
    else return val-val2 ; 
}

void serail_usb (void)
{

    static char message [20];
    static char message2[20];
    static int count_serial = 0 ;
    char incomingByte = 0 ;
    uint32_t valoare = 0 ;

        incomingByte = Serial.read() ;
        message[count_serial] = incomingByte;
        if (count_serial >= 20 )
        {
            count_serial = 0 ;
            Serial.print ("Lungime mesaj depasita !");
        //    break;
        }
        else
        {
            count_serial ++ ;
        }

        Serial.print(incomingByte);
    

    for (int i = 0; i<15 ; i++ )
    {
        message2[i]=message[i+3];
    }


    if (incomingByte == '.')
    {   
        Serial.println("");      
        Serial.print("Mesajul primit este :  "); 
        Serial.println(message);
        count_serial=0;
        valoare = atoi (message2);
        set_var (message , valoare);
        Serial.print ("Valoarea este : ");
        Serial.println (valoare);

        for (int i = 0; i < 20; i++)
        {
            message[i]= '\0';
            message2[i]='\0' ; 
        } 


    }

}

void set_var (char* set , int val)
{
    if (strncmp(set , "cul",3) == 0 )
    {
        culoare = val ;
    }

    else if (strncmp(set , "lum",3) == 0 )
    {
        lumina = val ; 
    }

    else if (strncmp(set , "sat",3) == 0 )
    {
        saturatie = val ;
    }

    else if (strncmp(set , "pos",3) == 0 )
    {
        led_pos = val ;
    }

    else if (strncmp(set , "fil",3) == 0 )
    {
        led_fill = val ;
    }

    else if (strncmp(set , "vit",3) == 0 )
    {
        viteza = val ;
    }

    else if (strncmp(set , "int",3) == 0 )
    {
        intarziere = val ;
    }

    else if (strncmp(set , "joc",3) == 0 )
    {
        jocPos = val ;
    }

    else if (strncmp(set , "tas",3) == 0 )
    {
        var_taste = val ;       
    }

}