// Svm_Using.h : Svm_Using DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CSvm_UsingApp
// 有关此类实现的信息，请参阅 Svm_Using.cpp
//

class CSvm_UsingApp : public CWinApp
{
public:
	CSvm_UsingApp(float C=1,float gamma=1,float epsilon=0.1);  
    void TrainModel(vector<vector<double>>traindata);//数据训练  
    void Predict(float x,float &y);//拟合预测函数  

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	svm_parameter Initialize_svm_parameter(float C=1,float gamma=1,float epsilon=0.1);//参数初始化函数  
    svm_parameter m_svmpara;  
    svm_problem m_svmprob;  
    vector<vector<double>>m_traindata;//二维拟合数据  
    svm_model *m_svmmodel;  
    void GetMax_Min(double*pdata,int data_num,double &minpt,double&maxpt);//获取数据归一化的最大最小值  
    double *m_maxx;//归一化用的参数  
    double *m_minx;  
    double m_miny;  
    double m_maxy;  
};
