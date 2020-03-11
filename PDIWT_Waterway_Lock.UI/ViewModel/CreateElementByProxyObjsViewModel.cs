using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.Messaging;
using GalaSoft.MvvmLight.CommandWpf;
using System;
using System.ComponentModel;
using System.Collections.ObjectModel;
using System.Text;
using System.Windows;
using System.Xml.Serialization;
using Xceed.Wpf.Toolkit.PropertyGrid.Attributes;
using BM = Bentley.MstnPlatformNET;

namespace PDIWT.Waterway.Lock.UI.ViewModel
{
    using Model;
    using System.Collections.Generic;

    public class CreateElementByProxyObjsViewModel: ViewModelBase
    {
        public CreateElementByProxyObjsViewModel(ICreateElementByProxyObjs callInterface)
        {
            _nativeManagedInterface = callInterface;
        }
        private readonly ICreateElementByProxyObjs _nativeManagedInterface;


        private ObservableCollection<ProxyObjsInfo> _ProxyEntities = new ObservableCollection<ProxyObjsInfo>();
        /// <summary>
        /// Property Description
        /// </summary>
        public ObservableCollection<ProxyObjsInfo> ProxyEntities
        {
            get { return _ProxyEntities; }
            set { Set(ref _ProxyEntities, value); }
        }
        

        private RelayCommand _LoadProxyObjs;

        /// <summary>
        /// Gets the LoadProxyObjs.
        /// </summary>
        public RelayCommand LoadProxyObjs
        {
            get
            {
                return _LoadProxyObjs
                    ?? (_LoadProxyObjs = new RelayCommand(
                    () =>
                    {
                        //ProxyEntities.Add(new ProxyObjsInfo() { Type = "1", Parameters = "jklsf" });
                        List<ProxyObjsInfo> _infos = _nativeManagedInterface.PopulateProxyObjsListFromActiveModel();
                        ProxyEntities = new ObservableCollection<ProxyObjsInfo>(_infos);
                    }));
            }
        }

        private RelayCommand _CreateSolidEntities;

        /// <summary>
        /// Gets the CreateSolidEntities.
        /// </summary>
        public RelayCommand CreateSolidEntities
        {
            get
            {
                return _CreateSolidEntities
                    ?? (_CreateSolidEntities = new RelayCommand(
                    () =>
                    {
                        _nativeManagedInterface.CreateSolidByProxyObject(new List<ProxyObjsInfo>(ProxyEntities));
                        //_nativeManagedInterface.CallInUnmanged();
                    }));
            }
        }
    }
}
