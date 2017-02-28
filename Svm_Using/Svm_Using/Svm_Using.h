// Svm_Using.h : Svm_Using DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSvm_UsingApp
// �йش���ʵ�ֵ���Ϣ������� Svm_Using.cpp
//

class CSvm_UsingApp : public CWinApp
{
public:
	CSvm_UsingApp(float C=1,float gamma=1,float epsilon=0.1);  
    void TrainModel(vector<vector<double>>traindata);//����ѵ��  
    void Predict(float x,float &y);//���Ԥ�⺯��  

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	svm_parameter Initialize_svm_parameter(float C=1,float gamma=1,float epsilon=0.1);//������ʼ������  
    svm_parameter m_svmpara;  
    svm_problem m_svmprob;  
    vector<vector<double>>m_traindata;//��ά�������  
    svm_model *m_svmmodel;  
    void GetMax_Min(double*pdata,int data_num,double &minpt,double&maxpt);//��ȡ���ݹ�һ���������Сֵ  
    double *m_maxx;//��һ���õĲ���  
    double *m_minx;  
    double m_miny;  
    double m_maxy;  
};
