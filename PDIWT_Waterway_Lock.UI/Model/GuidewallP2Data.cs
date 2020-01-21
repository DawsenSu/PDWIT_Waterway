using System;
using System.ComponentModel;
using System.Text;
using System.Windows;
using System.Reflection;
using System.Xml.Serialization;
using Xceed.Wpf.Toolkit.PropertyGrid.Attributes;

namespace PDIWT.Waterway.Lock.UI.Model
{
    [Serializable]
    [DisplayName("导航墙P2")]
    [CategoryOrder("墩柱", 0)]
    [CategoryOrder("承台", 1)]
    [CategoryOrder("桩基", 2)]
    [CategoryOrder("垫层", 3)]
    [CategoryOrder("挂板", 4)]
    [CategoryOrder("联系桥", 5)]
    [ECClass("PDIWT_Waterway", "GuidewallP2")]
    public class GuidewallP2Data
    {
        [Browsable(false)]
        [XmlAttribute]
        public string Name { get; set; } = "";

        [Category("墩柱")]
        [DisplayName("顶高程(m)")]
        [PropertyOrder(0)]
        [ECProperty("Parameters.DolphinTopElevation")]
        [UnitScale(Units.Meter)]
        public double DolphinTopElevation { get; set; } = 10;

        [Category("墩柱")]
        [DisplayName("底高程(m)")]
        [PropertyOrder(1)]
        [ECProperty("Parameters.DolphinBottomElevation")]
        [UnitScale(Units.Meter)]
        public double DolphinBottomElevation { get; set; } = 0;

        private double m_dolphinTopLength = 1.5;

        [Category("墩柱")]
        [DisplayName("顶长(m)")]
        [PropertyOrder(2)]
        [ECProperty("Parameters.DolphinTopLength")]
        [UnitScale(Units.Meter)]
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

        private double m_dolphinTopWidth = 1.8;

        [Category("墩柱")]
        [DisplayName("顶宽(m)")]
        [PropertyOrder(3)]
        [ECProperty("Parameters.DolphinTopWidth")]
        [UnitScale(Units.Meter)]
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

        private double m_dolphinBottomLength = 1.5;

        [Category("墩柱")]
        [DisplayName("底长(m)")]
        [PropertyOrder(4)]
        [ECProperty("Parameters.DolphinBottomLength")]
        [UnitScale(Units.Meter)]
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

        private double m_dolphinBottomWidth = 3.6;

        [Category("墩柱")]
        [DisplayName("底宽(m)")]
        [PropertyOrder(5)]
        [ECProperty("Parameters.DolphinBottomWidth")]
        [UnitScale(Units.Meter)]
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
        [ECProperty("Parameters.CushioncapBottomElevation")]
        [UnitScale(Units.Meter)]
        public double CushioncapBottomElevation { get; set; } = -2.5;

        private double m_cushioncapFrontToeLength = 1;

        [Category("承台")]
        [DisplayName("前趾长(m)")]
        [PropertyOrder(1)]
        [ECProperty("Parameters.CushioncapFrontToeLength")]
        [UnitScale(Units.Meter)]
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

        private double m_cushioncapRearToeLength = 1;

        [Category("承台")]
        [DisplayName("后趾长(m)")]
        [PropertyOrder(2)]
        [ECProperty("Parameters.CushioncapRearToeLength")]
        [UnitScale(Units.Meter)]
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

        private double m_cushioncapExtraSideLength = 0.25;

        [Category("承台")]
        [DisplayName("两侧超宽(m)")]
        [PropertyOrder(3)]
        [ECProperty("Parameters.CushioncapExtraSideLength")]
        [UnitScale(Units.Meter)]
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

        private double m_cushioncapChamferLegnth = 0.3;

        [Category("承台")]
        [DisplayName("前端倒角长(m)")]
        [PropertyOrder(4)]
        [ECProperty("Parameters.CushioncapChamferLength")]
        [UnitScale(Units.Meter)]
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
        [ECProperty("Parameters.PileTopElevation")]
        [UnitScale(Units.Meter)]
        public double PileTopElevation { get; set; } = -2.4;

        [Category("桩基")]
        [DisplayName("底高程(m)")]
        [PropertyOrder(1)]
        [ECProperty("Parameters.PileBottomElevation")]
        [UnitScale(Units.Meter)]
        public double PileBottomElevation { get; set; } = -10;

        private double m_pileDiameter = 1;

        [Category("桩基")]
        [DisplayName("直径(m)")]
        [PropertyOrder(2)]
        [ECProperty("Parameters.PileDiameter")]
        [UnitScale(Units.Meter)]
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

        private double m_pileWallThickness = 200;

        [Category("桩基")]
        [DisplayName("壁厚(mm)")]
        [PropertyOrder(3)]
        [ECProperty("Parameters.PileWallThickness")]
        [UnitScale(Units.Millimeter)]
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

