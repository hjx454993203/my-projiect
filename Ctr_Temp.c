/**********************************************
******************Ctr_Temp.c*************************
************************************************/
#define _TEMP_SRC_
#include "com.h"


//====================
unsigned long __ROM Temptable[]=  { 931,927,923,919,915,911,907,902,898,893,	//40-49c		pull_up 5.1k	
									889,884,879,874,869,863,858,852,847,841,	//50-59c
									835,829,823,816,810,804,797,790,784,777,	//60-69c
									770,763,755,748,741,734,726,718,711,703,	//70-79c
									696,688,680,672,664,656,648,640,632,624,	//80-89c
									616,607,599,591,583,575,566,558,550,542,	//90-99c
									534,526,518,510,502,494,486,478,471,463,	//100-109c
									455,448,440,433,425,418,411,404,397,390,	//110-119c
									383,376,369,362,356,349,343,336,330,324,	//120-129c
									318,312,306,300,295,289,284,278,273,268,	//130-139c
									262,257,252,248,243,238,233,229,224,220,	//140-149c
									216,211,207,203,199,195,191,188,184,180,	//150-159c
									177,173,170,167,163,160,157,154,151,148,	//160-169c
									145,142,139,137,134,131,129,126,124,121,	//170-179c
									119,117,115,112,110,108,106,104,102,100,	//180-189c
									98 ,96 ,94 ,93 ,91 ,89,88,87,86 									//190-195c				
								};
//======================
//=====================================
uchar looktable(ulong lookvalue)
{
	uchar i, tempvalue=39;	
	for(i=0;((i<159)&&(Temptable[i]>lookvalue));i++)
	{		
		tempvalue++;				
	}
	return tempvalue;	
}

//==============================
void Close_Coffee_Heater(void)
{
	if(CofHeaterStop_F == 0)
	{
		CofHeaterStop_F=1;//colse coffee heat
		
		cnt_stm_time = 0;
		Close_Start_F = 1;
	}
}
void Open_Coffee_Heater(void)
{
	if(CofHeaterOn_F == 0)
	{
		CofHeaterOn_F = 1;
		cnt_stm_time = 0;
	}
}
void Close_Steam_Heater(void)
{
	if(SteHeaterStop_F == 0)
	{
		SteHeaterStop_F = 1;
		
		cnt_stm_time = 0;
		Close_Start_F = 1;
	}
}
void Open_Steam_Heater(void)
{
	if(SteHeaterOn_F == 0)
	{
		SteHeaterOn_F = 1;
		cnt_stm_time = 0;
	}
}

