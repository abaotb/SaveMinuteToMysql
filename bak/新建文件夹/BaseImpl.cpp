					/************************************************************************/
					/*  ���ݴ�������ļ�����Ҫ��MdSpi.cpp�ļ������ݴ����5�룬1���ӵ�����   */
					/************************************************************************/
#include "Base.h"
using namespace std;


// USER_API����
extern CThostFtdcMdApi* pUserApi;
//���߳�

extern CRITICAL_SECTION cs;

//CThostFtdcDepthMarketDataField_Custom data_static2;                                     //ȡ��������,�ĵ�ȫ�ֱ���
extern CMdSpi_Custom * cmdSpi_custom;	


/************************************************************************/
/*							���ݷ����߳�                                */
/************************************************************************/
DWORD WINAPI Classify_Data2(LPVOID lpParamter)
{
	map<string,OneMinute*> *tsmd_a= new map<string,OneMinute*>(); 
	CThostFtdcDepthMarketDataField_Custom_Other current_data;
	CThostFtdcDepthMarketDataField_Custom_Other compare_data;
	current_data.TradingDay=0; compare_data.TradingDay=0;
	
	OneMinute oneninute("",0,current_data,compare_data,tsmd_a);
	BaseData *base;
	base= &oneninute;


	bool flag = false;        //ÿ��ֻ����һ������

 	while(1)
 	{

		if(method_Rest_Time())
		{
			Sleep(1000);
		    //������ʱ������
			if (oneninute.tsmd_a->size()!=0)
			{
				oneninute.contract="";
				oneninute.compare_time=0;
				memset(&(oneninute.current_data),0,sizeof(oneninute.current_data));
				memset(&(oneninute.compare_data),0,sizeof(oneninute.compare_data));
				oneninute.tsmd_a=NULL;
				oneninute.tsmd_a= new map<string,OneMinute*>();
				base= NULL;
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
    		//data_static2 =*dd;		
    		flag = true;   				
    	}
    	if (flag)
 		{
			//ҹ����0
			//methodYepan2(data_static2.UpdateTime,&oneninute);

 			flag = false;  
 
 			//ת������
			CThostFtdcDepthMarketDataField_Custom_Tick ct = methodChangeData(*dd/*data_static2*/);
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
 
}//end FUN











//ҹ������
//void methodYepan2(int yepan_time,OneMinute*oneninute) 
//{
//	if (yepan_time == 0)                        //ҹ��0��
//	{
//		map<string,OneMinute*>::iterator iter;
//		for (iter = oneninute->tsmd_a->begin();iter != oneninute->tsmd_a->end();iter++)
//		{
//			   //ҹ��0��
//			iter->second->compare_time = 0;
//		
//		}
//	}
//};


/*dd = new CThostFtdcDepthMarketDataField_Custom();
			dd->TradingDay=11;
			strcpy(dd->InstrumentID,"IF1511");
				data_static2 =*dd;		*/
