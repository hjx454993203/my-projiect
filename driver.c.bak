/**********************************************
******************Driver.c*************************
************************************************/
#define  _DRIVER_SRC_
#include "com.h"

//modify;

void TurnOn_StmCof_Heater(void)
{
	switch(cnt_stm_time)
	{
		case 0:
			if(bon_coffee_heater)	CH_DRV = 0;//select coffee;
			else					CH_DRV = 1;//select stm;
			break;
		case 5:
			bon_SCR = 1;//开可控硅;
			break;
		case 10:
			SH_DRV = 1;//开继电器;
			break;
		case 15:
			bon_SCR = 0;//关可控硅;
			break;
		case 20:
			FP11 = 1;
			break;
		default:
			break;
	}
	if(cnt_stm_time < 21)		cnt_stm_time++;
}

void TurnOff_StmCof_Heater(void)
{
	switch(cnt_stm_time)
	{
		case 0:
			bon_SCR = 1;//开可控硅;
			break;
		case 5:
			SH_DRV = 0;//关继电器;
			break;
		case 10:
			bon_SCR = 0;//关可控硅;
			break;
		case 15:	
			//Close_Start_F = 0;
			CH_DRV = 0;//select coffee;
			//FP11 = 0;
			break;
		case 20:	
			Close_Start_F = 0;
			//CH_DRV = 0;//select coffee;
			FP11 = 0;
			break;
		default:
			break;
	}
	if(cnt_stm_time < 21)		cnt_stm_time++;
	
}
void Force_TurnOff_Heater(void)
{
	switch(cnt_stm_time1)
	{
		case 0:
			bon_SCR = 1;//开可控硅;
			break;
		case 5:
			SH_DRV = 0;//关继电器;
			break;
		case 10:
			bon_SCR = 0;//关可控硅;
			break;
		case 15:
			CH_DRV = 0;//select coffee;
			break;
		case 20:	
			Reset_F = 0;
			break;
		default:
			break;
	}
	if(cnt_stm_time1 < 21)		cnt_stm_time1++;
}

void Ctr_Heater(void)
{
	if((Reset_F)||(PowerOn_F == 0))
	{
		Force_TurnOff_Heater();
	}
	else if(PowerOn_F)
	{
		if(Close_Start_F)		TurnOff_StmCof_Heater();
		else if((CofHeaterOn_F)||(SteHeaterOn_F))	TurnOn_StmCof_Heater();
		//Can Shu Chu Shi Hua;
		#if 1
		if((Close_Start_F==0)&&(coffee_pid_end)&&(stm_pid_end)&&(Cnt_16s==0))//both as 1,both heater heated;
		{
			coffee_pid_end = 0;
			stm_pid_end = 0;
			
			Cof_Heater_Close_F = 0;
			Stm_Heater_Close_F = 0;

			CofHeaterStop_F = 0;
			SteHeaterStop_F = 0;

			CofHeaterOn_F = 0;
			SteHeaterOn_F = 0;

			Cnt_4s = 0;

			StmPWMOUT = 0;
			CofPWMOUT = 0;
		}
		#endif
	}
}
