using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.CommandWpf;
using System;
using System.ComponentModel;
using System.Text;
using System.Windows;
using System.Xml.Serialization;
using Xceed.Wpf.Toolkit.PropertyGrid.Attributes;
using BM = Bentley.MstnPlatformNET;

namespace PDIWT.Waterway.Lock.UI.ViewModel
{
    using Model;
    public class DolphinGuidewallPlacementViewModel : ViewModelBase
    {
        #region MVVMBindingProperties

        private DolphinGuidewallPlacementData _selectedData = new DolphinGuidewallPlacementData();
        /// <summary>
        /// Property Description
        /// </summary>
        public DolphinGuidewallPlacementData SelectedData
        {
            get { return _selectedData; }
            set { Set(ref _selectedData, value); }
        }
        #endregion
    }
}
