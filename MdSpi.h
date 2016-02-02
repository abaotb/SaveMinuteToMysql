#ifndef MDSPI_H
#define MDSPI_H
#include <map>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <ctime>
#include<cmath>
#include<share.h>
#include <windows.h>
#include <cstring>
#include <string.h>
#include ".\ThostTraderApi\ThostFtdcMdApi.h"

#pragma region ����Mysql
//#include <WinSock2.h>  
#include "winsock.h" 
#include "mysql.h"

#pragma comment(lib,"libmysql.lib")  
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF")
#pragma endregion  ����Mysql


using namespace std;


#pragma once
//#define local_host "192.168.211.126"  
//#define user_name "root"  
//#define user_pwd "root"  
//#define data_base "exchangedata"  
//#define table "oneminute_data"
//#define data_port 3306 
//#define database_driver "DSN=mysql"

//------------------------
//���ݿ�������Ϣ  
//#define local_host "192.168.200.207"  
//#define user_name "root"  
//#define user_pwd "mysql"  
//#define data_base "exchangedata"  
//#define table "oneminute_data"
//#define data_port 3306 
//#define database_driver "DSN=mysql"
//------------------------

//------------------------
//��������С
#define MAXNUMBER 300000
//��Լ�ĸ���
#define CONTRACTCOUNT 500
//�������ٸ���д���ļ�401
#define SUM 500
#define TICKCOUNT 73000
#define FS_TICKCOUNT 7300
#define OM_TICKCOUNT 650
//#define DATA_TIME_INTERVAL 60          //Ĭ��һ����
//------------------------


//------------------------
//2tickʱ���
#define TIMEOUT 300

#pragma region ����������ʱ������ 
//����������ʱ������ 

#define OPEN_MORNING 90000           //���̿���ʱ��
#define OPEN_NOON    130000			 //���̿���ʱ��
#define OPEN_EVENING 210000			 //ҹ�̿���ʱ��
#pragma endregion ����������ʱ������

#pragma region ͣ��ʱ��
//ͣ��ʱ��
#define CLOSE_MORNING 113000		//����ͣ��ʱ��
#define CLOSE_NOON	  150000		//����ͣ��ʱ��
#define CLOSE_EVENING 23000			//����ͣ��ʱ��

#define TIME_DIF      1500           //ƫ�����90000+1500 150000+1500
#pragma endregion ͣ��ʱ�� 

//------------------------

//////////////////////////////////////////////////////////////////////////
///�Զ����������-Tick------------��OperatorData.h���õ�  
///��Ҫ�����ߵ��ռ۸���ͳһ�۸�
typedef struct 
{
	///����������
	char	ExchangeID[15];
	///��Լ����
	char InstrumentID[10];
	///������
	int TradingDay;
	///����޸�ʱ��
	int	UpdateTime;
	///���¼�
	TThostFtdcPriceType	LastPrice;
	///�ɽ����
	TThostFtdcMoneyType	Turnover;
	///�ֲ���
	TThostFtdcLargeVolumeType	OpenInterest;
	//�ֲ�
	TThostFtdcLargeVolumeType OpenInterestRange;
	///�����һ
	TThostFtdcPriceType	BidPrice1;
	///������һ
	TThostFtdcPriceType	AskPrice1;
	///����
	TThostFtdcVolumeType	Volume;
	///������һ
	TThostFtdcVolumeType	BidVolume1;
	///������һ
	TThostFtdcVolumeType	AskVolume1;

}CThostFtdcDepthMarketDataField_Custom_Tick;
///�Զ����������-Other  today-1min
///K��ͼ�Ŀ��ߵ��ռ�
typedef struct{
	///����������
	char	ExchangeID[15];
	///��Լ����
	char InstrumentID[10];
	///������
	int TradingDay;
	///����޸�ʱ��
	int	UpdateTime;
	///����
	TThostFtdcPriceType	OpenPrice;
	///������
	TThostFtdcPriceType	ClosePrice;
	///��߼�
	TThostFtdcPriceType	HighestPrice;
	///��ͼ�
	TThostFtdcPriceType	LowestPrice;
	///�ɽ����
	TThostFtdcMoneyType	Turnover;
	///�ֲ���
	TThostFtdcLargeVolumeType	OpenInterest;
	//�ֲ�
	TThostFtdcLargeVolumeType OpenInterestRange;
	///�����һ
	TThostFtdcPriceType	BidPrice1;
	///������һ
	TThostFtdcPriceType	AskPrice1;
	///����
	TThostFtdcVolumeType	Volume;
	///������һ
	TThostFtdcVolumeType	BidVolume1;
	///������һ
	TThostFtdcVolumeType	AskVolume1;

}CThostFtdcDepthMarketDataField_Custom_Other;
//////////////////////////////////////////////////////////////////////////

