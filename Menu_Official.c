/*+-----------------------------------------------------------------------+*
             *|   Chuong trinh Dung de hien thi MENU dieu khien len LCD 16x2          |*
             *|   MENU co dang cay(TREE) 1 MENU chinh co the co nhieu MENU con        |*
             *|   DE DANG them vao MENU bat ky dau hay sua chua de su dung lai        |*
             *|***********************************************************************|*
             *| Chuong trinh duoc viet boi Pham Minh Tuan: SVDH Su Pham Ky Thuat HCMC |*
             *|      Moi thac mac xin lien he email: emailchotoi@yahoo.com            |*
             *|***********************************************************************|*
             *|   Phan cung ket noi voi 1 LCD nhu sau (Su dung LCD driver cua CCS C)  |*
             *|               PIC     |       LCD                                     |*
             *|              ----------------------                                   |*
             *|              RD4->7   |      D4->7                                    |*
             *|              RD0      |      EN                                       |*
             *|              RD1      |      RS                                       |*
             *|              RD2      |      RW                                       |*
             *|   va ket noi voi cac phim nhan nhu sau:                               |*
             *|              RB4   :  CANCEL                                          |*
             *|              RB5   :  ENTER                                           |*
             *|              RB6   :  DOWN                                            |*
             *|              RB7   :  UP                                              |*
             *+-----------------------------------------------------------------------+*/

            #include <18f4550.h>
            #device adc=10
            #fuses NOLVP, NOWDT, XT
            #use delay(clock=4000000)
            #include <lcd.c>

            /* Dinh nghia MA PHIM se duoc dung de dieu khien Menu 
             * UP - DOWN : Lua chon mot Muc trong Menu
             * ENTER: Neu co Menu co, thi hien thi Menu con, nguoc lai thi hanh lenh
             * CANCEL: Tro ve Menu cap bac truoc no
            */
            #define UP     0xB0
            #define DOWN   0x70
            #define ENTER  0xD0
            #define MENU   0xF0
            #define CANCEL 0xE0

            /* Dinh nghia gia tri CHI MUC cho Menu Chinh, Menu con, tien cho viec so sanh
             * O day su dung tat ca la 12 Dong MENU
             */
            #define ALL_MENU     8