        private double m_pileoffsetPerpendicularRiver = 1.5;

        [Category("桩基")]
        [DisplayName("横河向边距(m)")]
        [PropertyOrder(4)]
        [ECProperty("Parameters.PileOffsetPerpendicularRiver")]
        [UnitScale(Units.Meter)]
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

        private double m_cushionThickness = 100;

        [Category("垫层")]
        [DisplayName("厚度(mm)")]
        [ECProperty("Parameters.CushionThickness")]
        [UnitScale(Units.Millimeter)]
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
        [ECProperty("Parameters.WallBottomElevation")]
        [UnitScale(Units.Meter)]
        public double WallBottomElevation { get; set; } = 3;

        private double m_wallThickness = 0.5;

        [Category("挂板")]
        [DisplayName("厚度(m)")]
        [PropertyOrder(2)]
        [ECProperty("Parameters.WallThickness")]
        [UnitScale(Units.Meter)]
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

        private double m_wallLength = 10;

        [Category("挂板")]
        [DisplayName("长度(m)")]
        [PropertyOrder(3)]
        [ECProperty("Parameters.WallLength")]
        [UnitScale(Units.Meter)]
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
        [ECProperty("Parameters.HasLeftWall")]
        public bool HasLeftWall { get; set; } = true;

        [Category("挂板")]
        [DisplayName("右侧挂板")]
        [PropertyOrder(5)]
        [ECProperty("Parameters.HasRightWall")]
        public bool HasRightWall { get; set; } = true;

        private double m_bridgeLength = 10;

        [Category("联系桥")]
        [DisplayName("桥长(m)")]
        [PropertyOrder(0)]
        [ECProperty("Parameters.BridgeLength")]
        [UnitScale(Units.Meter)]
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

        private double m_bridgeHeight = 500;

        [Category("联系桥")]
        [DisplayName("桥高(mm)")]
        [PropertyOrder(1)]
        [ECProperty("Parameters.BridgeHeight")]
        [UnitScale(Units.Millimeter)]
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

        private double m_bridgeWidth = 580;

        [Category("联系桥")]
        [DisplayName("桥宽(mm)")]
        [PropertyOrder(2)]
        [ECProperty("Parameters.BridgeWidth")]
        [UnitScale(Units.Millimeter)]
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

        private double m_bridgeTopBoardThickness = 120;

        [Category("联系桥")]
        [DisplayName("顶板厚(mm)")]
        [PropertyOrder(3)]
        [ECProperty("Parameters.BridgeTopBoardThickness")]
        [UnitScale(Units.Millimeter)]
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

        private double m_bridgeSupportWidth = 120;

        [Category("联系桥")]
        [DisplayName("支腿宽(mm)")]
        [PropertyOrder(4)]
        [ECProperty("Parameters.BridgeSupportWidth")]
        [UnitScale(Units.Millimeter)]
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

        private double m_bridgeChamferWidth = 50;

        [Category("联系桥")]
        [DisplayName("倒角尺寸(mm)")]
        [PropertyOrder(5)]
        [ECProperty("Parameters.BridgeChamferWidth")]
        [UnitScale(Units.Millimeter)]
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

        public bool ValidateParameters()
        {
            double cushioncapHalfBottomWidth = (CushioncapRearToeLength + DolphinBottomWidth + CushioncapFrontToeLength) / 2;
            if (DolphinBottomElevation >= DolphinTopElevation
                || CushioncapBottomElevation >= DolphinTopElevation
                || PileTopElevation >= DolphinBottomElevation
                || PileTopElevation <= CushioncapBottomElevation
                || PileBottomElevation >= PileTopElevation
                || PileOffsetPerpendicularRiver <= PileDiameter / 2
                || PileOffsetPerpendicularRiver >= cushioncapHalfBottomWidth - PileDiameter / 2)
                return false;
            return true;
        }


        /// <summary>
        /// Clone the current data in meter unit
        /// </summary>
        /// <returns></returns>
        public GuidewallP2Data CloneGuidewallDataP2InMeters()
        {
            GuidewallP2Data data = (GuidewallP2Data)MemberwiseClone();
            double scale = 1;
            foreach (var prop in data.GetType().GetProperties())
            {
                UnitScaleAttribute unitScaleAttri = prop.GetCustomAttribute<UnitScaleAttribute>();
                if (null == unitScaleAttri) continue;
                scale = unitScaleAttri.ToMeterScale();
                if (typeof(double) == prop.PropertyType)
                    prop.SetValue(data, (double)prop.GetValue(data) * scale);
                else if(typeof(int) == prop.PropertyType)
                    prop.SetValue(data, (int)prop.GetValue(data) * scale);
                else if (typeof(long) == prop.PropertyType)
                    prop.SetValue(data, (long)prop.GetValue(data) * scale);
            }
            return data;
        }
    }
}