///�Զ����������-Tick------------�����õ�   original��tick  
///��΢�޸���һ��CThostFtdcDepthMarketDataField,��Ҫ�����޸���һ������TradingDay����
///ȥ��һЩ����
typedef struct 
{
	///����������
	char	ExchangeID[15];
	///��Լ����
	char	InstrumentID[10];
	///������
	int	TradingDay;
	///���¼�
	TThostFtdcPriceType	LastPrice;
	///������
	TThostFtdcPriceType	PreClosePrice;
	///��ֲ���
	TThostFtdcLargeVolumeType	PreOpenInterest;
	///����
	TThostFtdcVolumeType	Volume;
	///�ɽ����
	TThostFtdcMoneyType	Turnover;
	///����
	TThostFtdcPriceType	OpenPrice;
	///������
	TThostFtdcPriceType	ClosePrice;
	///�ֲ���
	TThostFtdcLargeVolumeType	OpenInterest;
	///��ͣ���
	TThostFtdcPriceType	UpperLimitPrice;
	///��ͣ���
	TThostFtdcPriceType	LowerLimitPrice;
	///����ʵ��
	TThostFtdcRatioType	PreDelta;
	///����ʵ��
	TThostFtdcRatioType	CurrDelta;
	///����޸�ʱ��
	int	UpdateTime;
	///����޸ĺ���
	TThostFtdcMillisecType	UpdateMillisec;
	///������һ
	TThostFtdcVolumeType	BidVolume1;
	///������һ
	TThostFtdcVolumeType	AskVolume1;
	///�����һ
	TThostFtdcPriceType	BidPrice1;
	///������һ
	TThostFtdcPriceType	AskPrice1;	
	///���վ���
	TThostFtdcPriceType	AveragePrice;
	//�ֲ�
	TThostFtdcLargeVolumeType OpenInterestRange;
	///�������ݵ�ϵͳʱ��
	unsigned int ReceiveTime;
}CThostFtdcDepthMarketDataField_Custom;

//����
class CMdSpi : public CThostFtdcMdSpi
{
public:
	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);
		
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();
	
	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

private:
	void ReqUserLogin();
	void SubscribeMarketData();
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
};
// ��Լ��
class  CThostFtdcDepthMarketDataField_Custom_Clean_Contract{
	//�Զ�������
 public :
	CThostFtdcDepthMarketDataField_Custom data;
	//20151013                 tangb
	//�Ƿ��ǳ�ʼ����־  �����һ���Ƿ��fronttime_1��fronttime_2��ֵΪdatatemp->UpdateTime
	bool is_initflag;
	//��һ�����ݵ�ʱ��
	int fronttime_1;
	//���������ݵ�ʱ��
	int fronttime_2;
	///����޸ĺ��룬0��500������400��900������
	int	end_writetime_Millisec;
	//��Լ�Ƿ��޸Ĺ�
	bool flag_contract;
	//��һ�������Ƿ�
	bool flag_isfirst;
	//���캯��
	CThostFtdcDepthMarketDataField_Custom_Clean_Contract(){
		//data = new CThostFtdcDepthMarketDataField_Custom();	
		is_initflag = false;
		fronttime_1 = 0;
		fronttime_2 = 0;
		end_writetime_Millisec = 0;
		flag_contract= false;
		flag_isfirst=false;
	}
	~CThostFtdcDepthMarketDataField_Custom_Clean_Contract(){};
};
//����������״̬�࣬��Ҫ�����ȱ��©���ݡ�
class CThostFtdcDepthMarketDataField_Custom_Clean
{
public:	
	//ÿ���������ĵ�һ��ʱ��,�Ժ������
	unsigned int firsttime;
	//ÿ�������������һ��ʱ��,�Ժ������
	unsigned int lasttime;
	//�������Ƿ��޸Ĺ�
	bool flag_exchange;

	//��ϴ���ݵ�map��stringΪ��Լ��
	map<string,CThostFtdcDepthMarketDataField_Custom_Clean_Contract >totalTable;
	//����200msʱ
	void methodMoreThan200(CThostFtdcDepthMarketDataField_Custom &datatemp);
	//�ж������ظ�
	void methodIsRepetition(CThostFtdcDepthMarketDataField_Custom &datatemp);	

	//�ֲ�����
	void methodIsMising();
	//���캯��
	CThostFtdcDepthMarketDataField_Custom_Clean(){
		//totalTable = new map<string,CThostFtdcDepthMarketDataField_Custom_Clean_Contract >();
		firsttime = GetTickCount();
		lasttime = GetTickCount();
		flag_exchange = false;	
	}
	~CThostFtdcDepthMarketDataField_Custom_Clean(){};
};
//�������࣬��Ҫ����������ݣ�ȡ���ݣ�ת�����ݣ������ҳ�ÿ�ֺ�Լ���Ӧ�Ľ�����
class CMdSpi_Custom:public  CMdSpi
{
public:
	//ÿһ�ֺ�Լ��Ӧ�Ľ�����
	map<string,string>exchange_map;                                       //����ԼIF1510��������cffex��
	//��������ǰ�Ļ�����
	queue<CThostFtdcDepthMarketDataField_Custom> before_data_buffer;      
	//���������Ļ�����
	queue<CThostFtdcDepthMarketDataField_Custom> after_data_buffer;          
	//���������Ļ�����
	queue<CThostFtdcDepthMarketDataField_Custom> data_to_database;          //20150924 tangb ����д���ݿ�Ķ���,�� after_data_bufferһģһ��
	//���ֵ�һ�ʻ��ǵڶ��ʵĺ���������0ms����500ms
	//int MILLISEC;
	//�������ǰ����
	void methodAddDatas_Before(CThostFtdcDepthMarketDataField* data,unsigned int receivetime);
	//ȡ����ǰ����
	CThostFtdcDepthMarketDataField_Custom methodGetDatas_Before();
	//������������
	void methodAddDatas_After(CThostFtdcDepthMarketDataField_Custom data);
	//ȡ���������
	CThostFtdcDepthMarketDataField_Custom methodGetDatas_After();
	//ת������
	CThostFtdcDepthMarketDataField_Custom methodChangeToCustom(CThostFtdcDepthMarketDataField *data,unsigned int receivetime);
	//���캯��
	CMdSpi_Custom(){
	}
	~CMdSpi_Custom(){}
};

#endif MDSPI_H