void PwmOut_Pro(void)
{
	if(bon_coffee_heater)
	{
		if(coffee_pid_end)
		{
			if((Sys_State==Coffee)||(Sys_State==Hot_W))	//--Ö´ÐÐÖÜÆÚ(7+1)*0.5s
			{
				if(CntCofmS>=4)//4//*10ms*100=4s
				{					
					CntCofmS=0;
					
					if(CofPWMOUT==0)
					{
						if(Close_Start_F == 0)
						{
							if(Cof_Heater_Close_F == 0)
							{
								Cof_Heater_Close_F = 1;
								Close_Coffee_Heater();
							}
						}
					}
					else
					{
						if(CofPWMOUT > 0)
						{
							//CofPWMOUT--;
							if(Close_Start_F == 0)
							{
								CofPWMOUT--;
								Open_Coffee_Heater();
							}
							
						}
					}		
				}		
			}
			else
			{
				if(CntCofmS>=16)//16*10ms*100=16s
				{
					CntCofmS=0;
					if(CofPWMOUT==0)
					{
						if(Close_Start_F == 0)
						{
							if(Cof_Heater_Close_F == 0)
							{
								Cof_Heater_Close_F = 1;
								if(CofHeaterOn_F)
								Close_Coffee_Heater();
							}
						}
						
					}
					else
					{
						if(CofPWMOUT > 0)
						{
							//CofPWMOUT--;
							if(Close_Start_F == 0)
							{
								CofPWMOUT--;
								Open_Coffee_Heater();
							}
							
						}
					}
				}
			}
		}
	}
	else
	{
		if(stm_pid_end)
		{
			if(bStmRun)
			{	
				if(CntStmmS>=4)//40ms*100=4s
				{
					CntStmmS=0;
					if(StmPWMOUT==0)
					{
						if(Close_Start_F == 0)
						{
							Close_Steam_Heater();
						}
					}
					else
					{
						if(StmPWMOUT > 0)
						{
							//StmPWMOUT--;
							if(Close_Start_F == 0)
							{
								StmPWMOUT--;
								Open_Steam_Heater();
							}
						}
					}					
				}
			}
			else
			{
				if(CntStmmS>=16)//160ms*100=16s
				{
					CntStmmS=0;
					if(StmPWMOUT==0)
					{
						if(Close_Start_F == 0)
						{
							if(Stm_Heater_Close_F == 0)
							{
								Stm_Heater_Close_F = 1;
								if(SteHeaterOn_F)
								Close_Steam_Heater();
							}
						}
					}
					else
					{
						if(StmPWMOUT > 0)
						{
							//StmPWMOUT--;
							if(Close_Start_F == 0)
							{
								StmPWMOUT--;
								Open_Steam_Heater();
							}
						}
					}					
				}	
			}
		}	
	}		
}	
void Allocate_time(void)
{
	if(Pout_Cof < 50)//coffee heater first;
	{
		Sum_Cof=ppid.sum;
		Lasterrk_Cof=Cof_Errk_1;
		if(coffee_pid_end == 0)
		{
			coffee_pid_end = 1;
			CofPWMOUT=Pout_Cof;
			bon_coffee_heater = 1;
		}
	}
	else if(Pout_Stm < 50)//steam heater first;
	{
		Sum_Stm = ppid.sum;
		Lasterrk_Stm = Stm_Errk_1;	
		if(stm_pid_end == 0)
		{
			stm_pid_end = 1;
			StmPWMOUT = Pout_Stm;
			bon_coffee_heater = 0;
		}
	}
	else//coffee heater first; 
	{
		Sum_Cof=ppid.sum;
		Lasterrk_Cof=Cof_Errk_1;
		if(coffee_pid_end == 0)
		{
			coffee_pid_end = 1;
			CofPWMOUT=Pout_Cof;
			bon_coffee_heater = 1;
		}
	}
}
void Judge_Cof_Temp(void)
{
	if(COF_Temp>=(CofSetTemp-5))
	{
		CntCofOK++;
		if(CntCofOK>7)//8*0.5s=4s
		{
			CntCofOK=0;
			CofTEMPOK_F=1;									
		}		
	}
	else
	{	
		CntCofOK=0;
	}
}
void Judge_Stm_Temp(void)
{
	if((Stm_Temp>=(StmSetTemp-15)))
	{
		CntStmOK++;
		if(CntStmOK>7)	//8*0.5s=4s
		{
			CntStmOK=0;
			StmTEMPOK_F=1;									
		}								
	}
	else
	{
		CntStmOK=0;	
	}
}
void Coffee_Ad(void)
{
	ADM = 0x91; // AN1	 NTC1	COF
	ADPRO();
	NTCCHK1(ADdata);
	COF_Temp=looktable(ADdata);	//ntc temp 
}
void Steam_Ad(void)
{
	ADM = 0x92; // AN2	NTC2	stm
	ADPRO();
	NTCCHK2(ADdata);
	Stm_Temp=looktable(ADdata);	//ntc temp 
}
void Stm_pid(void)
{
	if(stm_pid_end == 0)
	{
		stm_pid_end = 1;
		
		Steam_Ad();
		
		ppid.kp=9;
		ppid.ki=6;							
		ppid.kd=4;
		
		if((Sys_State==PreHeat)||(Sys_State==Ready)||(Sys_State==CofClean))
		{
			Pout_Stm=Pidpro(Stm_Temp,StmSetTemp,Sum_Stm,Lasterrk_Stm);
			
		}
		
		if( Pout_Stm > 75) Pout_Stm = 100;
		else if( Pout_Stm < 25) Pout_Stm = 0;
		
		Sum_Stm = ppid.sum;
		Lasterrk_Stm = ppid.errk_1;	

		StmPWMOUT = Pout_Stm;
		bon_coffee_heater = 0;
	}
}
void Cof_pid(void)
{
	if(coffee_pid_end == 0)
	{
		coffee_pid_end = 1;
		
		Coffee_Ad();

		if(Sys_State==PreHeat)	ppid.kp=8;
		else	ppid.kp=5;
		ppid.ki=8;							
		ppid.kd=4;

		if((Sys_State==PreHeat)||(Sys_State==Ready)||(Sys_State==CofClean))
		{
			Pout_Cof=Pidpro(COF_Temp,CofSetTemp,Sum_Cof,Lasterrk_Cof);
		}
		
		if( Pout_Cof > 90) Pout_Cof = 100;
		else if( Pout_Cof < 10) Pout_Cof = 0;
		
		Sum_Cof=ppid.sum;
		Lasterrk_Cof=ppid.errk_1;

		CofPWMOUT=Pout_Cof;
		bon_coffee_heater = 1;
	}
}

void Calculate_Pid(void)
{
	if(Close_Start_F == 0)
	{
		if(Cof_Heater_Close_F == 1)
		{
			Stm_pid();
		}
		else if(Stm_Heater_Close_F == 1)
		{
			Cof_pid();
		}
	}
	
}


void Reset_Heater(void)
{
	Reset_F = 1;
	
	CofHeaterOn_F = 0;
	SteHeaterOn_F = 0;
	coffee_pid_end = 0;
	stm_pid_end = 0;
	Cof_Heater_Close_F = 0;
	Stm_Heater_Close_F = 0;
	CofHeaterStop_F = 0;
	SteHeaterStop_F = 0;
	
	Cnt_16s = 0;
	Cnt_4s = 0;

	cnt_stm_time1 = 0;

	StmPWMOUT = 0;
	CofPWMOUT = 0;
}

