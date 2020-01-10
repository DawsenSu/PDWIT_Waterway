
using System;
using System.ComponentModel;
using System.Windows;
using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.CommandWpf;
using Xceed.Wpf.Toolkit.PropertyGrid.Attributes;

namespace PDIWT.Waterway.Lock.UI.ViewModel
{

    public class GuidewallP2ViewModel : ViewModelBase
    {

        private GuidewallP2Data _data = new GuidewallP2Data();
        /// <summary>
        /// Property Description
        /// </summary>
        public GuidewallP2Data Data
        {
            get { return _data; }
            set { Set(ref _data, value); }
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
                        MessageBox.Show($"The dopline top Elevation is {Data.DolphinTopElevation}");
                    }));
            }
        }
    }


    [Serializable]
    [DisplayName("导航墙P2")]
    [CategoryOrder("墩柱", 0)]
    [CategoryOrder("承台", 1)]
    [CategoryOrder("桩基", 2)]
    [CategoryOrder("垫层", 3)]
    [CategoryOrder("挂板", 4)]
    [CategoryOrder("联系桥",5)]
    public class GuidewallP2Data
    //:ViewModelBase
    {
        public GuidewallP2Data()
        {
            DolphinTopElevation = 10;
            DolphinBottomElevation = 1;
            DolphinTopLength = 1.5;
            DolphinTopWidth = 1.8;
            DolphinBottomLength = 1.5;
            DolphinTopWidth = 3.6;

            CushioncapBottomElevation = -2.5;
            CushioncapFrontToeLength = 1;
            CushioncapRearToeLength = 1;
            CushioncapExtraSideLength = 0.25;
            CushioncapChamferLength = 0.3;

            PileTopElevation = -2.4;
            PileBottomElevation = -10;
            PileDiameter = 1;
            PileWallThickness = 200;
            PileOffsetPerpendicularRiver = 1.5;

            CushionThickness = 100;

            WallTopElevation = 10.1;
            WallBottomElevation = 3;
            WallThickness = 0.5;
            WallLength = 10;
            HasLeftWall = true;
            HasRightWall = true;

            BridgeLength = 10;
            BridgeHeight = 500;
            BridgeWidth = 580;
            BridgeTopBoardThickness = 120;
            BridgeSupportWidth = 120;
            BridgeChamferWidth = 50;
        }
        [Category("墩柱")]
        [DisplayName("顶高程(m)")]
        [PropertyOrder(0)]
        public double DolphinTopElevation { get; set; }
        [Category("墩柱")]
        [DisplayName("底高程(m)")]
        [PropertyOrder(1)]
        public double DolphinBottomElevation { get; set; }
        [Category("墩柱")]
        [DisplayName("顶长(m)")]
        [PropertyOrder(2)]
        public double DolphinTopLength { get; set; }
        [Category("墩柱")]
        [DisplayName("顶宽(m)")]
        [PropertyOrder(3)]
        public double DolphinTopWidth { get; set; }
        [Category("墩柱")]
        [DisplayName("底长(m)")]
        [PropertyOrder(4)]
        public double DolphinBottomLength { get; set; }
        [Category("墩柱")]
        [DisplayName("底宽(m)")]
        [PropertyOrder(5)]
        public double DolphinBottomWidth { get; set; }

        [Category("承台")]
        [DisplayName("底高程(m)")]
        [PropertyOrder(0)]
        public double CushioncapBottomElevation { get; set; }
        [Category("承台")]
        [DisplayName("前趾长(m)")]
        [PropertyOrder(1)]
        public double CushioncapFrontToeLength { get; set; }
        [Category("承台")]
        [DisplayName("后趾长(m)")]
        [PropertyOrder(2)]
        public double CushioncapRearToeLength { get; set; }
        [Category("承台")]
        [DisplayName("两侧超宽(m)")]
        [PropertyOrder(3)]
        public double CushioncapExtraSideLength { get; set; }
        [Category("承台")]
        [DisplayName("前端倒角长(m)")]
        [PropertyOrder(4)]
        public double CushioncapChamferLength { get; set; }


        [Category("桩基")]
        [DisplayName("顶高程(m)")]
        [PropertyOrder(0)]
        public double PileTopElevation { get; set; }
        [Category("桩基")]
        [DisplayName("底高程(m)")]
        [PropertyOrder(1)]
        public double PileBottomElevation { get; set; }
        [Category("桩基")]
        [DisplayName("直径(m)")]
        [PropertyOrder(2)]
        public double PileDiameter { get; set; }
        [Category("桩基")]
        [DisplayName("壁厚(mm)")]
        [PropertyOrder(3)]
        public double PileWallThickness { get; set; }
        [Category("桩基")]
        [DisplayName("横河向边距(m)")]
        [PropertyOrder(4)]
        public double PileOffsetPerpendicularRiver { get; set; }

        [Category("垫层")]
        [DisplayName("厚度(mm)")]
        public double CushionThickness { get; set; }

        [Category("挂板")]
        [DisplayName("顶高程(m)")]
        [PropertyOrder(0)]
        public double WallTopElevation { get; set; }
        [Category("挂板")]
        [DisplayName("底高程(m)")]
        [PropertyOrder(1)]
        public double WallBottomElevation { get; set; }
        [Category("挂板")]
        [DisplayName("厚度(m)")]
        [PropertyOrder(2)]
        public double WallThickness { get; set; }
        [Category("挂板")]
        [DisplayName("长度(m)")]
        [PropertyOrder(3)]
        public double WallLength { get; set; }
        [Category("挂板")]
        [DisplayName("左侧挂板")]
        [PropertyOrder(4)]
        public bool HasLeftWall { get; set; }
        [Category("挂板")]
        [DisplayName("右侧挂板")]
        [PropertyOrder(5)]
        public bool HasRightWall { get; set; }

        [Category("联系桥")]
        [DisplayName("桥长(m)")]
        [PropertyOrder(0)]
        public double BridgeLength { get; set; }
        [Category("联系桥")]
        [DisplayName("桥高(mm)")]
        [PropertyOrder(1)]
        public double BridgeHeight { get; set; }
        [Category("联系桥")]
        [DisplayName("桥宽(mm)")]
        [PropertyOrder(2)]
        public double BridgeWidth { get; set; }
        [Category("联系桥")]
        [DisplayName("顶板厚(mm)")]
        [PropertyOrder(3)]
        public double BridgeTopBoardThickness { get; set; }
        [Category("联系桥")]
        [DisplayName("支腿宽(mm)")]
        [PropertyOrder(4)]
        public double BridgeSupportWidth { get; set; }
        [Category("联系桥")]
        [DisplayName("倒角尺寸(mm)")]
        [PropertyOrder(5)]
        public double BridgeChamferWidth { get; set; }
    }
}
