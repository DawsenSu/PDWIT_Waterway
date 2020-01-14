
using System;
using System.ComponentModel;
using System.IO;
using System.Reflection;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Imaging;
using System.Xml.Serialization;
using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.CommandWpf;
using Xceed.Wpf.Toolkit.PropertyGrid.Attributes;

using BM = Bentley.MstnPlatformNET;
using BD = Bentley.DgnPlatformNET;
using System.Collections.ObjectModel;

namespace PDIWT.Waterway.Lock.UI.ViewModel
{

    public class GuidewallP2ViewModel : ViewModelBase
        //, IDataErrorInfo
    {
        public GuidewallP2ViewModel(BM.AddIn addIn)
        {
            _addIn = addIn;
            _datas.Add(new GuidewallP2Data());
            _datas.Add(new GuidewallP2Data());
            _selectedData = _datas[0];
        }
        private BM.AddIn _addIn;

        private ObservableCollection<GuidewallP2Data> _datas = new ObservableCollection<GuidewallP2Data>();
        /// <summary>
        /// Property Description
        /// </summary>
        public ObservableCollection<GuidewallP2Data> Datas
        {
            get { return _datas; }
            set { Set(ref _datas, value); }
        }

        private GuidewallP2Data _selectedData;
        /// <summary>
        /// Property Description
        /// </summary>
        public GuidewallP2Data SelectedData
        {
            get { return _selectedData; }
            set { Set(ref _selectedData, value); }
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
                        //Window pictureWindow = new Window() { Title = "GuidewallP2 Picture", Width = 400, Height = 250, ResizeMode = ResizeMode.CanResize };
                        //Image image = new Image() { Stretch = System.Windows.Media.Stretch.Fill,
                        //    Source = new BitmapImage(new Uri(@"pack://application:,,,/PDIWT_Waterway_Lock.UI;component/Images/GuidewallP2.png", UriKind.RelativeOrAbsolute)) };
                        //pictureWindow.Content = image;
                        //pictureWindow.Show();
                        IllustrationWindow illustrationWindow = new IllustrationWindow(_addIn);
                        illustrationWindow.Show();
                    }));
            }
        }

        private RelayCommand _addToTemplate;

        /// <summary>
        /// Gets the AddToTemplate.
        /// </summary>
        public RelayCommand AddToTemplate
        {
            get
            {
                return _addToTemplate
                    ?? (_addToTemplate = new RelayCommand(ExecuteAddToTemplate));
            }
        }

        private void ExecuteAddToTemplate()
        {
            string worksetStandards = BD.ConfigurationManager.GetVariable("_USTN_WORKSETSTANDARDS");
            if(!Directory.Exists(worksetStandards))
            {
                MessageBox.Show($"_USTN_WORKSETSTANDARDS: {worksetStandards} doesn't exist.", "ERROR", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            string xmlFileName = "GuidewallP2.xml";
            XmlSerializer templateSerializer = new XmlSerializer(typeof(ObservableCollection<GuidewallP2Data>));
            TextWriter sw = new StreamWriter(worksetStandards + xmlFileName);
            templateSerializer.Serialize(sw, _datas);
            sw.Close();
            MessageBox.Show($"Save to {worksetStandards + xmlFileName}","Success");
        }
        //public string Error => null;

        //public string this[string columnName]
        //{
        //    get
        //    {
        //        StringBuilder sb = new StringBuilder();
        //        switch (columnName)
        //        {
        //            case "Data":
        //                //sb.Append("Test");
        //                double cushioncapHalfBottomWidth = (_data.CushioncapRearToeLength + _data.DolphinBottomWidth + _data.CushioncapFrontToeLength) / 2;
        //                if (_data.DolphinBottomElevation >= _data.DolphinTopElevation)
        //                    sb.AppendLine($"Error -> Dolphin bottom elevation[{_data.DolphinBottomElevation}] >= Dolphin top elevation[{_data.DolphinTopElevation}]");
        //                if (_data.CushioncapBottomElevation >= _data.DolphinBottomElevation)
        //                    sb.AppendLine($"Error -> Cushioncap bottom elevation[{_data.CushioncapBottomElevation}] >= Dolphin bottom elevation:[{_data.DolphinBottomElevation}]");
        //                if (_data.PileTopElevation >= _data.DolphinBottomElevation)
        //                    sb.AppendLine($"Error -> Pile top elevation:[{_data.PileTopElevation}] >= Pile bottom elevation:[{_data.DolphinBottomElevation}]");
        //                if (_data.PileTopElevation <= _data.CushioncapBottomElevation)
        //                    sb.AppendLine($"Error -> Pile top elevation:[{_data.PileTopElevation}] <= Cushioncap bottom elevation:[{_data.CushioncapBottomElevation}]");
        //                if (_data.PileBottomElevation >= _data.PileTopElevation)
        //                    sb.AppendLine($"Error -> Pile bottom elevation:[{_data.PileBottomElevation}] >= Pile top elevation:[{_data.PileTopElevation}]");
        //                if (_data.PileOffsetPerpendicularRiver <= _data.PileDiameter / 2)
        //                    sb.AppendLine($"Error -> Pile offset perpendicular river:[{_data.PileOffsetPerpendicularRiver}] <= Pile radius:[{_data.PileDiameter}]");
        //                if (_data.PileOffsetPerpendicularRiver >= cushioncapHalfBottomWidth - _data.PileDiameter / 2)
        //                    sb.AppendLine($"Error -> Pile offset perpendicular river:[{_data.PileOffsetPerpendicularRiver}] >= Cushioncap Half bottom width + pile radius:[{cushioncapHalfBottomWidth - _data.PileDiameter / 2}]");
        //                break;
        //        }
        //        return sb.ToString();
        //    }
        //}
    }

    [Serializable]
    [DisplayName("导航墙P2")]
    [CategoryOrder("墩柱", 0)]
    [CategoryOrder("承台", 1)]
    [CategoryOrder("桩基", 2)]
    [CategoryOrder("垫层", 3)]
    [CategoryOrder("挂板", 4)]
    [CategoryOrder("联系桥", 5)]
    public class GuidewallP2Data
    {
        [Category("墩柱")]
        [DisplayName("顶高程(m)")]
        [PropertyOrder(0)]
        public double DolphinTopElevation { get; set; } = 10;
        [Category("墩柱")]
        [DisplayName("底高程(m)")]
        [PropertyOrder(1)]
        public double DolphinBottomElevation { get; set; } = 0;
        double m_dolphinTopLength = 1.5;
        [Category("墩柱")]
        [DisplayName("顶长(m)")]
        [PropertyOrder(2)]
        public double DolphinTopLength
        {
            get { return m_dolphinTopLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinTopLength", "It should be greater than 0");
                }
                else
                    m_dolphinTopLength = value;
            }
        }
        double m_dolphinTopWidth = 1.8;
        [Category("墩柱")]
        [DisplayName("顶宽(m)")]
        [PropertyOrder(3)]
        public double DolphinTopWidth
        {
            get { return m_dolphinTopWidth; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinTopWidth", "It should be greater than 0");
                }
                else
                    m_dolphinTopWidth = value;
            }
        }
        double m_dolphinBottomLength = 1.5;
        [Category("墩柱")]
        [DisplayName("底长(m)")]
        [PropertyOrder(4)]
        public double DolphinBottomLength
        {
            get { return m_dolphinBottomLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinBottomLength", "It should be greater than 0");
                }
                else
                    m_dolphinBottomLength = value;
            }
        }
        double m_dolphinBottomWidth = 3.6;
        [Category("墩柱")]
        [DisplayName("底宽(m)")]
        [PropertyOrder(5)]
        public double DolphinBottomWidth
        {
            get { return m_dolphinBottomWidth; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinBottomWidth", "It should be greater than 0");
                }
                else
                    m_dolphinBottomWidth = value;
            }
        }

        [Category("承台")]
        [DisplayName("底高程(m)")]
        [PropertyOrder(0)]
        public double CushioncapBottomElevation { get; set; } = -2.5;
        double m_cushioncapFrontToeLength = 1;
        [Category("承台")]
        [DisplayName("前趾长(m)")]
        [PropertyOrder(1)]
        public double CushioncapFrontToeLength
        {
            get { return m_cushioncapFrontToeLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("CushionCapFrontToeLength", "It should be greater than 0");
                }
                else
                    m_cushioncapFrontToeLength = value;
            }
        }
        double m_cushioncapRearToeLength = 1;
        [Category("承台")]
        [DisplayName("后趾长(m)")]
        [PropertyOrder(2)]
        public double CushioncapRearToeLength
        {
            get { return m_cushioncapRearToeLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("CushioncapRearToeLength", "It should be greater than 0");
                }
                else
                    m_cushioncapRearToeLength = value;
            }
        }
        double m_cushioncapExtraSideLength = 0.25;
        [Category("承台")]
        [DisplayName("两侧超宽(m)")]
        [PropertyOrder(3)]
        public double CushioncapExtraSideLength
        {
            get { return m_cushioncapExtraSideLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("CushioncapExtraSideLength", "It should be greater than 0");
                }
                else
                    m_cushioncapExtraSideLength = value;
            }
        }
        double m_cushioncapChamferLegnth = 0.3;
        [Category("承台")]
        [DisplayName("前端倒角长(m)")]
        [PropertyOrder(4)]
        public double CushioncapChamferLength
        {
            get { return m_cushioncapChamferLegnth; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("CushioncapChamferLength", "It should be greater than 0");
                }
                else
                    m_cushioncapChamferLegnth = value;
            }
        }


        [Category("桩基")]
        [DisplayName("顶高程(m)")]
        [PropertyOrder(0)]
        public double PileTopElevation { get; set; } = -2.4;
        [Category("桩基")]
        [DisplayName("底高程(m)")]
        [PropertyOrder(1)]
        public double PileBottomElevation { get; set; } = -10;
        double m_pileDiameter = 1;
        [Category("桩基")]
        [DisplayName("直径(m)")]
        [PropertyOrder(2)]
        public double PileDiameter
        {
            get { return m_pileDiameter; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("PileDiamter", "It should be greater than 0");
                }
                else
                    m_pileDiameter = value;
            }
        }
        double m_pileWallThickness = 200;
        [Category("桩基")]
        [DisplayName("壁厚(mm)")]
        [PropertyOrder(3)]
        public double PileWallThickness
        {
            get { return m_pileWallThickness; }
            set
            {
                m_pileWallThickness = 200;
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("PileWallThickness", "It should be greater than 0");
                }
                else if (m_pileWallThickness >= m_pileDiameter * 1000 / 2)
                {
                    throw new ArgumentException("Pile wall thickness should be less than pile diameter", "Pile wall Thickness");
                }
                else
                    m_pileWallThickness = value;
            }
        }
        double m_pileoffsetPerpendicularRiver = 1.5;
        [Category("桩基")]
        [DisplayName("横河向边距(m)")]
        [PropertyOrder(4)]
        public double PileOffsetPerpendicularRiver
        {
            get { return m_pileoffsetPerpendicularRiver; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("PileOffsetPerpendicularRiver", "It should be greater than 0");
                }
                else
                    m_pileoffsetPerpendicularRiver = value;
            }
        }

        double m_cushionThickness = 100;
        [Category("垫层")]
        [DisplayName("厚度(mm)")]
        public double CushionThickness
        {
            get { return m_cushionThickness; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("CushionThickness", "It should be greater than 0");
                }
                else
                    m_cushionThickness = value;
            }
        }

        //[Category("挂板")]
        //[DisplayName("顶高程(m)")]
        //[PropertyOrder(0)]
        //public double WallTopElevation { get; set; }
        [Category("挂板")]
        [DisplayName("底高程(m)")]
        [PropertyOrder(1)]
        public double WallBottomElevation { get; set; } = 3;
        double m_wallThickness = 0.5;
        [Category("挂板")]
        [DisplayName("厚度(m)")]
        [PropertyOrder(2)]
        public double WallThickness
        {
            get { return m_wallThickness; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("WallThickness", "It should be greater than 0");
                }
                else
                    m_wallThickness = value;
            }
        }
        double m_wallLength = 10;
        [Category("挂板")]
        [DisplayName("长度(m)")]
        [PropertyOrder(3)]
        public double WallLength
        {
            get { return m_wallLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("WallLength", "It should be greater than 0");
                }
                else
                    m_wallLength = value;
            }
        }
        [Category("挂板")]
        [DisplayName("左侧挂板")]
        [PropertyOrder(4)]
        public bool HasLeftWall { get; set; } = true;
        [Category("挂板")]
        [DisplayName("右侧挂板")]
        [PropertyOrder(5)]
        public bool HasRightWall { get; set; } = true;

        double m_bridgeLength = 10;
        [Category("联系桥")]
        [DisplayName("桥长(m)")]
        [PropertyOrder(0)]
        public double BridgeLength
        {
            get { return m_bridgeLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("BridgeLegnth", "It should be greater than 0");
                }
                else
                    m_bridgeLength = value;
            }
        }
        double m_bridgeHeight = 500;
        [Category("联系桥")]
        [DisplayName("桥高(mm)")]
        [PropertyOrder(1)]
        public double BridgeHeight
        {
            get { return m_bridgeHeight; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("BridgeHeight", "It should be greater than 0");
                }
                else
                    m_bridgeHeight = value;
            }
        }
        double m_bridgeWidth = 580;
        [Category("联系桥")]
        [DisplayName("桥宽(mm)")]
        [PropertyOrder(2)]
        public double BridgeWidth
        {
            get { return m_bridgeWidth; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("BridgeWidth", "It should be greater than 0");
                }
                else
                    m_bridgeWidth = value;
            }
        }
        double m_bridgeTopBoardThickness = 120;
        [Category("联系桥")]
        [DisplayName("顶板厚(mm)")]
        [PropertyOrder(3)]
        public double BridgeTopBoardThickness
        {
            get { return m_bridgeTopBoardThickness; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("BridgeTopBoardThickness", "It should be greater than 0");
                }
                else
                    m_bridgeTopBoardThickness = value;
            }
        }
        double m_bridgeSupportWidth = 120;
        [Category("联系桥")]
        [DisplayName("支腿宽(mm)")]
        [PropertyOrder(4)]
        public double BridgeSupportWidth
        {
            get { return m_bridgeSupportWidth; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("BridgeSupportWidth", "It should be greater than 0");
                }
                else
                    m_bridgeSupportWidth = value;
            }
        }
        double m_bridgeChamferWidth = 50;
        [Category("联系桥")]
        [DisplayName("倒角尺寸(mm)")]
        [PropertyOrder(5)]
        public double BridgeChamferWidth
        {
            get { return m_bridgeChamferWidth; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("BridgeChamferWidth", "It should be greater than 0");
                }
                else
                    m_bridgeChamferWidth = value;
            }
        }
    }

}