void Ctr_Temp(void)
{
	if(TIME500_F)
	{
		TIME500_F=0;
		
		Coffee_Ad();
		Steam_Ad();

		//StmTmpSamp[0]=StmTmpSamp[1];
        //StmTmpSamp[1]=StmTmpSamp[2];
        //StmTmpSamp[2]=StmTmpSamp[3];
        //StmTmpSamp[3]=Stm_Temp;

		Judge_Cof_Temp();
		Judge_Stm_Temp();

		if((PowerOn_F)&&(Reset_F == 0))
		{
			if(bStmRun)
			{
				if((Cnt_4s == 0)&&(coffee_pid_end == 0)&&(stm_pid_end == 0))
				{
					#if 1
					if(Stm_Temp< (StmSetTemp+4))
					{
						if(stm_pid_end == 0)
						{
							stm_pid_end = 1;
							StmPWMOUT=100;
							bon_coffee_heater = 0;
						}
					}
					else
					{
						if(stm_pid_end == 0)
						{
							stm_pid_end = 1;
							StmPWMOUT=0;
							bon_coffee_heater = 0;
						}
					}
					#endif
					
					Cnt_4s = 8;
				}
				else
				{
					if(Cnt_4s > 0)	Cnt_4s--;
					if(Cnt_4s == 0)
					{
						if(Close_Start_F == 0)
						{
							Close_Steam_Heater();
							coffee_pid_end = 1;
							Cnt_16s = 0;
						}
					}
				}
			}
			else if((Sys_State==Coffee)||(Sys_State==Hot_W))
			{
				if((Cnt_4s == 0)&&(coffee_pid_end == 0)&&(stm_pid_end == 0))
				{
					#if 1
					if(COF_Temp< (CofSetTemp+4))
					{
						if(coffee_pid_end == 0)
						{
							coffee_pid_end = 1;
							CofPWMOUT=100;
							bon_coffee_heater = 1;
						}
					}
					else
					{
						if(coffee_pid_end == 0)
						{
							coffee_pid_end = 1;
							CofPWMOUT=0;
							bon_coffee_heater = 1;
						}
					}
					#endif
					Cnt_4s = 8;
				}
				else
				{
					if(Cnt_4s > 0)	Cnt_4s--;
					if(Cnt_4s == 0)
					{
						if(Close_Start_F == 0)
						{
							Close_Coffee_Heater();
							stm_pid_end = 1;//no Calculate pid
							Cnt_16s = 0;
						}
					}
				}
			}
			else if((Sys_State==PreHeat)||(Sys_State==Ready)||(Sys_State==CofClean))
			{
				if((Cnt_16s == 0)&&(coffee_pid_end == 0)&&(stm_pid_end == 0))
				{
					if(((Sys_State==PreHeat)||(Sys_State==Ready)||(Sys_State==CofClean))&&(stm_pid_end == 0)&&(coffee_pid_end == 0))
					{
						//--------------------
						if(Sys_State==PreHeat)	ppid.kp=8;
						else	ppid.kp=5;
						ppid.ki=8;							
						ppid.kd=4;

						if(Sys_State==CofClean)
						{
							Pout_Cof=Pidpro(COF_Temp,90,Sum_Cof,Lasterrk_Cof);
							if( Pout_Cof > 90) Pout_Cof = 100;
							else if( Pout_Cof < 10) Pout_Cof = 0;
							Cof_Errk_1 = ppid.errk_1;
						}
						else
						{
							Pout_Cof=Pidpro(COF_Temp,CofSetTemp,Sum_Cof,Lasterrk_Cof);
							if( Pout_Cof > 90) Pout_Cof = 100;
							else if( Pout_Cof < 10) Pout_Cof = 0;
							Cof_Errk_1 = ppid.errk_1;
						}
						
						//--------------------------
						ppid.kp=9;
						ppid.ki=6;							
						ppid.kd=4;
						
						Pout_Stm=Pidpro(Stm_Temp,StmSetTemp,Sum_Stm,Lasterrk_Stm);
						if( Pout_Stm > 75) Pout_Stm = 100;
						else if( Pout_Stm < 25) Pout_Stm = 0;
						Stm_Errk_1 = ppid.errk_1;
						
						//--------------------------
						Allocate_time();

						Cnt_16s = 32;
					}
				}
				else
				{
					if(Cnt_16s > 0)		Cnt_16s--;
					if((Cnt_16s == 16)||(Cnt_16s == 0))
					{
						if(bon_coffee_heater)
						{
							if(Close_Start_F == 0)
							{
								if(Cof_Heater_Close_F == 0)
								{
									Cof_Heater_Close_F = 1;
									Close_Coffee_Heater();
								}
							}
						}
						else
						{
							if(Close_Start_F == 0)
							{
								if(Stm_Heater_Close_F == 0)
								{
									Stm_Heater_Close_F = 1;
									Close_Steam_Heater();
								}
							}
							
						}
					}
				}
			}
		}
	}
}

