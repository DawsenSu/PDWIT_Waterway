# PDWIT_Waterway
Water way component.  
## Set the environment variables so that you can use Intellitrace: MSINC to your Bentley SDK include foder; MSLIB to your Bentley Libs folder; MS to your bentley installation folder.
MSINC -> C:\Program Files\Bentley\MicroStationCONNECTSDK\include  
MSLIB -> C:\Program Files\Bentley\MicroStationCONNECTSDK\library  
MS -> C:\Program Files\Bentley\MicroStation CONNECT Edition\MicroStation  

### 要运行此项目需要进行以下配置
1.	打开对应目录下的文件 ~\Configuration\Organization\standards.cfg, 添加以下环境变量  
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
2.	新建文件夹 ~\Configuration\Organization\ECSchemas,并将PDWT_Waterway.01.00.ecschema.xml文件拷贝至此文件夹下  
