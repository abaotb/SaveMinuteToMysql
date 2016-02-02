#ifndef BASE_H
#define BASE
#include "MdSpi.h"
#include "Custom_Methods.h"
using namespace std;

extern int	  DATA_TIME_INTERVAL;

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

		this->contract = contract;					//��Լ���� 
		this->compare_time = compare_time;			//��¼Tick���ݵ�ǰʱ�� 
		this->current_data = current_data;			//��ǰ����
		this->compare_data = compare_data;			//���Ƚ�����	
	};
	BaseData()
	{
		CThostFtdcDepthMarketDataField_Custom_Other current_data;
		CThostFtdcDepthMarketDataField_Custom_Other compare_data;
		current_data.TradingDay = 0;
		compare_data.TradingDay = 0;
		this->contract = "";						//��Լ���� 
		this->compare_time = 0;						//��¼Tick���ݵ�ǰʱ�� 
		this->current_data = current_data;			//��ǰ����
		this->compare_data = compare_data;			//���Ƚ�����
	};
	~BaseData(){};
public:
	//��Լ����
	string contract;
	//��ǰ���ݣ����Ƚ�����	              
	CThostFtdcDepthMarketDataField_Custom_Other current_data; 
	CThostFtdcDepthMarketDataField_Custom_Other compare_data; 

	//��¼Tick���ݵ�ǰʱ�� 
	int compare_time;
	//�жϺͺ�Լ�Ƿ����map
	virtual bool isExist(string str)const =0;
	//��Ӻ�Լ��map
	virtual void addContractToMap(CThostFtdcDepthMarketDataField_Custom_Tick*ct) const =0;
	//��������
	virtual void updateData(CThostFtdcDepthMarketDataField_Custom_Tick*ct)const =0;
	//���ݵ��߼�����
	virtual void methodLogicalOperation(BaseData *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick &custom_tick)const =0;
	//��ʼ����һ���׶�����
	virtual void methodInitNext(BaseData *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick &custom_tick)const = 0;
	//��һ��tick���ݴ����ҳ�1�����ڵĿ��ոߵͼ۵ȵ�
	virtual CThostFtdcDepthMarketDataField_Custom_Other methodTickToOther(CThostFtdcDepthMarketDataField_Custom_Tick &original_data_tick)const =0;

};

//һ���Ӽ̳���
class  BaseDataInherit:public BaseData{
public:
	//���캯��
	//Triangle(double d,double h):Graph(d,h){};
	BaseDataInherit(string contract,
		int compare_time,
		CThostFtdcDepthMarketDataField_Custom_Other current_data,
		CThostFtdcDepthMarketDataField_Custom_Other compare_data
		):BaseData(contract,compare_time,current_data,compare_data)
	{

		map<string, BaseDataInherit*> *tsmd_a= new map<string, BaseDataInherit*>(); 
		this->tsmd_a = tsmd_a;					    //map
		this->TIME_INTERVAL	= DATA_TIME_INTERVAL;	//ʱ����				/**********Ҫ�ĵĵط�**********/

		this->contract = "";						//��Լ���� 
		this->compare_time = 0;						//��¼Tick���ݵ�ǰʱ�� 
		this->current_data = current_data;			//��ǰ����
		this->compare_data = compare_data;			//���Ƚ�����


	};

	BaseDataInherit()
	{

		map<string, BaseDataInherit*> *tsmd_a= new map<string, BaseDataInherit*>(); 
		CThostFtdcDepthMarketDataField_Custom_Other current_data;
		CThostFtdcDepthMarketDataField_Custom_Other compare_data;
		current_data.TradingDay = 0;
		compare_data.TradingDay = 0;
		this->tsmd_a = tsmd_a;						//map
		this->TIME_INTERVAL = DATA_TIME_INTERVAL;	//ʱ����			/**********Ҫ�ĵĵط�**********/

		this->contract = "";						//��Լ���� 
		this->compare_time = 0;						//��¼Tick���ݵ�ǰʱ�� 
		this->current_data = current_data;
		this->compare_data = compare_data;

	};
	~ BaseDataInherit(){};	

public:
	int TIME_INTERVAL;								//ʱ����
	map<string, BaseDataInherit*> *tsmd_a;          //��Լ��map
	//�жϺͺ�Լ�Ƿ����map
	bool isExist(string str) const;
	//��Ӻ�Լ��map
	void addContractToMap(CThostFtdcDepthMarketDataField_Custom_Tick*ct) const;
	//��������
	void updateData(CThostFtdcDepthMarketDataField_Custom_Tick*ct) const;
	//���ݵ��߼�����
	void methodLogicalOperation(BaseData *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick &custom_tick)const;

	//��ʼ����һ���׶�����
	void methodInitNext(BaseData *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick &custom_tick) const;
	//���Զ����tick����ת����5�������tick���ݻ���1���ӵ����ݣ������ոߵͼ�Ϊͬһ�۸�
	CThostFtdcDepthMarketDataField_Custom_Other  methodTickToOther(CThostFtdcDepthMarketDataField_Custom_Tick &original_data_tick) const;
};
	

#endif BASE_H