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
using PDIWT.Waterway.Lock.UI;

namespace PDIWT.Waterway.Lock.UI.ViewModel
{
    public class MountPierP1ViewModel: ViewModelBase
    {
        public MountPierP1ViewModel(BM.AddIn addIn)
        {
            _addIn = addIn;
            _datas.Add(new MountPierP1Data());
            //_datas.Add(new GuidewallP2Data());
            _selectedData = _datas[0];
        }
        private BM.AddIn _addIn;

        private ObservableCollection<MountPierP1Data> _datas = new ObservableCollection<MountPierP1Data>();
        /// <summary>
        /// Property Description
        /// </summary>
        public ObservableCollection<MountPierP1Data> Datas
        {
            get { return _datas; }
            set { Set(ref _datas, value); }
        }

        private MountPierP1Data _selectedData;
        /// <summary>
        /// Property Description
        /// </summary>
        public MountPierP1Data SelectedData
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
                        IllustrationForDolphinP11 illustrationWindow = new IllustrationForDolphinP11(_addIn);
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
            if (!Directory.Exists(worksetStandards))
            {
                MessageBox.Show($"_USTN_WORKSETSTANDARDS: {worksetStandards} doesn't exist.", "ERROR", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            string xmlFileName = "MountPierP1.xml";
            XmlSerializer templateSerializer = new XmlSerializer(typeof(ObservableCollection<MountPierP1Data>));
            TextWriter sw = new StreamWriter(worksetStandards + xmlFileName);
            templateSerializer.Serialize(sw, _datas);
            sw.Close();
            MessageBox.Show($"Save to {worksetStandards + xmlFileName}", "Success");
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
    [DisplayName("靠船墩P11")]
    [CategoryOrder("墩柱", 0)]
    [CategoryOrder("承台", 1)]
    [CategoryOrder("垫层", 2)]
    public class MountPierP1Data
    {
        [Category("墩柱")]
        [DisplayName("顶高程(m)")]
        [PropertyOrder(0)]
        public double DolphinColumnTopElevation { get; set; } = 10;
        [Category("墩柱")]
        [DisplayName("底高程(m)")]
        [PropertyOrder(1)]
        public double DolphinColumnBottomElevation { get; set; } = 0;
        double m_dolphinColumnTopLength = 3.5;
        [Category("墩柱")]
        [DisplayName("顶长(m)")]
        [PropertyOrder(2)]
        public double DolphinColumnTopLength
        {
            get { return m_dolphinColumnTopLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnTopLength", "It should be greater than 0");
                }
                else
                    m_dolphinColumnTopLength = value;
            }
        }
        double m_dolphinColumnTopWidth = 3.5;
        [Category("墩柱")]
        [DisplayName("顶宽(m)")]
        [PropertyOrder(3)]
        public double DolphinColumnTopWidth
        {
            get { return m_dolphinColumnTopWidth; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnTopWidth", "It should be greater than 0");
                }
                else
                    m_dolphinColumnTopWidth = value;
            }
        }
        double m_dolphinColumnTopEdgeRadius = 0.1;
        [Category("墩柱")]
        [DisplayName("顶边倒角半径(m)")]
        [PropertyOrder(4)]
        public double DolphinColumnTopEdgeRadius
        {
            get { return m_dolphinColumnTopEdgeRadius; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnTopEdgeRadius", "It should be greater than 0");
                }
                else
                    m_dolphinColumnTopEdgeRadius = value;
            }
        }
        double m_dolphinColumnSideEdgeRadius = 0.3;
        [Category("墩柱")]
        [DisplayName("侧边倒角半径(m)")]
        [PropertyOrder(5)]
        public double DolphinColumnSideEdgeRadius
        {
            get { return m_dolphinColumnSideEdgeRadius; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnSideEdgeRadius", "It should be greater than 0");
                }
                else
                    m_dolphinColumnSideEdgeRadius = value;
            }
        }
        double m_dolphinColumnBottomLength = 3.5;
        [Category("墩柱")]
        [DisplayName("底长(m)")]
        [PropertyOrder(6)]
        public double DolphinColumnBottomLength
        {
            get { return m_dolphinColumnBottomLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnBottomLength", "It should be greater than 0");
                }
                else
                    m_dolphinColumnBottomLength = value;
            }
        }
        double m_dolphinColumnBottomWidth = 5;
        [Category("墩柱")]
        [DisplayName("底宽(m)")]
        [PropertyOrder(7)]
        public double DolphinColumnBottomWidth
        {
            get { return m_dolphinColumnBottomWidth; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnBottomWidth", "It should be greater than 0");
                }
                else
                    m_dolphinColumnBottomWidth = value;
            }
        }
        double m_dolphinColumnHaunchHeight = 1;
        [Category("墩柱")]
        [DisplayName("底部倒角高度(m)")]
        [PropertyOrder(8)]
        public double DolphinColumnHaunchHeight
        {
            get { return m_dolphinColumnHaunchHeight; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnHaunchHeight", "It should be greater than 0");
                }
                else
                    m_dolphinColumnHaunchHeight = value;
            }
        }
        double m_dolphinColumnHaunchLength = 0.5;
        [Category("墩柱")]
        [DisplayName("底部倒角长度(m)")]
        [PropertyOrder(9)]
        public double DolphinColumnHaunchLength
        {
            get { return m_dolphinColumnHaunchLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnHaunchLength", "It should be greater than 0");
                }
                else
                    m_dolphinColumnHaunchLength = value;
            }
        }
        double m_angleOfFirstPolylineWall = 0;
        [Category("墩柱")]
        [DisplayName("第一折线墙背角度(°)")]
        [PropertyOrder(10)]
        public double AngleOfFirstPolylineWall
        {
            get { return m_angleOfFirstPolylineWall; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("AngleOfFirstPolylineWall", "It should be greater than 0");
                }
                else
                    m_angleOfFirstPolylineWall = value;
            }
        }
        double m_heightOfFirstPolylineWall = 4;
        [Category("墩柱")]
        [DisplayName("第一折线墙背高度(m)")]
        [PropertyOrder(11)]
        public double HeightOfFirstPolylineWall
        {
            get { return m_heightOfFirstPolylineWall; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("HeightOfFirstPolylineWall", "It should be greater than 0");
                }
                else
                    m_heightOfFirstPolylineWall = value;
            }
        }
        [Category("承台")]
        [DisplayName("底高程(m)")]
        [PropertyOrder(0)]
        public double CushionCapBottomElevation { get; set; } = -1.5;
        double m_frontToeLength = 1;
        [Category("承台")]
        [DisplayName("前趾长(m)")]
        [PropertyOrder(1)]
        public double FrontToeLength
        {
            get { return m_frontToeLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("FrontToeLength", "It should be greater than 0");
                }
                else
                    m_frontToeLength = value;
            }
        }
        double m_backToeLength = 1;
        [Category("承台")]
        [DisplayName("后趾长(m)")]
        [PropertyOrder(2)]
        public double BackToeLength
        {
            get { return m_backToeLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("BackToeLength", "It should be greater than 0");
                }
                else
                    m_backToeLength = value;
            }
        }
        double m_cushionCapExtraWideLength = 1;
        [Category("承台")]
        [DisplayName("两侧超宽(m)")]
        [PropertyOrder(3)]
        public double CushionCapExtraWideLength
        {
            get { return m_cushionCapExtraWideLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("CushionCapExtraWideLength", "It should be greater than 0");
                }
                else
                    m_cushionCapExtraWideLength = value;
            }
        }

        double m_capThickness = 100;
        [Category("垫层")]
        [DisplayName("厚度(mm)")]
        public double CapThickness
        {
            get { return m_capThickness; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("CapThickness", "It should be greater than 0");
                }
                else
                    m_capThickness = value;
            }
        }

        double m_pileDiameterOut = 1;
        [Category("桩基")]
        [DisplayName("外径(m)")]
        public double PileDiameterOut
        {
            get { return m_pileDiameterOut; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("PileDiameterOut", "It should be greater than 0");
                }
                else
                    m_pileDiameterOut = value;
            }
        }

        double m_pileWallThick = 100;
        [Category("桩基")]
        [DisplayName("桩壁厚(mm)")]
        public double PileWallThick
        {
            get { return m_pileWallThick; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("pileWallThick", "It should be greater than 0");
                }
                else
                    m_pileWallThick = value;
            }
        }

        double m_pileSideDistV=1.2;
        [Category("桩基")]
        [DisplayName("横河桩边距(m)")]
        public double PileSideDistV
        {
            get { return m_pileSideDistV; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("PileSideDistV", "It should be greater than 0");
                }
                else
                    m_pileSideDistV = value;
            }
        }
        double m_pileSideDistH=1.2;
        [Category("桩基")]
        [DisplayName("顺河桩边距(m)")]
        public double PileSideDistH
        {
            get { return m_pileSideDistH; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("PileSideDistH", "It should be greater than 0");
                }
                else
                    m_pileSideDistH = value;
            }
        }

        //double m_pileTopElevation;
        [Category("桩基")]
        [DisplayName("桩顶高程(m)")]
        public double PileTopElevation
        {
            get { return CushionCapBottomElevation + 0.1; }
        }

        double m_pileBottomElevation = -16.5;
        [Category("桩基")]
        [DisplayName("桩底高程(m)")]
        public double PileBottomElevation
        {
            get { return m_pileBottomElevation; }
            set
            {
                if (value > PileTopElevation)
                {
                    throw new ArgumentOutOfRangeException("m_pileBottomElevation", "It should be less than PileTopElevation");
                }
                else
                    m_pileBottomElevation = value;
            }
        }
    }
}
