# PDWIT_Waterway
Water way component.  
## Set the environment variables so that you can use Intellitrace: MSINC to your Bentley SDK include foder; MSLIB to your Bentley Libs folder; MS to your bentley installation folder.
MSINC -> C:\Program Files\Bentley\MicroStationCONNECTSDK\include  
MSLIB -> C:\Program Files\Bentley\MicroStationCONNECTSDK\library  
MS -> C:\Program Files\Bentley\MicroStation CONNECT Edition\MicroStation  

### Ҫ���д���Ŀ��Ҫ������������
1.	�򿪶�ӦĿ¼�µ��ļ� ~\Configuration\Organization\standards.cfg, ������»�������  
	\#----------------------------------------------------------------------  
	\# Define PDIWT Organization Level ECSchemas folder  
	\#----------------------------------------------------------------------  
	PDIWT_ORGANIZATION_ECSCHEMAPATH = $(_USTN_ORGANIZATION)ECSchemas/  
	PDIWT_ORGANIZATION_DATABASEPATH = $(_USTN_ORGANIZATION)data/  
	PDIWT_ORGANIZATION_WORDTEMPLATE = $(PDIWT_ORGANIZATION_DATABASEPATH)WordTemplate/  
	\#----------------------------------------------------------------------
	\# Add the Addin path
	\#----------------------------------------------------------------------  
	MS_ADDINPATH > $(_USTN_ORGANIZATION)Mdlapps/  
	\#----------------------------------------------------------------------  
	\# Add addin dependencypath to allow microstation application to search  
	\# directories which contains the dependent dll.  
	\#----------------------------------------------------------------------  
	MS_ADDIN_DEPENDENCYPATH > $(_USTN_ORGANIZATION)Mdlapps/  
2.	�½��ļ��� ~\Configuration\Organization\ECSchemas,����PDWT_Waterway.01.00.ecschema.xml�ļ����������ļ�����  
