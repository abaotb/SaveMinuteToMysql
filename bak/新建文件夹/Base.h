#ifndef BASE_H
#define BASE
#include "MdSpi.h"
#include "Custom_Methods.h"
using namespace std;
#pragma once
//����
class BaseData{
public:
	//���캯��
	BaseData(string contract,
		int compare_time,
		CThostFtdcDepthMarketDataField_Custom_Other current_data,
		CThostFtdcDepthMarketDataField_Custom_Other compare_data)
	{
		
		this->contract = contract;			 //��Լ���� 
		this->compare_time = compare_time;	 //��¼Tick���ݵ�ǰʱ�� 
		this->current_data = current_data;
		this->compare_data = compare_data;
	};
	BaseData()
	{

	};
	~BaseData(){};
public:
	//��Լ����
	string contract;
	//��¼Tick����,ͨ��5��\1����\1���Tick���ݴ�������������	              
	CThostFtdcDepthMarketDataField_Custom_Other current_data; 
	CThostFtdcDepthMarketDataField_Custom_Other compare_data; 

	//��¼Tick���ݵ�ǰʱ�� 
	int compare_time;
	//�жϺͺ�Լ�Ƿ����map
	virtual bool isExist(string str)const =0;//{return false;}
	//��Ӻ�Լ��map
	virtual void addContractToMap(CThostFtdcDepthMarketDataField_Custom_Tick*ct) const =0;//{}
	//��������
	virtual void updateData(CThostFtdcDepthMarketDataField_Custom_Tick*ct)const =0;//{}
	//���ݵ��߼�����
	virtual void methodLogicalOperation(BaseData *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick custom_tick)const =0;// {};
	//��ʼ����һ���׶�����
	virtual void methodInitNext(BaseData *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick custom_tick)const = 0;// {};
	//��һ��tick���ݴ����ҳ�1�����ڵĿ��ոߵͼ۵ȵ�
	virtual CThostFtdcDepthMarketDataField_Custom_Other methodTickToOther(CThostFtdcDepthMarketDataField_Custom_Tick original_data_tick)const =0;//{}
	
};

//һ���Ӽ̳���
class OneMinute:public BaseData{
	public:
	//���캯��
	//Triangle(double d,double h):Graph(d,h){};
	OneMinute(string contract,
		int compare_time,
		CThostFtdcDepthMarketDataField_Custom_Other current_data,
		CThostFtdcDepthMarketDataField_Custom_Other compare_data,
		map<string,OneMinute*> *tsmd_a):BaseData(contract,compare_time,current_data,compare_data),tsmd_a(tsmd_a)
	{
		TIME_INTERVAL=60;
	};

	OneMinute()
	{

	};
	~OneMinute(){};	

