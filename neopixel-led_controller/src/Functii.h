
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


