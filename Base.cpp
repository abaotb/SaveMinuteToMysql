#include "Base.h"

//�жϺͺ�Լ�Ƿ����map
bool BaseDataInherit::isExist(string str) const
{
	if (this->tsmd_a != NULL)                   //ע����this
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
void  BaseDataInherit::addContractToMap(CThostFtdcDepthMarketDataField_Custom_Tick*ct) const 
{
	//��������
	BaseDataInherit* torigin= new  BaseDataInherit();
	torigin->contract=(string)(ct->InstrumentID);
	(*(this->tsmd_a))[(string)(ct->InstrumentID)] = torigin;							//ע����this,����torgin	
	torigin->TIME_INTERVAL = this->TIME_INTERVAL;										//�޸�ʱ����
	int remainder = TIME_INTERVAL >=60 ? 100 : TIME_INTERVAL;							//100//60	/**********Ҫ�ĵĵط�**********/
	torigin->compare_time = method_TimeToInt_60(ct->UpdateTime-ct->UpdateTime%remainder);
	//����1���ӱ�׼
	torigin->compare_data.Volume = 0;
	torigin->compare_data.Turnover = 0;
	torigin->compare_data.OpenInterest =0;//ct->OpenInterestRange;
	//��ʼ����һ���׶�
	torigin->methodInitNext(torigin,*ct);


}
//��������
void  BaseDataInherit::updateData(CThostFtdcDepthMarketDataField_Custom_Tick*ct) const
{
	BaseDataInherit * torigin= (*(this->tsmd_a))[(string)(ct->InstrumentID)];            //ע����this

	torigin->methodLogicalOperation(torigin,*ct);
}
//���ݵ��߼�����
void  BaseDataInherit::methodLogicalOperation(BaseData *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick &custom_tick)const
{	
	
	int time_60=method_TimeToInt_60(custom_tick.UpdateTime);

	//���˲�����۸�      20151125   
	if (priceRange(tsmdarrays->current_data,custom_tick))
	{
		return;
	}
	if(time_60 - tsmdarrays->compare_time < TIME_INTERVAL && time_60 - tsmdarrays->compare_time >=0)
	{
		
		//���������߼�
		if (custom_tick.LastPrice >tsmdarrays->current_data.HighestPrice )
		{
			//������߼�
			tsmdarrays->current_data.HighestPrice = custom_tick.LastPrice;

		}
		if(custom_tick.LastPrice <  tsmdarrays->current_data.LowestPrice)
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
	else if (time_60 - tsmdarrays->compare_time>=TIME_INTERVAL )		//20151217�� && custom_tick.UpdateTime%100 == 0
	{
		//����̨���
		CThostFtdcDepthMarketDataField_Custom_Other fff = tsmdarrays->current_data;
		//cerr << fff.InstrumentID<<" "<<fff.UpdateTime<<" ��߼ۣ�"<<fff.HighestPrice<<"��ͼۣ� "<<fff.LowestPrice<<" "<<endl;
		//cerr <</* fff.InstrumentID<<*/"        "<<fff.UpdateTime<<" �ɽ�����"<<fff.Volume<<" �ɽ��"<<fff.Turnover<<" �ֲ�����"<<fff.OpenInterest<<" �ֲ"<<fff.OpenInterestRange<<endl;
		//cerr << fff.InstrumentID<<"   "<<fff.UpdateTime<<" ���̼ۣ�"<<fff.OpenPrice<<" ���̼ۣ�"<<fff.ClosePrice<<endl;
		//дMysql���ݿ�
		//itoa(number, string,10);
		char  filename[10];
		itoa(TIME_INTERVAL, filename,10);
		methodWriteFile2(fff,(string)filename);//"today-1min"

		Ado_Connect_Mysql(fff);

		//����ʱ��
		tsmdarrays->compare_time = time_60;//tsmdarrays->time_oneminute + 60;
		//д������
		tsmdarrays->methodInitNext(tsmdarrays,custom_tick);

	}else if (time_60 - tsmdarrays->compare_time < 0)                       //20151217   ��
	{
		tsmdarrays->compare_time =0;
	}

};

//��ʼ����һ���׶�����
void  BaseDataInherit::methodInitNext(BaseData *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick &custom_tick) const
{
	//cerr << custom_tick.InstrumentID<<"       methodUpdate_Base "<<endl;
	//�����ոߵ͸���ͬһ�۸�	
	CThostFtdcDepthMarketDataField_Custom_Other co = tsmdarrays->methodTickToOther(custom_tick);
	tsmdarrays->current_data = co;      //����data_oneminute[650]����
	tsmdarrays->compare_data= co;       //���±�׼

};
//���Զ����tick����ת����5�������tick���ݻ���1���ӵ����ݣ������ոߵͼ�Ϊͬһ�۸�
CThostFtdcDepthMarketDataField_Custom_Other  BaseDataInherit::methodTickToOther
	(CThostFtdcDepthMarketDataField_Custom_Tick &original_data_tick) const
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