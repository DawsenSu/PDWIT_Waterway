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
    using PDIWT.Waterway.Lock.UI.Model;
    public class GuidewallP3ViewModel : ViewModelBase
        , IDataErrorInfo
    {
        public GuidewallP3ViewModel(BM.AddIn addIn)
        {
            _addIn = addIn;
        }

        private BM.AddIn _addIn;

        #region MVVMBindingProperties

        private GuidewallP3Data _selectedData = new GuidewallP3Data() ;

        /// <summary>
        /// Property Description
        /// </summary>
        public GuidewallP3Data SelectedData
        {
            get { return _selectedData; }
            set { Set(ref _selectedData, value); }
        }

        #endregion MVVMBindingProperties

        #region MVVMRelayCommands

        private RelayCommand _propertyValueChanged;

        /// <summary>
        /// Gets the PropertyValueChanged.
        /// </summary>
        public RelayCommand PropertyValueChanged
        {
            get
            {
                return _propertyValueChanged
                    ?? (_propertyValueChanged = new RelayCommand(
                    () =>
                    {
                        RaisePropertyChanged("SelectedData");
                    }));
            }
        }

        private RelayCommand _showPicture;

        /// <summary>
        /// Gets the ShowPicture.
        /// </summary>
        public RelayCommand ShowPicture
        {
            get
            {
                return _showPicture
                    ?? (_showPicture = new RelayCommand(
                    () =>
                    {
                        IllustrationWindow illustrationWindow = new IllustrationWindow(_addIn);
                        illustrationWindow.Show();
                    }));
            }
        }

        private RelayCommand _showTemplates;

        /// <summary>
        /// Gets the AddToTemplate.
        /// </summary>
        public RelayCommand ShowTemplates
        {
            get
            {
                return _showTemplates
                    ?? (_showTemplates = new RelayCommand(ExecuteShowTemplates));
            }
        }

        private void ExecuteShowTemplates()
        {
            //PickupTemplateWindow pickupTemplateWindow = new PickupTemplateWindow()
            //{
            //    DataContext = new PickupTemplateViewModel(this),
            //    WindowStartupLocation = WindowStartupLocation.Manual
            //};
            //pickupTemplateWindow.ShowDialog();
        }

        #endregion MVVMRelayCommands

        #region IDataErrorInfo

        public string Error => null;

        public string this[string columnName]
        {
            get
            {
                StringBuilder sb = new StringBuilder();
                switch (columnName)
                {
                    case "SelectedData":
                        break;
                }
                return sb.ToString();
            }
        }

        #endregion IDataErrorInfo
    }

}