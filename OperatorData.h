#ifndef OPERATORDATA_H
#define OPERATORDATA_H
#include "MdSpi.h"
using namespace std;
#pragma once
//�ĸ������    //�������
class TSMD_Arrays{
public:
	//��Լ����
	string contract;
	//1)��¼Tick����,ͨ��5��\1����\1���Tick���ݴ�������������
	//CThostFtdcDepthMarketDataField_Custom_Tick data_tick[TICKCOUNT];												
	CThostFtdcDepthMarketDataField_Custom_Other data_fiveseconds[FS_TICKCOUNT];              
	CThostFtdcDepthMarketDataField_Custom_Other data_oneminute[OM_TICKCOUNT];                
	CThostFtdcDepthMarketDataField_Custom_Other data_oneday;  
	//2)��¼Tick���ݺ�Լ������ĸ���
	int index_tick;															       
	int index_fiveseconds;
	int index_oneminute; 
	int index_oneday; 
	//3)��¼Tick���ݵ�ǰʱ�� 
	int time_tick;                                                                           
	int time_fivesecond;
	int time_oneminute;
	int time_oneday;
	//����ת�����Զ�������
	/*CThostFtdcDepthMarketDataField_Custom_Tick methodChangeToCustom(CThostFtdcDepthMarketDataField original_data);*/
	CThostFtdcDepthMarketDataField_Custom_Other methodTickToOther(CThostFtdcDepthMarketDataField_Custom_Tick original_data_tick);
	
	//4)�ҳ�x���ڵĿ��ոߵͼ۵ȵ�
	//��һ��tick���ݴ����ҳ�5���ڵĿ��ոߵͼ۵ȵ�
	//void method_Fiveseconds(TSMD_Arrays *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick custom_tick);
	//��һ��tick���ݴ����ҳ�1�����ڵĿ��ոߵͼ۵ȵ�
	void method_Oneminute(TSMD_Arrays *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick custom_tick);
	
	
	//5)��¼��������ȥ
	//д��data_tick[73000]������ȥ
	//void methodUpdate_tick(TSMD_Arrays *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick custom_tick);             
	//д��data_Fiveseconds[7300]������ȥ
	//void methodUpdate_Fiveseconds(TSMD_Arrays *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick custom_tick);
	//д��data_Oneminute[650]������ȥ
	void methodUpdate_Oneminute(TSMD_Arrays *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick custom_tick);
	//д��data_onedayȥ
	//void method_Oneday(TSMD_Arrays *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick custom_tick);

	TSMD_Arrays()
	{
		//��Լ����
		contract="";
		//��¼Tick����,ͨ��5��\1����\1���Tick���ݴ�������������

		//��¼Tick���ݺ�Լ������ĸ���
		index_tick=0;															       
		index_fiveseconds=0;
		index_oneminute=0; 
		index_oneday=0;		 
		//��¼Tick���ݵ�ǰʱ�� 
		time_tick=0;                                                                     
		time_fivesecond=0;
		time_oneminute=0;
		time_oneday=0;

	};
	~TSMD_Arrays(){};
};

//Tick,5Seconds,One_minute,One_Day
class TSMD_Map{
	//stringΪ��Լ���룬TSMD_ArraysΪ4��������
public:map<string,TSMD_Arrays*>*tsmd_a; 
	   TSMD_Map()
	   {
		  tsmd_a = new map<string,TSMD_Arrays*>();
	   };
	   ~TSMD_Map(){};

};

//////////////////////////////////////////////////////////////////////////
//�������к�Լ������
class All_Contracts_Array{
public:
	//��������
	int index_all;		
	//���ݻ�����
	CThostFtdcDepthMarketDataField_Custom All_Contracts_Data[MAXNUMBER];       //����tickdata
	//CThostFtdcDepthMarketDataField_Custom_Other  All_Contracts_Data[MAXNUMBER];  //����1���ӣ�5���ӣ�x����
	//���µ�ʱ��
	int end_write_time;
	//���µ�λ��
	int end_write_index;
};
//////////////////////////////////////////////////////////////////////////

#endif OPERATORDATA_H