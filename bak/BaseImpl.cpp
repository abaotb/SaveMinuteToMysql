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


/************************************************************************/
/*							���ݷ����߳�                                */
/************************************************************************/
DWORD WINAPI Classify_Data2(LPVOID lpParamter)
{

	OneMinute oneninute;
	BaseData *base;
	base= &oneninute;


	bool flag = false;        //ÿ��ֻ����һ������

 	while(1)
 	{

		if(method_Rest_Time())
		{
			Sleep(1000);
		    //������ʱ������    ����ڴ�������							/**********Ҫ�ĵĵط�**********/
			if (oneninute.tsmd_a->size()!=0)
			{
				oneninute.TIME_INTERVAL=0;
				oneninute.contract="";
				oneninute.compare_time=0;
				memset(&(oneninute.current_data),0,sizeof(oneninute.current_data));
				memset(&(oneninute.compare_data),0,sizeof(oneninute.compare_data));
				oneninute.tsmd_a = NULL;
				oneninute.tsmd_a= new map<string,OneMinute*>();
				base = NULL;
				base = &oneninute;
			}
		}

 		//�����ٽ���
 		EnterCriticalSection(&cs);
 		CThostFtdcDepthMarketDataField_Custom* dd =cmdSpi_custom->methodGetDatas_After();
 		//�ͷ��ٽ���
 		LeaveCriticalSection(&cs);
     	if (dd!=NULL)
     	{	
    		flag = true;   				
    	}
    	if (flag)
 		{
			
 			flag = false;  
 
 			//ת������
			CThostFtdcDepthMarketDataField_Custom_Tick ct = methodChangeData(*dd);
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