enum ids{_BEGIN,_CAI_DAT,_NHIET_DO,_DIEN_AP,_THONG_TIN,_MODULE_1,_MODULE_2,_MODULE_3,_MODULE_4,};
            struct TypeMenu {
               int8 MenuID;
               int8 SubMenu;
               char MenuName[16];//17
            };
            int8 ContainMenu, STATE = _BEGIN, PAGE = 0, LEVEL[7], LevelPtr=0;
            Boolean  AccessENTER = TRUE, AccessCANCEL=TRUE;
            const struct TypeMenu MenuOfTuan[ALL_MENU]  
            ={{_CAI_DAT,_BEGIN,"Cai dat"},
{_NHIET_DO,_CAI_DAT,"Nhiet do"},
{_DIEN_AP,_CAI_DAT,"Dien ap"},
{_THONG_TIN,_BEGIN,"Thong tin"},
{_MODULE_1,_THONG_TIN,"Module 1"},
{_MODULE_2,_THONG_TIN,"Module 2"},
{_MODULE_3,_THONG_TIN,"Module 3"},
{_MODULE_4,_THONG_TIN,"Module 4"},
};struct typeMenu MenuPoiter[2];


            unsigned char SelectFrom(int8 from) {
             int8 i, NumMenu = 0,k=0, i0;                
               for(i=0; i<ALL_MENU; i++) {
                  if(MenuOfTuan[i].SubMenu == STATE) {
                     if(NumMenu == 0) i0 = i;
                     NumMenu++;
                     if(NumMenu>from && k < 2) {
                        MenuPoiter[k] = MenuOfTuan[i];
                        k++;
                     }
                     if(k == 1) MenuPoiter[1] = MenuOfTuan[i0];
                  }                                   
               }
               return NumMenu;
            }



            int8 QueryLastMenu() {
               int8 i;                          //Mang Menu dang duoc hien thi co SubMenu
               for(i=0; i<ALL_MENU; i++) {      //Giong nhau va giong voi Dinh danh(ID)tren no
                  if(MenuOfTuan[i].MenuID == MenuPoiter[0].SubMenu)  //mot bac
                     return MenuOfTuan[i].SubMenu;
               }
               return 0x00;
            }



            short HasSubMenu() {
               char i;
               for(i=0; i<ALL_MENU; i++) {            
                  if(STATE == MenuOfTuan[i].SubMenu)  
                     return TRUE;                     
               }                                      
               return FALSE;                         
            }



            void ProcessMenu() {
               if(PAGE > ContainMenu-1) PAGE = 0;
               if(PAGE == 0xFF) PAGE = ContainMenu - 2;
               ContainMenu = SelectFrom(PAGE);

                  printf(lcd_putc,"\f>%s",MenuPoiter[0].MenuName);
                  if(ContainMenu > 1)
                  printf(lcd_putc,"\n %s",MenuPoiter[1].MenuName);
              
            }



            void ProcessAction(long Key) {
               if(STATE == _BEGIN) AccessCANCEL = FALSE;  
               else  AccessCANCEL = TRUE;                
               switch(Key) {
                  case ENTER:
                  if(!AccessENTER) break;       
                     LEVEL[LevelPtr] = PAGE;    
                     LevelPtr++;               
                     STATE = MenuPoiter[0].MenuID; 
                     PAGE = 0;                 
                     ContainMenu = SelectFrom(0); 
                     if(ContainMenu) {
                        ProcessMenu();          
                     }else {
                        AccessENTER = FALSE;    
                        switch(STATE){
                        
                case _NHIET_DO: //Function for Nhiet do here
                lcd_putc("\fNhiet do");
                break;
                
                case _DIEN_AP: //Function for Dien ap here
                lcd_putc("\fDien ap");
                break;
                
                case _MODULE_1: //Function for Module 1 here
                lcd_putc("\fModule 1");
                break;
                
                case _MODULE_2: //Function for Module 2 here
                lcd_putc("\fModule 2");
                break;
                
                case _MODULE_3: //Function for Module 3 here
                lcd_putc("\fModule 3");
                break;
                
                case _MODULE_4: //Function for Module 4 here
                lcd_putc("\fModule 4");
                break;
                 }
                        
                     }
                     break;
                     
                  case UP:                   
                     if(!AccessENTER) break;
                     if(PAGE == 0) PAGE = ContainMenu;
                     PAGE --;                
                     ProcessMenu();
                     break;
                  case DOWN:                 
                     if(!AccessENTER) break;
                     PAGE ++;                
                     ProcessMenu();
                     break;
                  case CANCEL:                 
                     if(!AccessCANCEL) break;
                     if(LevelPtr) LevelPtr--;   
                     PAGE = LEVEL[LevelPtr];    
                     if(!HasSubMenu()) {        
                           AccessENTER = TRUE;  
                           STATE = MenuPoiter[0].SubMenu;
                     } else {                   
                           STATE = QueryLastMenu();   
                        }
                        ContainMenu = SelectFrom(0);
                        ProcessMenu();          
                     break;
                  case MENU:
                     ContainMenu = SelectFrom(0);
                     ProcessMenu();
                     Break;  
               }
            }


            int8 keyscan() {
               int8 inB, varRet;
               inB = input_b()&0xF0;
               if(inB != 0xF0){ 
                  varRet = inB;
                  delay_ms(20);
                  inB = input_b()&0xF0;
                  if(inB == 0xF0) {
                     return varRet;
                  } else {
                     return 0;
                  }
               }
            }



            void Main() {

               int8 key;
               set_tris_b(0xFF); 
               lcd_init();       
               processAction(MENU); 
               while(1){
                  key = keyscan();  
                  if(key) {         
                     processAction(key);
                  }
               }
            }

// day la comment de test