	public:
		int TIME_INTERVAL;
		map<string,OneMinute*> *tsmd_a;                        //��Լ��map
		//�жϺͺ�Լ�Ƿ����map
	    bool isExist(string str) const
		{
			if (this->tsmd_a != NULL)
			{
				//������
				if (this->tsmd_a->find(str)==this->tsmd_a->end())
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			return false;
		};
		//��Ӻ�Լ��map
	    void addContractToMap(CThostFtdcDepthMarketDataField_Custom_Tick*ct) const 
	    {
			map<string,OneMinute*> *tsmd_a= new map<string,OneMinute*>(); 
			CThostFtdcDepthMarketDataField_Custom_Other current_data;
			CThostFtdcDepthMarketDataField_Custom_Other compare_data;
			current_data.TradingDay=0; compare_data.TradingDay=0;

			//��������
			OneMinute* torigin= new OneMinute("",0,current_data,compare_data,tsmd_a);
			torigin->contract=(string)(ct->InstrumentID);
			(*(this->tsmd_a))[(string)(ct->InstrumentID)] = torigin;			
			torigin->compare_time = method_TimeToInt_60(ct->UpdateTime-ct->UpdateTime%100);//60
			//����1���ӱ�׼
			torigin->compare_data.Volume = 0;
			torigin->compare_data.Turnover = 0;
			torigin->compare_data.OpenInterest =0;//ct->OpenInterestRange;
			//����1����д��data_tickOneminute[7300]ȥ
			torigin->methodInitNext(torigin,*ct);
			
			
		}
		//��������
		void updateData(CThostFtdcDepthMarketDataField_Custom_Tick*ct) const
		{
			OneMinute * torigin= (*(this->tsmd_a))[(string)(ct->InstrumentID)];
			torigin->methodLogicalOperation(torigin,*ct);
		}
		//���ݵ��߼�����
 		void methodLogicalOperation(BaseData *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick custom_tick)const
 		{	
			//����ҹ��0��ʱʱ��ıȽ�
			if (custom_tick.UpdateTime == 0)                        
			{	
				tsmdarrays->compare_time = 0;
			}
			//cerr << custom_tick.InstrumentID<<"     method_Base"<<endl;

 			int time_60=method_TimeToInt_60(custom_tick.UpdateTime);
 			if(time_60 - tsmdarrays->compare_time < TIME_INTERVAL && time_60 - tsmdarrays->compare_time >=0)
 			{
 				
 				//���������߼�
 				if (custom_tick.LastPrice >tsmdarrays->current_data.HighestPrice )
 				{
 					//������߼�
 					tsmdarrays->current_data.HighestPrice = custom_tick.LastPrice;
 	
 				}else if(custom_tick.LastPrice <  tsmdarrays->current_data.LowestPrice )
 				{
 					//������ͼ�
 					 tsmdarrays->current_data.LowestPrice = custom_tick.LastPrice;		
 	
 				}
 				//�������̼�
 				tsmdarrays->current_data.ClosePrice = custom_tick.LastPrice;
 	
 				//ί��ί��
 				tsmdarrays->current_data.BidPrice1=custom_tick.BidPrice1;
 				tsmdarrays->current_data.BidVolume1=custom_tick.BidVolume1;
 				tsmdarrays->current_data.AskPrice1=custom_tick.AskPrice1;
 				tsmdarrays->current_data.AskVolume1=custom_tick.AskVolume1;
 				//���³ֲ���
 				 tsmdarrays->current_data.OpenInterest=custom_tick.OpenInterest;
 				//���³ɽ������ɽ���ֱֲ仯
 				tsmdarrays->current_data.Volume = 
 					custom_tick.Volume -  tsmdarrays->compare_data.Volume;
 	
 				tsmdarrays->current_data.Turnover =
 					custom_tick.Turnover -  tsmdarrays->compare_data.Turnover;
 	
 				tsmdarrays->current_data.OpenInterestRange = 
 					custom_tick.OpenInterest-  tsmdarrays->compare_data.OpenInterest;
 	
 			}
 			else if (time_60 - tsmdarrays->compare_time>=TIME_INTERVAL)
 			{
 				//����̨���
 				CThostFtdcDepthMarketDataField_Custom_Other fff = tsmdarrays->current_data;
 				//cerr << fff.InstrumentID<<" "<<fff.UpdateTime<<" ��߼ۣ�"<<fff.HighestPrice<<"��ͼۣ� "<<fff.LowestPrice<<" "<<endl;
 				//cerr <</* fff.InstrumentID<<*/"        "<<fff.UpdateTime<<" �ɽ�����"<<fff.Volume<<" �ɽ��"<<fff.Turnover<<" �ֲ�����"<<fff.OpenInterest<<" �ֲ"<<fff.OpenInterestRange<<endl;
 	
 				//дMysql���ݿ�
 				methodWriteFile2(fff,"today-1min");
 				Ado_Connect_Mysql(fff);
 				//����ʱ��
 				tsmdarrays->compare_time = time_60;//tsmdarrays->time_oneminute + 60;
 				//д������
 				tsmdarrays->methodInitNext(tsmdarrays,custom_tick);
 	
 			}
 	
 		};
 	
 		//��ʼ����һ���׶�����
 		 void methodInitNext(BaseData *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick custom_tick) const
		 {
			//cerr << custom_tick.InstrumentID<<"       methodUpdate_Base "<<endl;
 			//�����ոߵ͸���ͬһ�۸�	
 			CThostFtdcDepthMarketDataField_Custom_Other co = tsmdarrays->methodTickToOther(custom_tick);
  			tsmdarrays->current_data = co;      //����data_oneminute[650]����
 			tsmdarrays->compare_data= co;       //���±�׼
				
 		};
		 //���Զ����tick����ת����5�������tick���ݻ���1���ӵ����ݣ������ոߵͼ�Ϊͬһ�۸�
		CThostFtdcDepthMarketDataField_Custom_Other methodTickToOther
			(CThostFtdcDepthMarketDataField_Custom_Tick original_data_tick) const
		{

			CThostFtdcDepthMarketDataField_Custom_Other custom_tick_other;

			strncpy_s(custom_tick_other.ExchangeID,_countof(custom_tick_other.ExchangeID),
				original_data_tick.ExchangeID,strlen(original_data_tick.ExchangeID));

			strncpy_s(custom_tick_other.InstrumentID,_countof(custom_tick_other.InstrumentID),
				original_data_tick.InstrumentID,strlen(original_data_tick.InstrumentID));

			custom_tick_other.TradingDay=original_data_tick.TradingDay;
			//���̼ۣ����̼ۣ���߼ۣ���ͼ�
			custom_tick_other.OpenPrice=original_data_tick.LastPrice;
			custom_tick_other.ClosePrice=original_data_tick.LastPrice;
			custom_tick_other.HighestPrice=original_data_tick.LastPrice;
			custom_tick_other.LowestPrice=original_data_tick.LastPrice;

			custom_tick_other.Volume=original_data_tick.Volume;
			custom_tick_other.Turnover=original_data_tick.Turnover;
			custom_tick_other.OpenInterest=original_data_tick.OpenInterest;
			custom_tick_other.UpdateTime=original_data_tick.UpdateTime;
			custom_tick_other.BidPrice1=original_data_tick.BidPrice1;
			custom_tick_other.BidVolume1=original_data_tick.BidVolume1;
			custom_tick_other.AskPrice1=original_data_tick.AskPrice1;
			custom_tick_other.AskVolume1=original_data_tick.AskVolume1;
			custom_tick_other.OpenInterestRange = original_data_tick.OpenInterestRange;

			return custom_tick_other;
		};

		
};


#endif BASE_H