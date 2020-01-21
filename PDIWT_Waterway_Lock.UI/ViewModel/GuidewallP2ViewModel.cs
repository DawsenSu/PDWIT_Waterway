﻿using GalaSoft.MvvmLight;
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
    public class GuidewallP2ViewModel : ViewModelBase
        , IDataErrorInfo
    {
        public GuidewallP2ViewModel(BM.AddIn addIn)
        {
            _addIn = addIn;
        }

        private BM.AddIn _addIn;

        #region MVVMBindingProperties

        private GuidewallP2Data _selectedData = new GuidewallP2Data() { Name = "Default" };

        /// <summary>
        /// Property Description
        /// </summary>
        public GuidewallP2Data SelectedData
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
            PickupTemplateWindow pickupTemplateWindow = new PickupTemplateWindow()
            {
                DataContext = new PickupTemplateViewModel(this),
                WindowStartupLocation = WindowStartupLocation.Manual
            };
            pickupTemplateWindow.ShowDialog();
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
                        //sb.Append("Test");
                        double cushioncapHalfBottomWidth = (_selectedData.CushioncapRearToeLength + _selectedData.DolphinBottomWidth + _selectedData.CushioncapFrontToeLength) / 2;
                        if (_selectedData.DolphinBottomElevation >= _selectedData.DolphinTopElevation)
                            sb.AppendLine($"Error -> Dolphin bottom elevation:[{_selectedData.DolphinBottomElevation}] >= Dolphin top elevation:[{_selectedData.DolphinTopElevation}]");
                        if (_selectedData.CushioncapBottomElevation >= _selectedData.DolphinBottomElevation)
                            sb.AppendLine($"Error -> Cushioncap bottom elevation:[{_selectedData.CushioncapBottomElevation}] >= Dolphin bottom elevation:[{_selectedData.DolphinBottomElevation}]");
                        if (_selectedData.WallBottomElevation - 0.02 <= _selectedData.DolphinBottomElevation)
                            sb.AppendLine($"Error -> Wall bottom elevation:[{_selectedData.WallBottomElevation - 0.02}] <= Dolphin Bottom Elevation:[{_selectedData.DolphinBottomElevation}]");
                        if (_selectedData.PileTopElevation >= _selectedData.DolphinBottomElevation)
                            sb.AppendLine($"Error -> Pile top elevation:[{_selectedData.PileTopElevation}] >= Pile bottom elevation:[{_selectedData.DolphinBottomElevation}]");
                        if (_selectedData.PileTopElevation <= _selectedData.CushioncapBottomElevation)
                            sb.AppendLine($"Error -> Pile top elevation:[{_selectedData.PileTopElevation}] <= Cushioncap bottom elevation:[{_selectedData.CushioncapBottomElevation}]");
                        if (_selectedData.PileBottomElevation >= _selectedData.PileTopElevation)
                            sb.AppendLine($"Error -> Pile bottom elevation:[{_selectedData.PileBottomElevation}] >= Pile top elevation:[{_selectedData.PileTopElevation}]");
                        if (_selectedData.PileOffsetPerpendicularRiver <= _selectedData.PileDiameter / 2)
                            sb.AppendLine($"Error -> Pile offset perpendicular river:[{_selectedData.PileOffsetPerpendicularRiver}] <= Pile radius:[{_selectedData.PileDiameter}]");
                        if (_selectedData.PileOffsetPerpendicularRiver >= cushioncapHalfBottomWidth - _selectedData.PileDiameter / 2)
                            sb.AppendLine($"Error -> Pile offset perpendicular river:[{_selectedData.PileOffsetPerpendicularRiver}] >= Cushioncap Half bottom width + pile radius:[{cushioncapHalfBottomWidth - _selectedData.PileDiameter / 2}]");
                        break;
                }
                return sb.ToString();
            }
        }

        #endregion IDataErrorInfo
    }

}