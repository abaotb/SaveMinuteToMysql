					/************************************************************************/
					/*  ���ݴ�������ļ�����Ҫ��MdSpi.cpp�ļ������ݴ����5�룬1���ӵ�����   */
					/************************************************************************/
#include "Base.h"
using namespace std;


// USER_API����
extern CThostFtdcMdApi* pUserApi;
//���߳�
extern CRITICAL_SECTION cs;
extern CMdSpi_Custom * cmdSpi_custom;	
//����֣����ҹ����������
int TradingDay;


/************************************************************************/
/*							���ݷ����߳�                                */
/************************************************************************/
DWORD WINAPI Classify_Data2(LPVOID lpParamter)
{

	BaseDataInherit basedatainherit;
	basedatainherit.TIME_INTERVAL =DATA_TIME_INTERVAL;   //Ĭ��ʱ����


	BaseData *base;										 //����ָ��
	base= &basedatainherit;

	bool flag = false;        //ÿ��ֻ����һ������

 	while(1)
 	{
		//����ʱ���������
		if(method_Rest_Time())
		{
			Sleep(1000);
			//������ʱ������    ����ڴ�������							//Ҫ�ĵĵط�
			if (basedatainherit.tsmd_a->size()!=0)
			{
				basedatainherit.TIME_INTERVAL = DATA_TIME_INTERVAL;
				basedatainherit.contract="";
				basedatainherit.compare_time=0;
				memset(&(basedatainherit.current_data),0,sizeof(basedatainherit.current_data));
				memset(&(basedatainherit.compare_data),0,sizeof(basedatainherit.compare_data));
				basedatainherit.tsmd_a = NULL;
				basedatainherit.tsmd_a= new map<string,BaseDataInherit*>();
				base = NULL;
				base = &basedatainherit;
			}
		}
		
 		//�����ٽ���
 		EnterCriticalSection(&cs);
 		CThostFtdcDepthMarketDataField_Custom dd =cmdSpi_custom->methodGetDatas_After();
 		//�ͷ��ٽ���
 		LeaveCriticalSection(&cs);	
     	if (dd.TradingDay!=0)
     	{	
    		flag = true;   				
    	}else{
			//˯��10ms,��ֹcpu����      20151222 ��
			Sleep(10);
		}
    	if (flag)
 		{
			//////////////////////////////////////////////////////////////////////////
			//����֣����ҹ����������
			if (dd.UpdateTime>=210000&&dd.UpdateTime<210300)
			{			
				if ((string)(dd.ExchangeID)=="shfe")
				{
					TradingDay = dd.TradingDay;
				}
			}
			if (dd.UpdateTime>=210000||dd.UpdateTime<23000)
			{
				if ((string)(dd.ExchangeID)=="czce")
				{
					dd.TradingDay=TradingDay;
				}
			}	
			//////////////////////////////////////////////////////////////////////////
 			flag = false;  

 			//ת������
			CThostFtdcDepthMarketDataField_Custom_Tick ct = methodChangeData(dd);
			if (!base->isExist((string)(ct.InstrumentID)))
 			{
				base->addContractToMap(&ct);
 			}
 			else
			{
 				base->updateData(&ct);			
 			}
			
 			
 		}//end flag				
 
 	}//end while
 
}//end Classify_Data2






// 	CThostFtdcDepthMarketDataField_Custom_Other current_data;
// 	current_data.TradingDay =0;
// 	Ado_Connect_Mysql(current_data);
// 	while(1)
// 	{
// 
// 	}




//BaseDataInherit����
//vector<BaseDataInherit>vBasedata;
//BaseDataInherit basedatainherit2;
//basedatainherit.TIME_INTERVAL =5;   
//
////���BaseDataInherit����
//vBasedata.push_back(basedatainherit);
//vBasedata.push_back(basedatainherit2);



// 	BaseDataInherit bdi[2];
// 	bdi[0] = basedatainherit;
// 	bdi[1] = basedatainherit;

//BaseDataInherit basedatainherit2;
//basedatainherit2.TIME_INTERVAL = 5;					 //5��ʱ����

//methodClearDatas(bdi,2);
/*
void methodClearDatas(BaseDataInherit bdi,int len)
{

	
	if(method_Rest_Time())
	{
		Sleep(1000);
		for (int i=0;i<len;i++)
		{	
			BaseDataInherit	basedatainherit = bdi[i];
			//������ʱ������    ����ڴ�������							
			if (basedatainherit.tsmd_a->size()!=0)
			{
				basedatainherit.TIME_INTERVAL = DATA_TIME_INTERVAL;
				basedatainherit.contract="";
				basedatainherit.compare_time=0;
				memset(&(basedatainherit.current_data),0,sizeof(basedatainherit.current_data));
				memset(&(basedatainherit.compare_data),0,sizeof(basedatainherit.compare_data));
				basedatainherit.tsmd_a = NULL;
				basedatainherit.tsmd_a= new map<string,BaseDataInherit*>();
				base = NULL;
				base = &basedatainherit;
			}
		}
	  
	}

}*/
/*
//���⳵ת��
base= &basedatainherit2;
if (!base->isExist((string)(ct.InstrumentID)))
{
	base->addContractToMap(&ct);
}
else
{
	base->updateData(&ct);			
}
*/